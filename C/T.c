#include <stdio.h>
#include <stdlib.h>

#define MAX_STACK_SIZE 100
#define MAX_EXPR_SIZE 100


int stack[MAX_STACK_SIZE]; // global stack
char expr[MAX_EXPR_SIZE]; // input string


// 구조체 Push
void Push(int *top, int item) {
    if (*top >= MAX_STACK_SIZE - 1) { //check if the stack overflowed
        printf("Stack Overflow");
        return;
    }
    stack[++(*top)] = item;
}

// Element를 타입으로 가지는 구조체 Pop
int Pop(int *top) {
    if (*top == -1) { //남아있는 값이 있는지 확인
        printf("Stack Underflow\n");
        exit(1);
    }
    return stack[(*top)--];
}


typedef enum {
    lparen, rparen, plus, minus, times, divide, mod, eos, operand
} Precedence;


Precedence Get_token(char *psymbol, int *pn) {
    *psymbol = expr[(*pn)++];
    switch (*psymbol) {
        case '(' : return lparen;
        case ')' : return rparen;
        case '+' : return plus;
        case '-' : return minus;
        case '*' : return times;
        case '/' : return divide;
        case '%' : return mod;
        case ' ' : return eos;
        default: return operand;
        /* no error checking */
    }
}


int Eval() {
    Precedence token; // enum token;
    char symbol;
    int op1, op2; 
    int n = 0; 
    int top = -1;
    token = Get_token(&symbol, &n);

    while (token != eos) {
        if (token == operand) {
            Push(&top, symbol-'0'); // change to number
        } else {
            op2 = Pop(&top);
            op1 = Pop(&top);

            switch (token) {
                case plus:
                    Push(&top, op1+op2);
                    break;
                case minus:
                    Push(&top, op1-op2);
                    break;
                case times:
                    Push(&top, op1*op2);
                    break;
                case divide:
                    Push(&top, op1/op2);
                    break;
                case mod:
                    Push(&top, op1%op2);
            }
        }
        token = Get_token(&symbol, &n);
    }
    return Pop(&top); //pop the evaluated value
}

int main() {
    printf("Enter an expression: ");
    fgets(expr, MAX_EXPR_SIZE, stdin);

    int result = Eval();

    printf("Result: %d\n", result);

    return 0;
}
