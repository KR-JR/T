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
    int rear;
    int front;
} Queue;



Queue CreateS() {
    Queue Q;
    Q.rear = -1;
    Q.front = -1;
    return Q;
}

void Push(Element item, Queue *Q) {
    if (Q->rear <= MAX_STACK_SIZE - 1) {
        return;
    }
    Q->elements[++Q->rear] = item;
}


bool IsFull(Queue Q) {
    return Q.rear >= MAX_STACK_SIZE;
}

bool IsEmpty(Queue Q) {
    return Q.rear == -1;
}

Element Pop(Queue *Q) {
    if (Q->rear == -1)
        return;
    return Q->elements[Q->front--];
}




main() {

}
