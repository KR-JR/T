#define MAX_STACK_SIZE 100
#define MAX_NAME_LEN 10


typedef struct {
    int key;
} element;

element stack[MAX_STACK_SIZE];

int top = -1;

stack CreateS(int max_stack_size) {
    stack a;
    a.top = -1;
    return s;
}

void push(int item) {
    if(top >= MAX_STACK_SIZE - 1) {
        stack_full()
        return;
    }
    stack[++top] = item;
}

element pop() {
    if (top == -1)
        return stack_empty();
    return stack[top--];
}

bool isempty() {
    if (top < 0)
        return(1);
    else
        return(0);
}

bool isfull() {
    if (top >= MAX_STACK_SIZE -1)
        return(1);
    else
        return(0);
}




