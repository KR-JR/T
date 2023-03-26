#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_QUEUE_SIZE 100
#define MAX_NAME_LEN 10




typedef struct {
    int key;
    char name[MAX_NAME_LEN + 1];
} Element;

Element queue[MAX_QUEUE_SIZE];

typedef struct {
    Element elements[MAX_QUEUE_SIZE];
    int rear;
    int front;
} C_Queue;



C_Queue CreateS() { //C Queue 지정
    C_Queue C;
    C.rear = 0;
    C.front = 0;
    return C;
}

void Add(Element item, C_Queue *C) {  //C Queue 추가
    C->rear = (C->rear) % MAX_QUEUE_SIZE;
    if (C->front == C->rear) { 
        printf("Error");
        return;
    }
    queue[C->rear] = item;
}

Element Delete(C_Queue *C) { //C Queue 삭제
    if (C->front == C->rear)
        return;
    (C->front = (C->front + 1) % MAX_QUEUE_SIZE);
    return queue[C->front];
}


main() {

}
