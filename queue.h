#define N 9
#define EMPTY 0

typedef struct node{
    int d;
    struct node *next;
}node;

typedef struct queue{
    node *head, *tail;
}queue;

void qinit(queue *q);
int qfull(queue *q);
int qempty(queue *q);
void enq(queue *q, int val);
int deq(queue *q);