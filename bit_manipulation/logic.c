#include "header.h"

//basically this function returns how many times we have to leftshift 1 so that it will be in its correct position
int tileValue(int n, int board[n][n], int row, int col) {
    /* get the integer value of a tile
    return -1 if the tile is empty */
    if (board[row][col] == 0) {
        return -1;
    } else {
        return board[row][col] - 1;
    }
}

int intToChar(int value) {
    /* convert an integer into a sudoku number (0 -> '1', 1 -> '2', etc.)*/
    //basically index 0 in rows[row] tells us if '1' is present in that row
    return (1 + value);
}

int getSquare(int n, int row, int col) {
    int k = (int)sqrt(n);
    /* calculate a tile's sqaure based on its row and column */
    return ((row / k) * k) + (col / k);
}

bool prepare(int n, int board[n][n], int rows[n], int cols[n], int squares[n]) {
    for (int row = 0; row < n; row++) {
        for (int col = 0; col < n; col++) {
            int square = getSquare(n, row, col);
            int intValue = tileValue(n, board, row, col);
            
            if (intValue != -1) {
                int candidateBits = rows[row] | cols[col] | squares[square];
                if ((candidateBits >> intValue) & 1) {
                    return false; // error
                }               
                // Set the bit for this value
                rows[row] |= (1 << intValue);
                cols[col] |= (1 << intValue);
                squares[square] |= (1 << intValue);
            }
        }
    }
    return true;
}

bool solveRecur(int n, int board[n][n], int rows[n], int cols[n], int squares[n], int row, int col) {
    /* recursively solve the entire board */
    if (row == n) {
        return true;
    }
    int nextrow = (col == (n-1) ? row + 1 : row);
    int nextcol = (col == (n-1) ? 0 : col + 1);

    int square = getSquare(n, row, col);

    int intValue = tileValue(n, board, row, col);

    if (intValue != -1) { // skip tiles that already have numbers
        return solveRecur(n, board, rows, cols, squares, nextrow, nextcol);
    }
    // get the possible values for the tile's row, column and square
    int rowBits = rows[row];
    int colBits = cols[col];

    int squareBits = squares[square];

    // OR them all together to get the final candidates
    int candidateBits = rowBits | colBits | squareBits;

    for (int i = 0; i < n; i++) {
        // iterate through each bit and only attempt the 0 bits 
        if ((candidateBits >> i & 1) == 0) { 
            // set the bit to 1
            rows[row] |= (1 << i);//right shifting 1, i times and doing OR with rows[row]
            cols[col] |= (1 << i);
            squares[square] |= (1 << i);
            board[row][col] = intToChar(i);
            if (solveRecur(n, board, rows, cols, squares, nextrow, nextcol)) {
                return true;
            }
            // if the bit didn't work out, reset it to 0
            rows[row] &= ~(1 << i);//resetting that specific bit since it didnt work
            cols[col] &= ~(1 << i);
            squares[square] &= ~(1 << i);
        }
    }

    board[row][col] = 0;
    return false;
}

bool solve_bitmanipulation(int n, int board[n][n]) {
    //sets the 3 bitmasking arrays
    int k =(int)sqrt(n);
    int rows[n];
    int cols[n];
    int squares[n];
    for (int i = 0; i < n; i++) {
        rows[i] = 0;
        cols[i] = 0;
        squares[i] = 0;
    }
    if (!prepare(n, board, rows, cols, squares)) {
        return false;
    }
    return solveRecur(n, board, rows, cols, squares, 0, 0);
}


void print_sudoku(int n, int board[n][n]) {
    int k = (int)sqrt(n);
    for (int row = 0; row < n; row++) {
        if (row % k == 0 && row != 0) {
            for (int i = 0; i < n + k - 1; i++) {
                printf("-");
            }
            printf("\n");
        }
        for (int col = 0; col < n; col++) {
            if (col % k == 0 && col != 0) {
                printf(" |");
            }
            printf(" %d", board[row][col]);
        }
        printf("\n");
    }
}