#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_LINE_LENGTH 25

typedef struct enroll_s { 
    int id_num;
    int course_num;
    struct enroll_s *left;
    struct enroll_s *right;
} *enroll_sp;

enroll_sp courses[35];
enroll_sp students[500];

// Add this function to print the courses for each student in ascending order of ID number
void print_course_list() {
    for (int i = 0; i < 500; i++) {
        enroll_sp student = students[i];
        if (student != NULL) {
            printf("%d: ", i);
            enroll_sp curr = student->right;
            while (curr != NULL) {
                printf("%d ", curr->course_num);
                curr = curr->right;
            }
            printf("\n");
        }
    }
}

int main() {
    char line[MAX_LINE_LENGTH];
    FILE *fp = fopen("input.txt", "r"); // replace "input.txt" with your input file name
    if (fp == NULL) {
        printf("Error opening file\n");
        return 1;
    }

    while (fgets(line, MAX_LINE_LENGTH, fp) != NULL) {
        int id, course;
        char action;
        sscanf(line, "%10d%10d %c", &id, &course, &action);
        if (id < 0 || id >= 500 || course < 0 || course >= 35) {
            printf("Error: Invalid input\n");
            continue;
        }

        enroll_sp student = students[id];
        if (student == NULL) {
            student = malloc(sizeof(struct enroll_s));
            student->id_num = id;
            student->left = NULL;
            student->right = NULL;
            students[id] = student;
        }

        enroll_sp course_node = courses[course];
        if (course_node == NULL) {
            course_node = malloc(sizeof(struct enroll_s));
            course_node->course_num = course;
            course_node->left = NULL;
            course_node->right = NULL;
            courses[course] = course_node;
        }

        if (action == 'A') {
            enroll_sp prev = NULL;
            enroll_sp curr = student;
            while (curr != NULL) {
                prev = curr;
                curr = curr->right;
            }
            prev->right = course_node;
            prev = NULL;
            curr = course_node;
            while (curr != NULL) {
                prev = curr;
                curr = curr->left;
            }
            prev->left = student;
        } 

        else if (action == 'D') {
            enroll_sp student = students[id];
            enroll_sp course_node = courses[course];
            if (student == NULL || course_node == NULL) {
                printf("%d %d D ** not enrolled **\n", id, course);
            } else {
                enroll_sp prev = NULL;
                enroll_sp curr = student;
                while (curr != NULL && curr != course_node) {
                    prev = curr;
                    curr = curr->right;
                }
                if (curr == course_node) {
                    if (prev == NULL) {
                        student = curr->right;
                    } else {
                        prev->right = curr->right;
                    }
                    prev = NULL;
                    curr = course_node;
                    while (curr != NULL && curr != student) {
                        prev = curr;
                        curr = curr->left;
                    }
                    if (curr == student) {
                        if (prev == NULL) {
                            course_node = curr->left;
                        } else {
                            prev->left = curr->left;
                        }
                    }
                    free(curr);
                } else {
                    printf("%d %d D ** not enrolled **\n", id, course);
                }
            }
        } 
        
        else {
            printf("Error: Invalid input\n");
}   
    }
    print_course_list();
    fclose(fp);
    return 0;
}
