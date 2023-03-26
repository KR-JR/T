#include <stdio.h>
#define MAX_STACK_SIZE 100
#define max_row 10
#define max_col 10



typedef struct {
    short int row;
    short int col;
    short int dir;
} Element;


typedef struct {
    Element elements[MAX_STACK_SIZE];
    int top;
} Stack;


// maze sample
int maze[max_row][max_col] = { 
    {0,0,0,0,0,1},
    {1,1,1,1,1,0},
    {1,0,0,0,0,1},
    {0,1,1,1,1,1},
    {1,0,0,0,0,1},
    {1,1,1,1,1,0},
    {1,0,0,0,0,1},
    {0,1,1,1,1,1},
    {1,0,0,0,0,0},
}


Stack CreateS() {
    Stack A;
    A.top = -1;
    return A;
}



void Push(Element item, Stack *S) {
    if (S->top >= MAX_STACK_SIZE - 1) { //스택이 넘쳤는지 확인
        printf("Stack Overflow")
        return;
    }
    S->elements[++S->top] = item;
}



Element Pop(Stack *S) {
    if (S->top == -1) //남아있는 값이 있는지 확인
        printf("Stack Underflow")
        return;
    return S->elements[S->top--];
}



typedef struct {
    short int vert;
    short int horiz;
} offsets;


//시계 방향으로 각각의 위치 할당
offsets move[8] = { {-1, 0}, {-1, 1}, {0, 1}, {1, 1}, {1, 0}, {1, -1}, {0, -1}, {-1, -1} }; 

void Path() {
    int row, col, next_row, next_col, dir;.
    Element position;
    dir = 0;
    push(position);
    while(top > -1) {

    }
}
