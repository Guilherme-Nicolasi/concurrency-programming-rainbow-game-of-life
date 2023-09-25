#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <pthread.h>

#define MAX_SIZE 2048
#define MAX_GEN 2001
#define MAX_NEIGHBORS 8
#define NUM_THREADS 4

typedef struct {
    float **grid;
} Generation;

typedef struct {
    size_t start;
    size_t end;
    Generation *generation;
    Generation *newGeneration;
} Thread;

Generation *InitGeneration(void);
void AddInitialCells(Generation *generation);
int GetNeighbors(float **grid, size_t i, size_t j);
bool IsAlive(float **grid, size_t i, size_t j);
float AverageNeighbors(int nCells);
void CellUpdate(float **grid, float **newGrid, size_t i, size_t j, int nCells);
void *NewGeneration(void *arg);
long long TotalLivingCells(Generation *generation);
void PrintGrid(Generation *generation);
void FreeGeneration(Generation *generation);

int main(int argc, char **argv) {
    size_t i, j;
    long long totalLivingCells;

    pthread_t threads[NUM_THREADS];
    Thread thread[NUM_THREADS];

    Generation *generation = InitGeneration();
    if(generation == NULL) {
        return -1;
    }
    
    AddInitialCells(generation);
    
    printf("** Rainbow Game of Life\nCondição inicial: %lld\n", TotalLivingCells(generation));
    for(i = 1; i < (MAX_GEN - 1); i++) {
        Generation *newGeneration = InitGeneration();
        if(newGeneration == NULL) {
            break;
        }
        
        for(j = 0; j < NUM_THREADS; j++) {
            thread[j].start = (j * (MAX_SIZE / NUM_THREADS));
            thread[j].end = ((j + 1) * (MAX_SIZE / NUM_THREADS));
            thread[j].generation = generation;
            thread[j].newGeneration = newGeneration;
            pthread_create(&threads[j], NULL, NewGeneration, &thread[j]);
        }

        for(j = 0; j < NUM_THREADS; j++) {
            pthread_join(threads[j], NULL);
        }
        
        totalLivingCells = TotalLivingCells(newGeneration);
        printf("Geração %zu: %lld\n", (size_t)i, totalLivingCells);
        
        if(i == (MAX_GEN - 1)) {
            FreeGeneration(newGeneration);
        }

        generation = newGeneration;
    }

    printf("Última geração (%zu iterações): %lld células vivas\n", (size_t)(MAX_GEN - 1), totalLivingCells);
    
    FreeGeneration(generation);
    return 0;
}

Generation *InitGeneration(void) {
    Generation *generation = (Generation *)malloc(sizeof(Generation));
    if(generation == NULL) {
        return NULL;
    }
    
    generation->grid = (float **)malloc(MAX_SIZE * sizeof(float *));
    if(generation->grid == NULL) {
        free(generation);
        return NULL;
    }
    
    size_t i, j;
    for(i = 0; i < MAX_SIZE; i++) {
        generation->grid[i] = (float *)malloc(MAX_SIZE * sizeof(float));
        if(generation->grid[i] == NULL) {
            FreeGeneration(generation);
        }

        memset(generation->grid[i], 0, MAX_SIZE * sizeof(float));
    }
    
    return generation;
}

void AddInitialCells(Generation *generation) {
    size_t i = 1, j = 1;
    generation->grid[i][j + 1] = 1.0;
    generation->grid[i + 1][j + 2] = 1.0;
    generation->grid[i + 2][j] = 1.0;
    generation->grid[i + 2][j + 1] = 1.0;
    generation->grid[i + 2][j + 2] = 1.0;
    
    i = 10; j = 30;
    generation->grid[i][j + 1] = 1.0;
    generation->grid[i][j + 2] = 1.0;
    generation->grid[i + 1][j] = 1.0;
    generation->grid[i + 1][j + 1] = 1.0;
    generation->grid[i + 2][j + 1] = 1.0;
}

int GetNeighbors(float **grid, size_t i, size_t j) {
    int nCells = 0;

    int x, y;
    for(x = -1; x <= 1; x++) {
        for(y = -1; y <= 1; y++) {
            if(!(x == 0 && y == 0)) {
                //Periodic Boundary
                if(grid[(i + x + MAX_SIZE) % MAX_SIZE][(j + y + MAX_SIZE) % MAX_SIZE] == 1.0) {
                    nCells++;
                }
            }
        }
    }

    return nCells;
}

bool IsAlive(float **grid, size_t i, size_t j) {
    return (grid[i][j] == 1.0);
}

float AverageNeighbors(int nCells) {
    return ((float)nCells / MAX_NEIGHBORS);
}

void CellUpdate(float **grid, float **newGrid, size_t i, size_t j, int nCells) {
    if(IsAlive(grid, i, j) == true) {
        if(nCells >= 2 && nCells <= 3) {
            newGrid[i][j] = 1.0;
        } else {
            newGrid[i][j] = 0.0;
        }
    } else if(AverageNeighbors(nCells) > 0.0 && nCells == 3) {
        newGrid[i][j] = 1.0;
    }
}

void *NewGeneration(void *arg) {
    size_t i, j;
    Thread *thread = (Thread *)arg;
    for(i = thread->start; i < thread->end; i++) {
        for(j = 0; j < MAX_SIZE; j++) {
            CellUpdate(thread->generation->grid, thread->newGeneration->grid, i, j, GetNeighbors(thread->generation->grid, i, j));
        }
    }

    pthread_exit(NULL);
    return NULL;
}

long long TotalLivingCells(Generation *generation) {
    long long totalCells = 0;
    
    size_t i, j;
    for(i = 0; i < MAX_SIZE; ++i) {
        for(j = 0; j < MAX_SIZE; ++j) {
            if(generation->grid[i][j] > 0.0) {
                totalCells++;
            }
        }
    }
    
    return totalCells;
}

void FreeGeneration(Generation *generation) {
    if(generation != NULL) {
        size_t i;
        for(i = 0; i < MAX_SIZE; i++) {
            free(generation->grid[i]);
        }
        
        free(generation->grid);
        free(generation);
        generation = NULL;
    }
}
