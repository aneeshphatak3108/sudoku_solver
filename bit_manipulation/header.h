#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>
int tileValue(int n, int board[n][n], int row, int col);
int intToChar(int value);
int getSquare(int n, int row, int col);
bool prepare(int n, int board[n][n], int rows[n], int cols[n], int squares[n]);
bool solveRecur(int n, int board[n][n], int rows[n], int cols[n], int squares[n], int row, int col);
bool solve_bitmanipulation(int n, int board[n][n]);
void print_sudoku(int n, int board[n][n]);
void sudoku_generator(int n, int sudoku[n][n]);
void swap(int A[], int i, int j);