#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "sudoku.h"
/*#include "queue.h"*/
#include "stack.h"
#include "mixed.h"
#define N 9
#define EMPTY 0

int main(){
    stack U, R;
    sinit(&R);
    sinit(&U);
    int sudoku[N][N] = { { 5, 3, 0, 0, 7, 0, 0, 0, 0 },
                         { 6, 0, 0, 1, 9, 5, 0, 0, 0 },
                         { 0, 9, 8, 0, 0, 0, 0, 6, 0 },
                         { 8, 0, 0, 0, 6, 0, 0, 0, 3 },
                         { 4, 0, 0, 8, 0, 3, 0, 0, 1 },
                         { 7, 0, 0, 0, 2, 0, 0, 0, 6 },
                         { 0, 6, 0, 0, 0, 0, 2, 8, 0 },
                         { 0, 0, 0, 4, 1, 9, 0, 0, 5 },
                         { 0, 0, 0, 0, 8, 0, 0, 7, 9 } };
    int solved_sudoku[N][N];
    int take_ip_sudoku[N][N];
    int skeleton_sudoku[N][N];
    int new_sudoku[N][N];
    generate_skeleton_sudoku(sudoku, skeleton_sudoku);

    copy_sudoku(sudoku, solved_sudoku);
    
    copy_sudoku(sudoku, take_ip_sudoku);
    
    print_sudoku(solved_sudoku, skeleton_sudoku);
    
    /*solve_sudoku(solved_sudoku);*/
    take_input(&U, &R, take_ip_sudoku, skeleton_sudoku, 1, 3, 4);
    
    take_input(&U, &R, take_ip_sudoku, skeleton_sudoku, 1, 4, 4);

    hint(take_ip_sudoku);

    /*take_input(&U, &R, take_ip_sudoku, skeleton_sudoku, 1, 9, 4);*/

    print_sudoku(take_ip_sudoku, skeleton_sudoku);
    
    undo(&U, &R, take_ip_sudoku);
    undo(&U, &R, take_ip_sudoku);
    undo(&U, &R, take_ip_sudoku);
    undo(&U, &R, take_ip_sudoku);

    print_sudoku(take_ip_sudoku, skeleton_sudoku);

    redo(&U, &R, take_ip_sudoku);
    redo(&U, &R, take_ip_sudoku);
    redo(&U, &R, take_ip_sudoku);
    redo(&U, &R, take_ip_sudoku);

    print_sudoku(take_ip_sudoku, skeleton_sudoku);

    char name[50] = "File!_.csv";
    FILE *fp;
    fp = fopen(name, "w");
    fputs("hello it is a csv file\nisnt it amazing!!!\n", fp);
    fclose(fp);
    generate_sudoku(new_sudoku, 5);
    generate_skeleton_sudoku(new_sudoku, skeleton_sudoku);
    print_sudoku(new_sudoku, skeleton_sudoku);
    hint(new_sudoku);
    take_input(&U, &R, new_sudoku, skeleton_sudoku, 1, 4, 2);
    print_sudoku(new_sudoku, skeleton_sudoku);
    hint(new_sudoku);
    solve_sudoku(new_sudoku);
    print_sudoku(new_sudoku, skeleton_sudoku);
    return 0;
}