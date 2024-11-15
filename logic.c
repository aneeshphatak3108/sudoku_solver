#include "header.h"
void init_dlm_col(root* r) {
    r->start = NULL;
    int access_index = 0;
    column* trailing = NULL;
    for (int j = 1; j <= 4; j++) {
        for (int i = 0; i < 81; i++) {
            column* p = (column*)malloc(sizeof(column));
            r->access[access_index++] = p;
            p->down = NULL;
            p->up = NULL;
            p->latest = NULL;
            p->description[0] = j;
            p->description[1] = i/9 + 1;
            p->description[2] = i%9 + 1;
            if (r->start == NULL)
                r->start = p;
            if (trailing != NULL) {
                trailing->right = p;
                p->left = trailing;
            }
            trailing = p;
        }
    }
    trailing->right = r->start;
    r->start->left = trailing;
}

void init_dlm_row(root* r) {
    r->row_start = NULL;
    row* trailing = NULL;
    for (int i = 1; i <= 9; i++) {
        for (int j = 1; j <= 9; j++) {
            for (int k = 1; k <= 9; k++) {
                row* p = (row*)malloc(sizeof(row));
                if (r->row_start == NULL)
                    r->row_start = p;
                p->left = NULL;
                p->right = NULL;
                p->description[2] = k;//r1c1n1 r1c1n2 ...
                p->description[1] = j;
                p->description[0] = i;
                if (trailing != NULL) {
                    trailing->down = p;
                    p->up = trailing;
                }
                trailing = p;
            }
        }
    }
    trailing->down = r->row_start;
    r->row_start->up = trailing; 
}

void make_access(root* r) {
    r->row_start = NULL;
    r->start = NULL;
    r->visited = (int*)malloc(sizeof(int)* 324);
    r->access = (column**)malloc(sizeof(column*)*324);
    r->node_cnt = (int*)malloc(sizeof(int)* 324);
    for (int i = 0; i  < 324; i++) {
        r->access[i] = NULL;
        r->node_cnt[i] = 9;
        r->visited[i] = 0;
    }
}

void init_dlm(root* r) {
    row* trav = NULL;
    int index = 0;
    node* trailing = NULL, *nn = NULL;
    for (int j = 0; j < 729; j++) {
        if (trav == NULL)
            trav = r->row_start;
        //create 4 nodes in each row, 1 for each type of constraint
        for (int i = 0; i < 4; i++) {
            nn = (node*)malloc(sizeof(node));
            if (i == 0) {
                trav->right = nn;
            }
            if (trailing != NULL) {
                trailing->right = nn;
                nn->left = trailing;
            }
            trailing = nn;
            //based on the description of trav row, link the node with suitable column
            if (i==0)
                index = 9*(trav->description[0] - 1) + (trav->description[1] - 1);
            if (i==1)
                index = 81 + 9*(trav->description[1] - 1) + (trav->description[2] - 1);
            if (i==2)
                index = 81*2 + 9*(trav->description[0] - 1) + (trav->description[2] - 1);
            if (i==3)
                index = 81*3 + 9*(find_block(trav->description[0], trav->description[1]) - 1) + (trav->description[2] - 1);

            nn->c = r->access[index];
            nn->r = trav;
            if (nn->c->latest == NULL) {
                nn->c->latest = nn;
                nn->c->down = nn;
            }
            else {
                nn->c->latest->down = nn;
                nn->up = nn->c->latest;
                nn->c->latest = nn;
            }
            //if we are in the last row, then connect up pointer of 1st row to this ptr of last row
            if (j == 728) {
                nn->c->down->up = nn;
                nn->down = nn->c->down;
            }            
        }
        trailing->right = trav->right;
        trav->right->left = trailing;
        trav->left = trailing;//NEWCHANGE
        trav = trav->down;
        trailing = NULL;//NEWCHANGE
    }

    //making all the up pointers of the column node point to the latest nodes(last nodes)
    //making all the down pointers of the latest node point to the first node of the column(down node of the column)
    for (int i = 0; i < 324; i++) {
        r->access[i]->up = r->access[i]->latest;
        r->access[i]->latest->down = r->access[i]->down;
    }
}

//row and col start from 1
//b1//b2//b3
//b4//b5//b6
//b7//b8//b9
int find_block(int row, int col) {
    // Use integer division to determine the block number
    return (row - 1) / 3 * 3 + (col - 1) / 3 + 1;
}



/*UPTO THIS STEP, WE HAVE CREATED THE STRUCTURE OF DOUBLY LINKED MATRIX DLM
THE NEXT 2 STEPS ARE
1)TRAVERSE THE SUDOKU AND REDUCE THE DLM
2)APPLY ALGORITHM X ON REDUCED DLM

Now begins the code of step 2*/

//this function is called in loop while traversing the entire sudoku
// row and col indexing starts from 1
void reduce_dlm(int row, int col, int val, root* r) {
    int block = find_block(row, col);
    delete_rnc(r->access[(row-1)*9 + (col-1)], r);
    delete_rnc(r->access[81 + (col-1)*9 + (val-1)], r);
    delete_rnc(r->access[81*2 + (row-1)*9 + (val-1)], r);
    delete_rnc(r->access[81*3 + (block-1)*9 +(val-1)], r);
}


void delete_rnc(column* col, root* r) {
    node* traverse = col->down;
    col->left->right = col->right;
    col->right->left = col->left;
    if (col->down == NULL)
        return;
//handling the special cases
//if only 1 column
    if (col->left==col && col->right==col) {
        int index = 0;
        index = (col->description[0]-1)*81 + (col->description[1]-1)*9 + (col->description[2]-1);
        r->visited[index] = 1;
        return;
        //basically if there is only 1 column remaining then that means that our soln was right and so no need to further backtrack
    }
//if the column has only 1 node and we have multiple columns
    if (col->down == col->up) {
        col->latest = col->down;
        traverse
    }

    do {
        if (traverse==NULL || traverse->up == NULL || traverse->down == NULL)
            break;
        traverse->left->right = traverse->right;
        traverse->right->left = traverse->left;//above 2 lines are the gist of dancing links
        traverse = traverse->down;
    }
    while(traverse != col->down);
    int index = 0;
    index = (col->description[0]-1)*81 + (col->description[1]-1)*9 + (col->description[2]-1);
    r->visited[index] = 1;
    traverse = col->down;
    do {
        delete_row(traverse->r, r);
        traverse = traverse->down;
    }
    while(traverse != col->down);
}

void delete_row(row* r, root* groot) {
    node* traverse = r->right;
    //not thinking that this is the first row
    //also not thinking that there is only one row
    r->up->down = r->down;
    r->down->up = r->up;
    if (r->right == NULL)
        return;
    do {
        if (traverse == NULL || traverse->up == NULL || traverse->down == NULL)
            break;
        traverse->up->down = traverse->down;
        traverse->down->up = traverse->up;
        //updating the no. of nodes in each column as we are traversing the row
        column* col = traverse->c;
        /*if (col->down == traverse)//if we are deleting the 1st row, then we need to adjust the down pointer of column
            col->down = traverse->down;*/
        int index = 0;
        index = (col->description[0]-1)*81 + (col->description[1]-1)*9 + (col->description[2]-1);
        groot->node_cnt[index] = groot->node_cnt[index] - 1;
        traverse = traverse->right;
    }
    while (traverse != r->right);
}

//NOW BEGINS THE CODE OF ALGORITHM X AND BACKTRACKING
column* find_min(root* r) {
    int min_cnt = 10;//when we are removing a column, we are setting its count in the node_cnt array as 10
    column* min = NULL;
    int i = 0;
    for (i = 0; i < 324; i++) {
        if (r->node_cnt[i] < min_cnt && r->visited[i] == 0) {
            min = r->access[i];
            min_cnt = r->node_cnt[i];
        }
    }
    if (min != NULL)//NEWCHANGE
        r->visited[i] = 1;
    return min;
}



int algox(root* r, int board[10][10]) {
    column* min = find_min(r);
    //if all the columns are exhausted that means that our set of rows make the soln of sudoku
    if (min == NULL)
        return 1;
    int index = 0;
    index = (min->description[0]-1)*81 + (min->description[1]-1)*9 + (min->description[2]-1);
    //our soln was wrong, now backtrack
    if (r->node_cnt[index] == 0)
        return 0;
    node* n = min->down;
    //traverse the selected column min
    do {
        int r_val = n->r->description[0];
        int col_val = n->r->description[1];
        int val = n->r->description[2];
        board[r_val][col_val] = val;

        //cover the chosen row and column
        delete_rnc(min, r);
        if (algox(r, board) == 1)
            return 1;
        //backtrack by uncovering using dancing links
        uncover_rnc(min, r);
        board[r_val][col_val] = 0;
        n = n->down;
    }
    while (n != min->down);
    return 0;
}

void uncover_rnc(column* col, root* r) {
    /*if (col->down == NULL && col->up == NULL && col->latest != NULL) {
        uncover_row(col->latest->r, r);
        col->down = col->latest;
        col->down->left->right = col->down;
        col->down->right->left = col->down;
        col->up = col->latest;
        col->left->right = col;
        col->right->left = col;
        int index = 0;
        index = (col->description[0]-1)*81 + (col->description[1]-1)*9 + (col->description[2]-1);
        r->visited[index] = 0;
        return;
    //}*/
    col->right->left = col;
    col->left->right = col;
    node* traverse = col->down;
    do {  
        if (traverse == NULL || traverse->up == NULL || traverse->down == NULL)
            break; 
        uncover_row(traverse->r, r); 
        traverse->left->right = traverse;
        traverse->right->left = traverse;//dancing links
        traverse = traverse->down;
    }
    while(traverse != col->down);
    int index = 0;
    index = (col->description[0]-1)*81 + (col->description[1]-1)*9 + (col->description[2]-1);
    r->visited[index] = 0; 
}

void display_sudoku(int board[10][10]) {
    for (int i = 1; i <= 9; i++) {
        for (int j = 1; j <= 9; j++) {
            printf("%d ", board[i][j]);
        }
        printf("\n");
    }
}

void uncover_row(row* r, root* groot) {
    r->up->down = r;
    r->down->up = r;
    node* traverse = r->right;
    do {
        traverse->up->down = traverse;
        traverse->down->up = traverse;
        traverse = traverse->right;
    }while(traverse != r->right);
}