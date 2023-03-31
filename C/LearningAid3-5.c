#include <stdio.h>
#include <stdlib.h>

#define MAX_QUEUE_SIZE 5

typedef struct {
    int key;
    char value[20]; 
} Element;

Element queue[MAX_QUEUE_SIZE]; //queue를 MAX_QUEUE_SIZE 크기로 할당

typedef struct {
    Element items[MAX_QUEUE_SIZE];
    int rear;
    int front;
} C_Queue;

C_Queue CreateQ() { //Queue생성 및 rear, front = -1로 초기화
    C_Queue C;
    C.rear = -1;
    C.front = -1;
    return C;
}

void Add(Element item, C_Queue *C) { //Add 함수 생성
    C->rear = (C->rear + 1) % MAX_QUEUE_SIZE;
    if (C->front == C->rear) {
        printf("Queue is full");
        return;
    }
    C->items[C->rear] = item; 
}

Element Delete(C_Queue *C) { //Delete함수 생성, 인자:Element, 
    Element item;
    if (C->front == C->rear) {
        printf("Queue is empty"); 
        item.key = -1; 
        return item;
    }
    C->front = (C->front + 1) % MAX_QUEUE_SIZE;
    item = C->items[C->front]; 
    return item;
}

int main() {
    C_Queue q = CreateQ(); //q 정의 인자 C_Queue
    Element item1 = {1, "stack"}; //item에 1~5까지 순서대로 삽입
    Element item2 = {2, "queue"};
    Element item3 = {3, "array"};
    Element item4 = {4, "struct"};
    Element item5 = {5, "linked list"};
    Add(item1, &q);
    Add(item2, &q);
    Add(item3, &q);
    Add(item4, &q);
    Add(item5, &q);
    printf("Added 5 items.\n"); //5개의 item Add 후 확인 출력

    Element item = Delete(&q); 
    printf("%d %s\n", item.key, item.value); //첫번째 스택 출력
    item = Delete(&q); //두번째 스택 Delete
    printf("%d %s\n", item.key, item.value); //두번째 스택 출력

    return 0;
}
