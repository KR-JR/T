#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct node {
    char component_number[7];
    char component_name[20];
    int stock_amount;
    int reorder_level;
    struct node *left;
    struct node *right;
} Node;

Node *root = NULL; // BST의 루트 노드

// 새로운 노드를 생성하는 함수
Node *createNode(char component_number[], char component_name[], int stock_amount, int reorder_level) {
    Node *newNode = (Node *)malloc(sizeof(Node));
    strcpy(newNode->component_number, component_number);
    strcpy(newNode->component_name, component_name);
    newNode->stock_amount = stock_amount;
    newNode->reorder_level = reorder_level;
    newNode->left = newNode->right = NULL;
    return newNode;
}

// 새로운 노드를 BST에 추가하는 함수
Node *insert(Node *root, char component_number[], char component_name[], int stock_amount, int reorder_level) {
    if (root == NULL) {
        return createNode(component_number, component_name, stock_amount, reorder_level);
    }
    if (strcmp(component_number, root->component_number) < 0) {
        root->left = insert(root->left, component_number, component_name, stock_amount, reorder_level);
    } else if (strcmp(component_number, root->component_number) > 0) {
        root->right = insert(root->right, component_number, component_name, stock_amount, reorder_level);
    } else { // 중복된 구성 요소 번호
        printf("Duplicate component number: %s\n", component_number);
    }
    return root;
}

// 주어진 BST에서 가장 작은 값을 가지는 노드를 반환하는 함수
Node *getMin(Node *node) {
    Node *current = node;
    while (current != NULL && current->left != NULL) {
        current = current->left;
    }
    return current;
}

// BST에서 구성 요소 번호에 해당하는 노드를 삭제하는 함수
Node *Delete(Node *root, char component_number[]) {
    if (root == NULL) {
        return root;
    }
    if (strcmp(component_number, root->component_number) < 0) {
        root->left = Delete(root->left, component_number);
    } else if (strcmp(component_number, root->component_number) > 0) {
        root->right = Delete(root->right, component_number);
    } else {
        if (root->left == NULL) {
            Node *temp = root->right;
            free(root);
            return temp;
        } else if (root->right == NULL) {
            Node *temp = root->left;
            free(root);
            return temp;
        }
        Node *temp = getMin(root->right);
        strcpy(root->component_number, temp->component_number);
        strcpy(root->component_name, temp->component_name);
        root->stock_amount = temp->stock_amount;
        root->reorder_level = temp->reorder_level;
        root->right = Delete(root->right, temp->component_number);
    }
    return root;
}


// 구성 요소 번호로 노드를 검색하는 함수
Node *search(Node *root, char component_number[]) {
    if (root == NULL || strcmp(component_number, root->component_number) == 0) {
        return root;
    }
    if (strcmp(component_number, root->component_number) < 0) {
        return search(root->left, component_number);
    } else {
        return search(root->right, component_number);
    }   
}

// 중위 순회를 이용하여 이진 탐색 트리를 출력하는 함수
void printInOrder(Node *root, int reorder_level) {
    if (root != NULL) {
        printInOrder(root->left, reorder_level);
        printf("%s %s %d %d", root->component_number, root->component_name, root->stock_amount, root->reorder_level);
        if (root->stock_amount < reorder_level) {
            printf(" *reorder required*");
        }
        printf("\n");
        printInOrder(root->right, reorder_level);
    }
}

int main() {
    char command[2];
    char component_number[7];
    char component_name[20];
    int amount;
    int reorder_level;

    FILE *fp = fopen("inventory.txt", "r"); // 파일을 읽기 모드로 열기
    if (fp == NULL) {
        printf("Error opening file.\n");
        return 1;
    }

    while (fscanf(fp, "%2s%6s%19s%d%5d", command, component_number, component_name, &amount, &reorder_level) == 5) {
    Node *node = search(root, component_number);
        if (command[0] == 'A') { // Add New Component
            if (node == NULL) {
                root = insert(root, component_number, component_name, amount, reorder_level);
            } else {
                printf("Duplicate component number: %s\n", component_number);
            }
        } else if (command[0] == 'D') { // Delete New Component
            if (node == NULL) {
                printf("No such component: %s\n", component_number);
            } else {
                root = Delete(root, component_number);
            }
        } else if (command[0] == 'I') { // Increase Inventory
            if (node == NULL) {
                printf("No such component: %s\n", component_number);
            } else {
                node->stock_amount += amount;
            }
        } else if (command[0] == 'R') { // Reduce Inventory
            if (node == NULL) {
                printf("No such component: %s\n", component_number);
            } else if (amount > node->stock_amount) {
                printf("Insufficient stock for component: %s\n", component_number);
            } else {
                node->stock_amount -= amount;
            }
        } else if (command[0] == 'P') { // Print Inventory
            printInOrder(root, reorder_level);
        } else {
            printf("Invalid command: %s\n", command);
        }
    }

    fclose(fp); // 파일 닫기

    return 0;
}