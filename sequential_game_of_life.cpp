#include <stdio.h>
#include <stdbool.h>
#include <math.h>

//#define MAX_SIZE 2048
#define MAX_SIZE 100
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
            generation->grid[i] = (float *)malloc(MAX_SIZE * sizeof(float))
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

int GetNeighbors(float **grid, size_t i, size_t j) {
    size_t x, y;
    int nCells = 0;
    
    for(x = (i - 1); x < (i + 3); x++) {
        for(y = (j - 1); y < (j + 3); y++) {
            if(x == i && y == j) continue;
            else if(grid[x][y] == 1.0) nCells++;
        }
    }
    
    return nCells;
}

bool IsAlive(float **grid, size_t i, size_t j) {
    return ((grid[i][j] == 1.0) ? true : false);
}

float AverageNeighbors(float **grid, size_t i, size_t j) {
    return (GetNeighbors(grid, i, j) / 8);
}

void CellUpdate(float **grid, size_t i, size_t j, int nCells) {
    if(IsAlive(grid, i, j) == true) {
        if(nCells >= 2 && nCells <= 3) {
            grid[i][j] = 1.0;
        } else {
            grid[i][j] = 0.0;
        }
    } else {
        if(nCells == 3) {
            grid[i][j] = round(AverageNeighbors(grid, i, j));
        }
    }
}

int main(int argc, char **argv) {
    Generation gen[MAX_GEN];
    return 0;
}
