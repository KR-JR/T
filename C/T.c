#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct enroll_s{
     int id_num;
     int course_num;
     struct enroll_s* left;
     struct enroll_s* right;
} *enroll_sp;

enroll_sp courses[35];
enroll_sp students[500];

void reset() {
    for (int i = 0; i < 35; i++) {
        courses[i] = NULL;
    }
    for (int i = 0; i < 500; i++) {
        students[i] = NULL;
    }
}

void insert(enroll_sp* head, enroll_sp node) {
    enroll_sp c = *head;
    enroll_sp p = NULL;
    while (c != NULL && c->id_num < node->id_num) {
        p = c;
        c = c->right;
    }

    if (p == NULL) {
        node->right = *head;
        *head = node;
    }

    else {
        p->right = node;
        node->left = p;
        node->right = c;
        if (c != NULL) {
            c->left = node;
        }

    }
}

void delete(enroll_sp* head, enroll_sp node) {
    if (*head == NULL) {
        printf("Error: list is empty\n");
        return;
    }

    enroll_sp c = *head;
    enroll_sp p = NULL;
    while (c != NULL && c != node) {
        p = c;
        c = c->right;
    }
    if (c == NULL) {
        printf("Error : Node not found.\n");
        return;
    }

    if (p == NULL) {
        *head = c->right;
        if (*head != NULL) {
            (*head)->left = NULL;
        }
    }
    else {
        p->right = c->right;
        if (c->right != NULL) {
            c->right->left = p;
        }
    }
    free(c);
}

void print_course_list(int course_num) {
    enroll_sp c = courses[course_num - 100];
    printf("%d: ", course_num);
    while (c != NULL) {
        printf("%d ", c->id_num);
        c = c->right;
    }
    printf("\n");
}

void print_student_list(int id_num) {
    enroll_sp curr = students[id_num - 2000];
    printf("%d: ", id_num);
    while (curr != NULL) {
        printf("%d ", curr->course_num);
        curr = curr->right;
    }
    printf("\n");
}

void enroll_student(int id_num, int course_num) {
    enroll_sp node = (enroll_sp)malloc(sizeof(struct enroll_s));
    node->id_num = id_num;
    node->course_num = course_num;
   
    insert(&courses[course_num - 100], node);

    insert(&students[id_num - 2000], node);
}



int main() {
    reset();
   
    enroll_student(2002, 107);
    enroll_student(2001, 102);
    enroll_student(2002, 101);
    enroll_student(2003, 109);
    enroll_student(2005, 101);
    enroll_student(2004, 108);
    enroll_student(2017, 103);
    enroll_student(2034, 104);
    enroll_student(2008, 105);
    enroll_student(2005, 106);


   
    print_course_list(101);
    print_course_list(102);
    print_course_list(103);
    print_course_list(104);
    print_student_list(2001);
    print_student_list(2002);
    print_student_list(2017);
    print_student_list(2034);

    print_course_list(101);
    print_student_list(2004);

    return 0;

} 