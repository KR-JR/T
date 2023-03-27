#include <stdio.h>
#define MAX_STACK_SIZE 100
#define max_row 11
#define max_col 8
#define exit_row 9
#define exit_col 6
#define True 1
#define False 0



typedef struct {
    short int row;
    short int col;
    short int dir;
} Element;

int top;

Element stack[MAX_STACK_SIZE];



/* maze sample */
int maze[max_row][max_col] = { 
    {1,1,1,1,1,1,1,1},
    {1,0,0,0,0,0,1,1},
    {1,1,1,1,1,1,0,1},
    {1,1,0,0,0,0,1,1},
    {1,0,1,1,1,1,1,1},
    {1,1,0,0,0,0,1,1},
    {1,1,1,1,1,1,0,1},
    {1,1,0,0,0,0,1,1},
    {1,0,1,1,1,1,1,1},
    {1,1,0,0,0,0,0,1},
    {1,1,1,1,1,1,1,1}
};



void Add(int *top, Element item) {
    if (*top >= MAX_STACK_SIZE - 1) { //스택이 넘쳤는지 확인
        printf("Stack Overflow");
        return;
    }
    stack[++(*top)] = item;
}



Element Delete(int *top) {
    if (*top == -1) {//남아있는 값이 있는지 확인
        printf("Stack Underflow");
    }
    Element item = stack[(*top)--];
    return item;
}



typedef struct {
    short int vert;
    short int horiz;
} offsets;



//시계 방향으로 각각의 위치 할당
offsets move[8] = { {-1, 0}, {-1, 1}, {0, 1}, {1, 1}, {1, 0}, {1, -1}, {0, -1}, {-1, -1} }; 

void Path(void) {
//미로 이동하는 경로가 있으면 출력
        int i, row, col, next_row, next_col, dir, mark[max_row][max_col] = {0}, found = False;
        Element position;
        mark[1][1] = 1; //시작지점
        top = 0; //시작점 스택Add
        stack[0].row = 1;
        stack[0].col = 1;
        stack[0].dir = 1;
        
        while(top > -1 && !found) {
            position = Delete(&top);
            row = position.row; col=position.col;
            dir = position.dir;

            while(dir < 8 && !found) { //주변 8방향 확인
                // dir으로 이동
                next_row = row + move[dir].vert;
                next_col = col + move[dir].horiz;
                if(next_row == exit_row && next_col == exit_col)
                    found = True; //탈출
                else if(!maze[next_row][next_col] && !mark[next_row][next_col]) {
                    mark[next_row][next_col] = True;
                    position.row = row; position.col = col;
                    position.dir = ++dir;
                    Add(&top, position);
                    row = next_row; col = next_col; dir = 0;
                }
                else ++dir;
            }
        }
        
    if(found) {
        printf("Path is \n");
        printf("row  col\n");
        for(i = 0; i <= top; i++)
            printf("%2d%5d\n", stack[i].row, stack[i].col);
            printf("%2d%5d\n", row, col);
            printf("exit row exit_col\n");
            printf("%d \t %d\n", exit_row, exit_col);
    }
    else printf("No Path!\n");
}

int main() {
    Path();
    return 0;
}