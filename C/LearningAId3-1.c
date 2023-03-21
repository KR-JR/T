#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

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



Stack CreateS() {
    Stack A;
    A.top = -1;
    return A;
}

void Push(Element item, Stack *S) {
    if (S->top <= MAX_STACK_SIZE - 1) {
        return;
    }
    S->elements[++S->top] = item;
}

bool IsFull(Stack S) {
    return S.top >= MAX_STACK_SIZE;
}

bool IsEmpty(Stack stack) {
    return stack.top == -1;
}

Element Pop(Stack *S) {
    if (S->top == -1)
        return;
    return S->elements[S->top--];
}



main() {

}
