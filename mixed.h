#define N 9
#define EMPTY 0

void take_input(stack *u, stack *r, int sudoku[N][N], int skeleton[N][N], int row, int col, int num);
void undo(stack *u, stack *r, int sudoku[N][N]);
void redo(stack *u, stack *r, int sudoku[N][N]);