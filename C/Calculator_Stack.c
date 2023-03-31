#define MAX_STACK_SIZE 100
#define MAX_EXPR_SIZE 100




void push(int *top, Element item) {
    if (*top >= MAX_STACK_SIZE - 1) { //스택이 넘쳤는지 확인
        printf("Stack Overflow");
        return;
    }
    stack[++(*top)] = item;
}



Element pop(int *top) {
    if (*top == -1) {//남아있는 값이 있는지 확인
        printf("Stack Underflow");
    }
    Element item = stack[(*top)--];
    return item;
}



typedef enum {
    lparen, rparen, plus, minus, times, divide, mod, eos, operand
} Precedence;

int stack[MAX_STACK_SIZE]; /* global stack */
char expr[MAX_EXPR_SIZE]; /* input string */


int eval() {
    Precedence token; // enum token;
    char symbol; 
    int op1, op2; int n = 0; int top = -1;
    token = get_token(&symbol, &n);

    while (token != eos)
        if (token == operand)
            push(&top, symbol-’0’); // 숫자로 변경
        else {
        op2 = pop(&top);
        op1 = pop(&top);

            switch (token) {
                case plus: 
                    push(&top, op1+op2); 
                    break;
                case minus: 
                    push(&top, op1-op2); 
                    break;
                case times: 
                    push(&top, op1*op2); 
                    break;
                case divide: 
                    push(&top, op1/op2); 
                    break;
                case mod: 
                    push(&top, op1%op2);
                }
            }
            token = get_token(&symbol, &n);
        }
        return pop(&top); //pop the evaluated value