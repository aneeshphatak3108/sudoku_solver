#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>
typedef struct column column;
typedef struct row row;
typedef struct node{
    struct node* up;
    struct node* down;
    struct node* left;
    struct node* right;
    struct column* c;
    struct row* r;
    //int id;
}node;//node of the dll representing a 1 in the exact cover matrix

typedef struct root{
    int* visited;
    column* start;//points to the first column
    row* row_start;//points to the first row
    column** access;
    int* node_cnt;
}root;//root is the entry point of the matrix

typedef struct column{
    node* up;//points to the bottommost node of the exact cover matrix
    node* down;//points to the first node of the exact cover matrix
    node* latest;
    struct column* left;
    struct column* right;
    //int size;
    //int id;
    int description[3];
    /*
    description[0] tells the type of constraint
    type 1:Each cell only 1 value
    type 2:Each column has unique values
    type 3:Each row with unique values
    type 4:Each block with unique values
    description[1], description[2] tells r1c1 in case of type1, c1n1 in type2, r1n1 in type3, b1n1 in type4
    */
}column;

typedef struct row{
    node* left;
    node* right;
    struct row* up;
    struct row* down;
    int description[3];
    /*
    r1c2n3 translates as row1 column2 of the sudoku has the value3
    {1, 2, 3} stored in the description integer array
    */
}row;

void init_dlm_col(root* r);
void init_dlm_row(root* r);
void make_access(root* r);//this function is the 1st one to be called in main function
void init_dlm(root* r);//creates the nodes
int find_block(int row, int col);//indexing starts from 1
void reduce_dlm(int row, int col, int val, root* r);
void delete_rnc(column* col, root* r);
void delete_row(row* r, root* groot);
int algox(root* r, int board[10][10]);
column* find_min(root* r);
void uncover_rnc(column* col, root* r);
void display_sudoku(int board[10][10]);
void uncover_row(row* r, root* groot);