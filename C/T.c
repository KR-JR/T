#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_TERMS 250000
#define MAX_COL 500

typedef struct {
    int col;
    int row;
    int value;
} term;

term a [MAX_TERMS];
term b [MAX_TERMS];

void transpose(term a[], term b[]) {

    int n, i, j, currentb;
    n = a[0].value;
    b[0].row = a[0].col;
    b[0].col = a[0].row;
    b[0].value = n;
    
    if( n > 0 ) {
        currentb = 1;
        for(i = 0; i < a[0].col; i++) {
            for(j = 1; j <= n; j++) {
                if(a[j].col == i) {
                    b[currentb].row = a[j].col;
                    b[currentb].col = a[j].row;
                    b[currentb].value = a[j].value;
                    currentb++;
                }
            }
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
            starting_pos[i] = starting_pos[i-1] + row_terms[i-1];

        for(i = 1; i <= num_terms; i++) {
            j=starting_pos[a[i].col]++;
            b[j].row = a[i].col;
            b[j].col = a[i].row;
            b[j].value = a[i].value;
        }
    }
}

int main() {
    FILE *fp;
    int i, row, col, value, count = 1, n = 500;
    fp = fopen("input.txt", "r");
    if (fp == NULL) {
        printf("Failed to open input file.\n");
        return 1;
    }

    // Read input matrix from file
    for (i = 0; i < n * n; i++) {
        if (fscanf(fp, "%d %d %d", &row, &col, &value) != 3) {
            printf("Error reading input file.\n");
            fclose(fp);
            return 1;
        }
        if (value != 0) {
            a[count].row = row;
            a[count].col = col;
            a[count].value = value;
            count++;
        }
    }
    a[0].row = n;
    a[0].col = n;
    a[0].value = count - 1;

    fclose(fp);
    return 0;
}
