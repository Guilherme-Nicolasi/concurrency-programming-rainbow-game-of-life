#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <iostream>
#include <omp.h>

using namespace std;

//#define MAX_SIZE 2048
#define MAX_SIZE 50
//#define MAX_GEN 2000
#define MAX_GEN 10

typedef struct {
    float **grid;
} Generation;

Generation *StartGame(void) {
    Generation *generation = (Generation *)malloc(sizeof(Generation));
    if(generation == NULL) {
        perror("Error: Wasn't possible to start the game.\n");
        return NULL;
    } else {
        generation->grid = (float **)malloc(MAX_SIZE * sizeof(float *));
        if(generation->grid == NULL) {
            perror("Error: Wasn't possible to create the first grid.\n");
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
    int lin = 1, col = 1;
    generation->grid[lin][col+1] = 1.0;
    generation->grid[lin+1][col+2] = 1.0;
    generation->grid[lin+2][col] = 1.0;
    generation->grid[lin+2][col+1] = 1.0;
    generation->grid[lin+2][col+2] = 1.0;

    lin =10; col = 30;
    generation->grid[lin][col+1] = 1.0;
    generation->grid[lin][col+2] = 1.0;
    generation->grid[lin+1][col] = 1.0;
    generation->grid[lin+1][col+1] = 1.0;
    generation->grid[lin+2][col+1] = 1.0;
}

void FreeGeneration(Generation *generation) {
    if (generation) {
        for (int i = 0; i < MAX_SIZE; i++) {
            free(generation->grid[i]);
        }

        free(generation->grid);
        free(generation);
    }
}

void PrintBoard(const Generation *generation) {
    for (int i = 0; i < MAX_SIZE; ++i) {
        for (int j = 0; j < MAX_SIZE; ++j) {
            cout << generation->grid[i][j] << " ";
        }
        cout << endl;
    }
}

int TotalLivingCells(const Generation *generation) {
    int totalCels = 0;
    for (int i = 0; i < MAX_SIZE; ++i) {
        for (int j = 0; j < MAX_SIZE; ++j) {
            if(generation->grid[i][j] < 0) {
                totalCels++;
            }
        }
    }
    return totalCels;
}


int GetNeighbors(Generation *generation) {
    size_t x, y;
    int nCells = 0;

    double time_spent = 0.0;

    clock_t start_time = clock();
    for(x = (MAX_SIZE - 1); x < (MAX_SIZE + 3); x++) {
        for(y = (MAX_SIZE - 1); y < (MAX_SIZE + 3); y++) {
            if(x == MAX_SIZE && y == MAX_SIZE) continue;
            else if(generation->grid[x][y] == 1.0) nCells++;
        }
    }

    clock_t end_time = clock();
    time_spent += (double)(end_time - start_time) / CLOCKS_PER_SEC;


    cout << time_spent;

    return nCells;
}

bool IsAlive(Generation *generation, size_t i, size_t j) {
    return ((generation->grid[i][j] == 1.0) ? true : false);
}

float AverageNeighbors(Generation *generation) {
    return (GetNeighbors(generation) / 8);
}

void CellUpdate(Generation *generation, size_t i, size_t j, int nCells) {
    if(IsAlive(generation, i, j) == true) {
        if(nCells >= 2 && nCells <= 3) {
            generation->grid[i][j] = 1.0;
        } else {
            generation->grid[i][j] = 0.0;
        }
    } else {
        if(nCells == 3) {
            generation->grid[i][j] = round(AverageNeighbors(generation));
        }
    }
}

int main(int argc, char **argv) {
  //  Generation gen[MAX_GEN];
    Generation *myGeneration = StartGame();

    AddInitialCells(myGeneration);

    int total = TotalLivingCells(myGeneration);

    cout << total;

    GetNeighbors(myGeneration);

    PrintBoard(myGeneration);

    FreeGeneration(myGeneration);


    return 0;
}
