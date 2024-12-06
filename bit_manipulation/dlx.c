#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define N 9
#define SUBGRID 3
#define MAX_NODES (N * N * N * 4)

// Node structure for the Dancing Links
typedef struct Node {
    struct Node *left, *right, *up, *down;
    struct Column *col;
    int rowID;
} Node;

// Column structure to track columns in the Dancing Links
typedef struct Column {
    Node node;
    int size;
    char name[4];
} Column;

// Root node of the Dancing Links matrix
Node root;
Column columns[N * N * 4];
Node nodes[MAX_NODES];
int node_count = 0;

// Solution array
int solution[N * N];

// Initialize a column
void init_column(Column *col, const char *name) {
    strcpy(col->name, name);
    col->node.up = &col->node;
    col->node.down = &col->node;
    col->size = 0;
    col->node.col = col;
}

// Link a node vertically into a column
void link_vertical(Node *node, Column *col) {
    node->up = col->node.up;
    node->down = &col->node;
    node->col = col;

    col->node.up->down = node;
    col->node.up = node;
    col->size++;
}

// Link a node horizontally
void link_horizontal(Node *node, Node *row) {
    node->left = row->left;
    node->right = row;

    row->left->right = node;
    row->left = node;
}

// Cover a column
void cover(Column *col) {
    col->node.right->left = col->node.left;
    col->node.left->right = col->node.right;

    for (Node *row = col->node.down; row != &col->node; row = row->down) {
        for (Node *node = row->right; node != row; node = node->right) {
            node->down->up = node->up;
            node->up->down = node->down;
            node->col->size--;
        }
    }
}

// Uncover a column
void uncover(Column *col) {
    for (Node *row = col->node.up; row != &col->node; row = row->up) {
        for (Node *node = row->left; node != row; node = node->left) {
            node->col->size++;
            node->down->up = node;
            node->up->down = node;
        }
    }

    col->node.right->left = &col->node;
    col->node.left->right = &col->node;
}

// Choose a column with the smallest size (heuristic for efficiency)
Column *choose_column() {
    Column *best = NULL;
    int min_size = MAX_NODES;

    for (Column *col = (Column *)root.right; col != (Column *)&root; col = (Column *)col->node.right) {
        if (col->size < min_size) {
            best = col;
            min_size = col->size;
        }
    }
    return best;
}

// Solve using Dancing Links and backtracking
int solve(int k) {
    if (root.right == &root) {
        return 1; // Solution found
    }

    Column *col = choose_column();
    if (!col) return 0;

    cover(col);

    for (Node *row = col->node.down; row != &col->node; row = row->down) {
        solution[k] = row->rowID;

        for (Node *node = row->right; node != row; node = node->right) {
            cover(node->col);
        }

        if (solve(k + 1)) return 1;

        for (Node *node = row->left; node != row; node = node->left) {
            uncover(node->col);
        }
    }

    uncover(col);
    return 0;
}

// Add a row to the Dancing Links structure
void add_row(int rowID, int *columns, int count) {
    Node *row = &nodes[node_count++];
    row->rowID = rowID;

    for (int i = 0; i < count; i++) {
        Node *node = &nodes[node_count++];
        link_vertical(node, &columns[columns[i]]); // Correct use of indices

        if (i == 0) {
            row->left = node;
            node->left = node->right = row;
        } else {
            link_horizontal(node, row);
        }
    }
}


// Parse the Sudoku puzzle and setup the Dancing Links matrix
void setup_dlx(int grid[N][N]) {
    // Initialize root and columns
    root.right = (Node *)&columns[0];
    root.left = (Node *)&columns[N * N * 4 - 1];
    columns[0].node.left = (Node *)&root;
    columns[N * N * 4 - 1].node.right = (Node *)&root;

    for (int i = 0; i < N * N * 4; i++) {
        init_column(&columns[i], "");
    }

    // Add rows for each possible value in each cell
    for (int r = 0; r < N; r++) {
        for (int c = 0; c < N; c++) {
            if (grid[r][c] == 0) {
                for (int v = 1; v <= N; v++) {
                    int rowID = (r * N * N) + (c * N) + (v - 1);
                    int subgrid_index = (r / SUBGRID) * SUBGRID + (c / SUBGRID);
                    int cols[4] = {
                        r * N + c,                       // Cell constraint
                        N * N + r * N + (v - 1),         // Row constraint
                        2 * N * N + c * N + (v - 1),     // Column constraint
                        3 * N * N + subgrid_index * N + (v - 1) // Subgrid constraint
                    };
                }
            } else {
                int v = grid[r][c];
                int rowID = (r * N * N) + (c * N) + (v - 1);
                int cols[4] = {
                    r * N + c,
                    N * N + r * N + (v - 1),
                    2 * N * N + c * N + (v - 1),
                    3 * N * N + ((r / SUBGRID) * SUBGRID + (c / SUBGRID)) * N + (v - 1)
                };
                add_row(rowID, cols, 4);
            }
        }
    }
}

// Print the solution grid
void print_solution() {
    int grid[N][N] = {0};
    for (int i = 0; i < N * N; i++) {
        int rowID = solution[i];
        int r = rowID / (N * N);
        int c = (rowID / N) % N;
        int v = rowID % N + 1;
        grid[r][c] = v;
    }

    for (int r = 0; r < N; r++) {
        for (int c = 0; c < N; c++) {
            printf("%d ", grid[r][c]);
        }
        printf("\n");
    }
}

int main() {
    // Example Sudoku grid (0 represents empty cells)
    int grid[N][N] = {
        {5, 3, 0, 0, 7, 0, 0, 0, 0},
        {6, 0, 0, 1, 9, 5, 0, 0, 0},
        {0, 9, 8, 0, 0, 0, 0, 6, 0},
        {8, 0, 0, 0, 6, 0, 0, 0, 3},
        {4, 0, 0, 8, 0, 3, 0, 0, 1},
        {7, 0, 0, 0, 2, 0, 0, 0, 6},
        {0, 6, 0, 0, 0, 0, 2, 8, 0},
        {0, 0, 0, 4, 1, 9, 0, 0, 5},
        {0, 0, 0, 0, 8, 0, 0, 7, 9}
    };

    // Setup the Dancing Links structure
    setup_dlx(grid);

    // Solve the Sudoku puzzle
    if (solve(0)) {
        printf("Sudoku solved successfully:\n");
        print_solution();
    } else {
        printf("No solution exists for the given Sudoku puzzle.\n");
    }

    return 0;
}
