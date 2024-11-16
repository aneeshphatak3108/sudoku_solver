#define N 9
#define EMPTY 0

typedef struct data{
    int num, row, col;
}data;

typedef struct stacknode{
    data d;
    struct stacknode *next;
}stacknode;

typedef struct stack{
    stacknode *top;
}stack;

void sinit(stack *s);
int sEmpty(stack *s);
void push(stack *s, data prev_step);
data pop(stack *s);
data peek(stack *s);
void freestack(stack *s);