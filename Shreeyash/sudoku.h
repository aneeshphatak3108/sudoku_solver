#define N 9
#define EMPTY 0

int valid_in_grid(int sudoku[N][N], int row, int col, int val);
int valid_in_row(int sudoku[N][N], int row, int val);
int valid_in_col(int sudoku[N][N], int col, int val);
int is_safe(int sudoku[N][N], int row, int col, int num);
int find_empty_cell(int sudoku[N][N], int row, int col);
int solve_sudoku(int sudoku[N][N]);
void print_sudoku(int sudoku[N][N], int skeleton[N][N]);
void copy_sudoku(int sour[N][N], int dest[N][N]);
void generate_skeleton_sudoku(int sudoku[N][N], int skeleton[N][N]);
int count_row_elements(int sudoku[N][N], int row);
int count_col_elements(int sudoku[N][N], int col);
int count_grid_elements(int sudoku[N][N], int row, int col);
void hint(int sudoku[N][N]);
void remove_cells(int sudoku[N][N], int cells_to_remove);
void fill_diagonal(int sudoku[N][N]);
void shuffle(int arr[], int n);
void generate_sudoku(int sudoku[N][N], int difficulty);