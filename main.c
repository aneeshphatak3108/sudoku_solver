#include "header.h"

int main() {
    root r;
    make_access(&r);
    init_dlm_col(&r);
    init_dlm_row(&r);
    init_dlm(&r);
    int board[10][10] = {{0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                        {0, 3, 0, 6, 5, 0, 8, 4, 0, 0},
                       {0, 5, 2, 0, 0, 0, 0, 0, 0, 0},
                       {0, 0, 8, 7, 0, 0, 0, 0, 3, 1},
                       {0, 0, 0, 3, 0, 1, 0, 0, 8, 0},
                       {0, 9, 0, 0, 8, 6, 3, 0, 0, 5},
                       {0, 0, 5, 0, 0, 9, 0, 6, 0, 0 },
                       {0, 1, 3, 0, 0, 0, 0, 2, 5, 0 },
                       {0, 0, 0, 0, 0, 0, 0, 0, 7, 4 },
                       {0, 0, 0, 5, 2, 0, 6, 3, 0, 0 }}; 
    //reducing the dlm from the given sudoku
    for (int i = 1; i <= 9; i++) {
        for (int j = 1; j <= 9; j++) {
            if (board[i][j] != 0)
                reduce_dlm(i, j, board[i][j], &r);
        }
    }
    int x = algox(&r, board);
    display_sudoku(board);
}