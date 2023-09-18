#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

//#define MAX_SIZE 2048
#define MAX_SIZE 20
//#define MAX_GEN 2000
#define MAX_GEN 5

typedef struct {
    float **grid;
} Generation;

Generation *InitGeneration(void) {
    Generation *generation = (Generation *)malloc(sizeof(Generation));
    if(generation == NULL) {
        return NULL;
    } else {
        generation->grid = (float **)malloc(MAX_SIZE * sizeof(float *));
        if(generation->grid == NULL) {
            free(generation);
            return NULL;
        }
        
        size_t i, j;
        for(i = 0; i < MAX_SIZE; i++) {
            generation->grid[i] = (float *)malloc(MAX_SIZE * sizeof(float));
            if(generation->grid[i] == NULL) {
                for(j = 0; j < i; j++) {
                    free(generation->grid[j]);
                }
                free(generation->grid);
                free(generation);
                return NULL;
            }
            
            for(j = 0; j < MAX_SIZE; j++) {
                generation->grid[i][j] = 0.0;
            }
        }
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
    size_t x, y;
    int nCells = 0;
    
    if((i > 0 || i < MAX_SIZE) && (j > 0 || j < MAX_SIZE)) {
        for(x = (i - 1); x <= (i + 1); x++) {
            for(y = (j - 1); y <= (j + 1); y++) {
                if((x >= 0 && x < MAX_SIZE) && (y >= 0 && y < MAX_SIZE) && !(x == i && y == j)) {
                    if (grid[x][y] == 1.0) {
                        nCells++;
                    }
                }
            }
        }
    }
    
    return nCells;
}

bool IsAlive(float **grid, size_t i, size_t j) {
    return ((grid[i][j] == 1.0) ? true : false);
}

float AverageNeighbors(int nCells) {
    return ((float)(nCells / 8));
}

void CellUpdate(float **grid, float **newGrid, size_t i, size_t j, int nCells) {
    if(IsAlive(grid, i, j) == true) {
        if(nCells >= 2 && nCells <= 3) {
            newGrid[i][j] = 1.0;
        } else {
            newGrid[i][j] = 0.0;
        }
    } else if(nCells == 3) {
        newGrid[i][j] = 1.0;
    }
}

void NewGeneration(Generation *newGeneration, Generation *generation) {
    size_t row, col;
    for(row = 0; row < MAX_SIZE; row++) {
        for(col = 0; col < MAX_SIZE; col++) {
            CellUpdate(generation->grid, newGeneration->grid, row, col, GetNeighbors(generation->grid, row, col));
        }
    }
}

void PrintGrid(Generation *generation) {
    size_t i, j;
    for(i = 0; i < MAX_SIZE; ++i) {
        for(j = 0; j < MAX_SIZE; ++j) {
            printf("%.0f ", generation->grid[i][j]);
        } printf("\n");
    }
}

void FreeGeneration(Generation *generation) {
    if(generation) {
        size_t i;
        for(i = 0; i < MAX_SIZE; i++) {
            free(generation->grid[i]);
        }
        
        free(generation->grid);
        free(generation);
    }
}

int main(int argc, char **argv) {
    Generation *firstGeneration = InitGeneration();
    AddInitialCells(firstGeneration);
    
    Generation *generations[MAX_GEN];
    generations[0] = firstGeneration;
    
    size_t i, row, col;
    for(i = 1; i < MAX_GEN; i++) {
        generations[i] = InitGeneration();
        NewGeneration(generations[i], generations[i - 1]);
    }
    
    for(i = 0; i < MAX_GEN; i++) {
        PrintGrid(generations[i]);
        printf("\n");
    }
    
    for(i = 0; i < MAX_GEN; i++) {
        FreeGeneration(generations[i]);
    }
    
    return 0;
}
