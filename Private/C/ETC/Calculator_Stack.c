#include <stdio.h>
#include <string.h>
#define MAX_STACK_SIZE 100 
#define MAX_EXPR_SIZE 60



typedef enum {
	lparen, rparen, plus, minus, times, divide, mod, eos, operand
}precedence;


int isp[] = { 0, 19, 12, 12, 13, 13, 13, 0 };
int icp[] = { 20, 19, 12, 12, 13, 13, 13, 0 };


char expr[MAX_EXPR_SIZE];
int top = 0;

precedence stack[MAX_STACK_SIZE];


void push(precedence item) {
	if (top >= MAX_STACK_SIZE - 1) printf("stack is full\n");
	else stack[++top] = item;
}


precedence pop() {
	if (top == -1) printf("stack is empty");
	return stack[top--];
}


void printToken(precedence token) {
	switch(token){
	case lparen: printf("("); break;
	case rparen: printf(")"); break;
	case plus: printf("+"); break;
	case minus: printf("-"); break;
	case divide: printf("/"); break;
	case times: printf("*"); break;
	case mod: printf("%%"); break;
	case eos: printf("\0"); break;
	}

}

precedence getToken(char *symbol, int *pn) {

	switch (*(symbol + *pn)) {
	case '(': return lparen;
	case ')': return rparen;
	case '+': return plus;
	case '-': return minus;
	case '/': return divide;
	case '*': return times;
	case '%': return mod;
	case '\0': return eos;
	default: return operand; 
	}
}

void infix_to_postfix(void) { 
	int n = 0;
	precedence token;

	stack[0] = eos;
	for (token = getToken(expr, &n); token != eos; token = getToken(expr, &n)) {
		if (token == operand)
			printf("%c", expr[n]);
		else if (token == rparen) { 
			while (stack[top] != lparen)
				printToken(pop());
			pop();
		}
		else { 
			while (isp[stack[top]] >= icp[token])
				printToken(pop()); 
			push(token);
		}
		n++;
	}
	while ((token = pop()) != eos)
		printToken(token);
	printf("\n");
}


int eval(char* symbol) { 
	precedence token;
	int op1, op2;
	int n = -1; 
	token = getToken(symbol, &n);

	while (token != eos) {
		if (token == operand)
			push(symbol[n] - '0'); 
		else { 
			op2 = pop(); 
			op1 = pop();
			switch (token) {
			case plus: push(op1 + op2); break;
			case minus: push(op1 - op2); break;
			case times: push(op1 *op2); break;
			case divide: push(op1 / op2); break;
			case mod: push(op1 % op2);
			}
		}
		token = getToken(symbol, &n);
	}				
	return pop(); /* return result */
}








int main() {
    printf("Enter an infix expression: ");
    scanf("%s", expr);
    
    infix_to_postfix();
    
    int result = eval(expr);
    printf("Result: %d\n", result);
    
    return 0;
}
