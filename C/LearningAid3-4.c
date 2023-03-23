#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_QUEUE_SIZE 100
#define MAX_NAME_LEN 10




typedef struct {
    int key;
    char name[MAX_NAME_LEN + 1];
} Element;

typedef struct {
    Element elements[MAX_QUEUE_SIZE];
    int rear;
    int front;
} C_Queue;



C_Queue CreateS() {
    C_Queue C;
    C.rear = 0;
    C.front = 0;
    return C;
}

void Add(Element item, C_Queue *C) {
    C->rear = (C->rear) % MAX_QUEUE_SIZE;
    if (C->front == C->rear) {
        printf("Error");
        return;
    }
    C->elements[C->rear] = item;
}

Element Delete(C_Queue *C) {
    if (C->front == C->rear)
        return;
    return C->front = (C->front + 1) % MAX_QUEUE_SIZE;

}


/* void Push(Element item, C_Queue *C) {
    if (C->rear <= MAX_QUEUE_SIZE - 2) {
        return;
    }
    C->elements[++C->rear] = item;
}


bool IsFull(C_Queue C) {
    return C.rear == C.front - 1;
}

bool IsEmpty(C_Queue C) {
    return C.rear == 0, C.front == 0;
}

Element Pop(C_Queue *C) {
    if (C->rear == -1)
        return;
    return C->elements[C->front--];
}

*/


main() {

}
