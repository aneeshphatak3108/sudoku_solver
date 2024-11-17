#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "sudoku.h"
/*#include "queue.h"*/
#include "stack.h"
#include "mixed.h"
#define N 9
#define EMPTY 0

/*void qinit(queue *q){
    q -> head = NULL;
    q -> tail = NULL;
    return;
}

int qfull(queue *q){
    return 0;
}

int qempty(queue *q){
    if(q -> head == NULL)
        return 1;
    else
        return 0;
}

void enq(queue *q, int val){
    node *new = (node *)malloc(sizeof(node));
    if(!new){
        printf("memory Allocation Failed\n");
        return;
    }
    new -> d = val;
    if(qempty(q)){
        q -> head = new;
        q -> tail = new;
        new -> next = new;
    }
    else{
        q -> tail -> next = new;
        q -> tail = new;
        new -> next = q -> head;
    }
    return;
}

int deq(queue *q){
    if(qempty(q)){
        printf("Empty Queue\n");
        return -99999;
    }
    else if(q -> head == q -> tail){
        int val = q -> head -> d;
        free(q -> head);
        q -> head = q -> tail = NULL;
        return val;
    }
    else{
        int val = q -> head -> d;
        node *temp = q -> head;
        q -> head = q -> head -> next;
        q -> tail -> next = q -> head;
        free(temp);
        return val;
    }
}*/

/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/

void sinit(stack *s){
    s -> top = NULL;
    return;
}

int sEmpty(stack *s){
    if(s -> top == NULL){
        return 1;
    }
    else{
        return 0;
    }
}

void push(stack *s, data prev_step){
    stacknode *nn = (stacknode *)malloc(sizeof(stacknode));
    nn -> d.num = prev_step.num;
    nn -> d.row = prev_step.row;
    nn -> d.col = prev_step.col;
    nn -> next = s -> top;
    s -> top = nn;
    return;
}

data pop(stack *s){
    data x;
    x.num = -1;
    x.row = -1;
    x.col = -1;
    if(sEmpty(s)){
        return x;
    }
    else{
        stacknode *temp = s -> top;
        x.num = temp -> d.num;
        x.row = temp -> d.row;
        x.col = temp -> d.col;
        s -> top = s -> top -> next;
        free(temp);
        return x;
    }
}

data peek(stack *s){
    data x;
    x.num = -1;
    x.row = -1;
    x.col = -1;
    if(sEmpty(s)){
        return x;
    }
    else{
        stacknode *temp = s -> top;
        x.num = temp -> d.num;
        x.row = temp -> d.row;
        x.col = temp -> d.col;
        return x;
    }
}

void freestack(stack *s){
    if(sEmpty(s))
        return;
    while(!sEmpty(s)){
        stacknode *temp = s -> top;
        s -> top = s -> top -> next;
        free(temp);
    }
    return;
}

/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/

int valid_in_grid(int sudoku[N][N], int row, int col, int val){
    row /= 3;
    col /= 3;
    row *= 3;
    col *= 3;
    for(int i = row; i < row + 3; i++){
        for(int j = col; j < col + 3; j++){
            if(sudoku[i][j] == val)
                return 0;
        }
    }
    return 1;
}

int valid_in_row(int sudoku[N][N], int row, int val){
    for(int col = 0; col < N; col++){
        if(sudoku[row][col] == val)
            return 0;
    }
    return 1;
}

int valid_in_col(int sudoku[N][N], int col, int val){
    for(int row = 0; row < N; row++){
        if(sudoku[row][col] == val)
            return 0;
    }
    return 1;
}

int is_safe(int sudoku[N][N], int row, int col, int num){
    if(valid_in_grid(sudoku, row, col, num) && valid_in_row(sudoku, row, num) && valid_in_col(sudoku, col, num))
        return 1;
    return 0;
}

int is_empty_cell(int sudoku[N][N], int row, int col) {
    if(sudoku[row][col] == EMPTY)
        return 1;
    return 0;
}

void copy_sudoku(int sour[N][N], int dest[N][N]){
    for(int row = 0; row < N; row++){
        for(int col = 0; col < N; col++){
            dest[row][col] = sour[row][col];
        }
    }
    return;
}

void generate_skeleton_sudoku(int sudoku[N][N], int skeleton[N][N]){
    for(int row = 0; row < N; row++){
        for(int col = 0; col < N; col++){
            if(sudoku[row][col] != 0)
                skeleton[row][col] = 0;      //permanent element
            else    
                skeleton[row][col] = 1;      //editable element
        }
    }
    return;
}

void print_sudoku(int sudoku[N][N], int skeleton[N][N]){
    for(int row = 0; row < N; row++){
        for(int col = 0; col < N; col++){
            if(skeleton[row][col] == 0)
                printf("\033[1;31m%d\033[0m ", sudoku[row][col]);
            else if(skeleton[row][col] == 1 && sudoku[row][col] != 0)
                printf("\033[1;32m%d\033[0m ", sudoku[row][col]);
            else    
                printf("\033[1m%d\033[0m ", sudoku[row][col]);
            if((col + 1) % (int)sqrt(N) == 0)
                printf(" ");
        }
        printf("\n");
        if((row + 1) % (int)sqrt(N) == 0)
            printf("\n");
    }
    printf("\n");
    return;
}

int solve_sudoku(int sudoku[N][N]) {
    for(int row = 0; row < N; row++){
        for(int col = 0; col < N; col++){
            if(is_empty_cell(sudoku, row, col)){
                for(int num = 1; num <= N; num++){
                    if(is_safe(sudoku, row, col, num)){
                        sudoku[row][col] = num;
                        if(solve_sudoku(sudoku))
                            return 1;
                        sudoku[row][col] = EMPTY;
                    }
                }
                return 0;
            }   
        }
    }
    return 1;
}

void take_input(stack *u, stack *r, int sudoku[N][N], int skeleton[N][N], int row, int col, int num){
    if((row > N || row < 1) || (col > N || col < 1) || (num > N || num < 1) || skeleton[row - 1][col - 1] == 0){
        printf("Invalid inputs!\nRetry.\n\n");
        return;
    }
    data prev_step, next_step;
    prev_step.row = next_step.row = row;
    prev_step.col = next_step.col = col;
    prev_step.num = sudoku[row - 1][col - 1];
    next_step.num = num;
    push(u, prev_step);
    freestack(r);
    sudoku[row - 1][col - 1] = num;
    return;    
}

void undo(stack *u, stack *r, int sudoku[N][N]){
    if(sEmpty(u)){
        printf("No more steps to undo.\n\n");
        return;
    }
    data prev_step = pop(u);
    data next_step;
    next_step.row = prev_step.row;
    next_step.col = prev_step.col;
    next_step.num = sudoku[prev_step.row - 1][prev_step.col - 1];
    push(r, next_step);
    if(prev_step.num == -1)
        return;
    sudoku[prev_step.row - 1][prev_step.col - 1] = prev_step.num;
    return;
}

void redo(stack *u, stack *r, int sudoku[N][N]){
    if(sEmpty(r)){
        printf("No more steps to redo.\n\n");
        return;
    }
    data next_step = pop(r);
    data prev_step;
    prev_step.row = next_step.row;
    prev_step.col = next_step.col;
    prev_step.num = sudoku[next_step.row - 1][next_step.col - 1];
    push(u, prev_step);
    if(prev_step.num == -1)
        return;
    sudoku[next_step.row - 1][next_step.col - 1] = next_step.num;
    return;
}

int count_row_elements(int sudoku[N][N], int row){
    int count = 0;
    for(int col = 0; col < N; col++){
        if(sudoku[row][col] != 0)
            count++;
    }
    return count;
}

int count_col_elements(int sudoku[N][N], int col){
    int count = 0;
    for(int row = 0; row < N; row++){
        if(sudoku[row][col] != 0)
            count++;
    }
    return count;
}

int count_grid_elements(int sudoku[N][N], int row, int col){
    int count = 0;
    row /= 3;
    col /= 3;
    row *= 3;
    col *= 3;
    for(int i = row; i < row + 3; i++){
        for(int j = col; j < col + 3; j++){
            if(sudoku[i][j] != 0)
                count++;
        }
    }
    return count;
}

void hint(int sudoku[N][N]){
    int hint_col, hint_row, row_count, col_count, grid_count, temp_sum, max_sum;
    temp_sum = max_sum = col_count = row_count = grid_count = 0;
    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
            if(sudoku[i][j] == 0){
                row_count = count_row_elements(sudoku, i);
                col_count = count_col_elements(sudoku, j);
                grid_count = count_grid_elements(sudoku, i, j);
                temp_sum = row_count + col_count + grid_count;
                if(temp_sum > max_sum){
                    max_sum = temp_sum;
                    hint_row = i + 1;
                    hint_col = j + 1;
                }
            }
        }
    }
    printf("You can try to determine the next element in row %d and column %d.\n\n", hint_row, hint_col);
    return;
}

void remove_cells(int sudoku[N][N], int cells_to_remove) {
    // Create a list of all cell positions
    int positions[N * N][2];
    int index = 0;

    // Fill positions with row and column coordinates
    for (int row = 0; row < N; row++) {
        for (int col = 0; col < N; col++) {
            positions[index][0] = row;
            positions[index][1] = col;
            index++;
        }
    }

    // Shuffle the list of positions to randomize
    for (int i = 0; i < N * N; i++) {
        int rand_index = rand() % (N * N);
        int temp_row = positions[i][0];
        int temp_col = positions[i][1];
        positions[i][0] = positions[rand_index][0];
        positions[i][1] = positions[rand_index][1];
        positions[rand_index][0] = temp_row;
        positions[rand_index][1] = temp_col;
    }

    // Remove cells based on the shuffled positions
    int count = 0;
    for (int i = 0; i < N * N && count < cells_to_remove; i++) {
        int row = positions[i][0];
        int col = positions[i][1];
        if (sudoku[row][col] != 0) {
            sudoku[row][col] = 0;
            count++;
        }
    }
}

void fill_diagonal(int sudoku[N][N]){
    for (int i = 0; i < N; i += 3){
    int num_list[N] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
        shuffle(num_list, N); // Shuffle the numbers for randomness
        for (int row = 0; row < 3; row++){
            for (int col = 0; col < 3; col++){
                sudoku[i + row][i + col] = num_list[row * 3 + col];
            }
        }
    }
}

void shuffle(int arr[], int n){
    for (int i = 0; i < n; i++){
        int rand_index = rand() % n;
        int temp = arr[i];
        arr[i] = arr[rand_index];
        arr[rand_index] = temp;
    }
}

void generate_sudoku(int sudoku[N][N], int difficulty){
    int skeleton_sudoku[N][N];
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            sudoku[i][j] = 0;
    fill_diagonal(sudoku);
    solve_sudoku(sudoku);
    generate_skeleton_sudoku(sudoku, skeleton_sudoku);
    print_sudoku(sudoku, skeleton_sudoku);
    int cells_to_remove;
    if (difficulty == 1)       // Easy
        cells_to_remove = 30;
    else if (difficulty == 2)  // Medium
        cells_to_remove = 40;
    else                       // Hard
        cells_to_remove = 50;
    remove_cells(sudoku, cells_to_remove);
}