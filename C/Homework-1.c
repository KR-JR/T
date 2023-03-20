#include <stdio.h>
#define MAX_TERMS 101
#define MAX_COL 8

typedef struct {
    int col;
    int row;
    int value;

} term;

term a [MAX_TERMS];

void transpose(term a[], term b[]) {

    int n, i, j, currentb;
    n = a[0].value;
    b[0].row = a[0].col;
    b[0].col = a[0].row;
    b[0].value = n;
    
    if( n > 0 ) {
        currentb = 1;
        for(i = 0; i < a[0].col; i++)
        for(j = 1; j <= n; j++)
        if(a[j].col == i) {
            b[currentb].row = a[j].col;
            b[currentb].col = a[j].row;
            b[currentb].value = a[j].value;
            currentb++;
        }

    } 


}

void FAST_TRANS(term a[],term b[]) {
    int row_terms[MAX_COL];
    int starting_pos[MAX_COL];
    int i,j;
    int num_cols = b[0].row = a[0].col;
    int num_terms = b[0].value = a[0].value;
    b[0].col = a[0].row;

    if(num_terms > 0) {
        for(i = 0; i < num_cols; i++) //init
            row_terms[i] = 0;
        for(i = 1; i <= num_terms; i++)
            row_terms[a[i].col]++;
        starting_pos[0] = 1;
        for(i = 1; i < num_cols; i++)
            starting_pos[i] =
            starting_pos[i-1] + row_terms[i-1];

            for(i = 1; i <= num_terms; i++) {
                j=starting_pos[a[i].col]++;
                b[j].row = a[i].col;
                b[j].col = a[i].row;
                b[j].value = a[i].value;
            }
    }
}

int main() {
    term a[MAX_TERMS], b[MAX_TERMS];
    int i, j, m, n, count = 1;

    printf("Enter the number of rows and colums: ");
    scanf("%d%d", &m, &n);
}
