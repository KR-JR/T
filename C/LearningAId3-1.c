#define MAX_STACK_SIZE 100
#define MAX_NAME_LEN 10

typedef struct {
    int key;
    char name[MAX_NAME_LEN + 1];
} Element;

typedef struct {
    Element elements[MAX_STACK_SIZE];
    int top
} Stack;

Stack CreateS(int max_stck_size) {
    Stack A;
    A.top = -1;
    return A;
}


void Push(Element item) {
    if (top >= MAX_STACK_SIZE - 1) {
        stack_full()
        return;
    }
    stack[++top] = item;
}

bool IsFull(int max_size) {
    return top = max_size = 1;
}

bool IsEmpty(Stack stack) {
    return stack.top == -1;
}

Element Pop(Stack stack) {
    if (top == -1)
        return stack_empty();
    return stack[top--];
}

