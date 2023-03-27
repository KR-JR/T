#include <stdio.h>
#define MAX_STACK_SIZE 100
#define max_row 10
#define max_col 10
#define exit_row 6
#define exit_col 9
#define True 1
#define False 0



typedef struct {
    short int row;
    short int col;
    short int dir;
} Element;


typedef struct {
    Element stack[MAX_STACK_SIZE];
    int top;
} Stack;


/* maze sample */
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
};


Stack CreateS() {
    Stack A;
    A.top = -1;
    return A;
}



void Add(Element *item, Stack *S) {
    if (S->top >= MAX_STACK_SIZE - 1) { //스택이 넘쳤는지 확인
        printf("Stack Overflow");
        return;
    }
    S->stack[++S->top] = *item;
}



Element Delete(Stack *S) {
    if (S->top == -1) //남아있는 값이 있는지 확인
        printf("Stack Underflow");
    return S->stack[S->top--];
}








typedef struct {
    short int vert;
    short int horiz;
} offsets;


//시계 방향으로 각각의 위치 할당
offsets move[8] = { {-1, 0}, {-1, 1}, {0, 1}, {1, 1}, {1, 0}, {1, -1}, {0, -1}, {-1, -1} }; 

void Path(Stack *S) {
//미로 이동하는 경로가 있으면 출력
        int i, row, col, next_row, next_col, dir, mark[max_row][max_col] , found = False;
        Element position;
        mark[1][1] = 1; S->top = 0;
        S->stack[0].row = 1;
        S->stack[0].col = 1;
        S->stack[0].dir = 1;
        
        while(S->top > -1 && !found) {
            position = Delete(&S->top);
            row = position.row; col=position.col;
            dir = position.dir;

            while(dir < 8 && !found) {
                // dir으로 이동
                next_row = row + move[dir].vert;
                next_col = col + move[dir].horiz;
                if(next_row == exit_row && next_col == exit_col)
                        found = True;
                else if(!maze[next_row][next_col] && !mark[next_row][next_col]) {
                    mark[next_row][next_col] = 1;
                    position.row = row; position.col = col;
                    position.dir = ++dir;
                    Add(&position, S->top);
                    row = next_row; col = next_col; dir = 0;
                }
                        else ++dir;
            }
        }
        
        if(found) {
                printf("경로는 \n");
                printf("row col\n");
                for(i = 0; i <= S->top; i++)
                        printf("%2d%5d", S->stack[i].row, S->stack[i].col);
                printf("%2d%5d\n", row, col);
                printf("%2d%5d\n", exit_row, exit_col);
        }
        else printf("경로가 없음!\n");
}

int main() {
    Path();
    return 0;
}