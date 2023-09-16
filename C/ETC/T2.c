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
    // ...
    while (fgets(line, MAX_LINE_LENGTH, fp) != NULL) {
        // ...
    }
    print_course_list(); // call the function to print the course list
    // ...
}
