#include <stdio.h>

char *str = "HELLO";

void print_string() {
    int i = 0;
    char ch;
    printf("The given string is: ");
    while(ch = get_char(i)) {
        printf("%c", ch);
        i++;
    }
    printf("\n");
}

char get_char(int pos) {
    return str[pos];
}

int main() {
    print_string();
    return 0;
}
