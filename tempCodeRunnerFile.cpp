       for (j = 0; j < MAX_SIZE; j++) {
            if (newGeneration->grid[i][j] != generation->grid[i][j]) {
                return false;  // As gerações são diferentes
            }
        }
    }

    return true;  // As gerações são iguais
}