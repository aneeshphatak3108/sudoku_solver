#include "header.h"
#include "logic.c"
void readAndSolveSudoku(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Failed to open Sudoku file");
        return;
    }

    int m;
    if (fscanf(file, "%d", &m) != 1) {
        printf("Invalid Sudoku format in file: %s\n", filename);
        fclose(file);
        return;
    }

    int board[m][m];
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < m; j++) {
            if (fscanf(file, "%d", &board[i][j]) != 1) {
                printf("Invalid Sudoku format in file: %s\n", filename);
                fclose(file);
                return;
            }
        }
    }
    fclose(file);

    // Solve the Sudoku
    printf("Solving Sudoku from file: %s\n", filename);
    if (!solve_bitmanipulation(m, board)) {
        printf("incorrect sudoku puzzle\n");
        return;
    }
    print_sudoku(m, board);
}

int main() {
    char filename[256];
    FILE *fileList = fopen("file_list.txt", "r");
    if (!fileList) {
        perror("Failed to open file list");
        return 1;
    }

    printf("Available Sudoku files:\n");
    while (fgets(filename, sizeof(filename), fileList)) {
        // Remove newline character
        filename[strcspn(filename, "\n")] = '\0';
        printf("%s\n", filename);
    }
    rewind(fileList); // Reset the file pointer to the start

    char selectedFile[256];
    printf("Enter the name of the Sudoku file to solve: ");
    scanf("%255s", selectedFile);

    int found = 0;
    while (fgets(filename, sizeof(filename), fileList)) {
        filename[strcspn(filename, "\n")] = '\0';
        if (strcmp(filename, selectedFile) == 0) {
            found = 1;
            break;
        }
    }
    fclose(fileList);

    if (found) {
        readAndSolveSudoku(selectedFile);
    } else {
        printf("File %s not found in the list.\n", selectedFile);
    }

    return 0;
}