/**
 * @file    test.c
 * @author  null
 * @brief   전치 행렬 실습
 * @version null
 * @date    2023-03-22
 *
 * @copyright C00
 *
 */
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#pragma warning(disable : 4996) // scanf 보안 이슈 해제

#define MAX_TRANSPOSE 250000 // 데이터 저장 행렬 크기
#define MAX_COL 500          // 행 크기

/**
 * @brief 전치 행렬 구조체
 */
typedef struct
{
    int col;   // 행
    int row;   // 열
    int value; // 저장값
} transpose;

transpose a[MAX_TRANSPOSE]; // 행렬 a
transpose b[MAX_TRANSPOSE]; // 행렬 b

/**
 * @brief 전치 행렬 (심플)
 *
 * @param a 행렬 a
 * @param b 행렬 b
 */
void simpleTranspose(transpose a[], transpose b[]);

/**
 * @brief 전치 행렬 (빠름)
 *
 * @param a 행렬 a
 * @param b 행렬 b
 */
void fastTranspose(transpose a[], transpose b[]);

/**
 * @brief print error message
 *
 * @param message message info
 */
void error(char *message);

int main()
{

    int n = MAX_COL; // 요소 총 개수
    int i, j, count = 1;
    double simpleTransposeResult, fastTransposeResult; // 시간 측정 결과 저장 변수
    clock_t start, end;                                // 시간 측정 플래그

    int input;
    printf("file load: 1\t input: 2\n>> ");
    scanf("%d", &input);

    switch (input)
    {

    case 1: // 파일입출력 사용

        FILE *fp = fopen("data.txt", "r"); // 파일 포인터 fp에 data.txt 파일을 읽기모드로 호출
        if (fp == NULL)
            error("file not found"); // 파일 포인터가 비어있을 경우 에러 출력 후 종료

        int c = 0;
        while (!feof(fp)) { // 개조해서 데이터의 개수만큼 동적할당 가능
            fscanf(fp, "%d %d %d", &a[c].col, &a[c].row, &a[c].value);
            printf("%d %d %d\n", a[c].col, a[c].row, a[c].value); // 데이터 검증
            c++;
        }

        fclose(fp); // 파일 포인터 동적 할당 해제

        break;

    case 2: // 직접 입력 사용

        printf("input -1 in row or col: stop\n");
        printf("a[ ]  row\tcol\tvalue\n");
        for (int i = 0; i < MAX_COL; i++) {
            printf("a[%d]  ");
            scanf("%d %d %d", &a[c].col, &a[c].row, &a[c].value);
            if((a[c].col == -1) && (a[c].row == -1))
                break;
        }
        break;
    default: // 예외
        error("wrong answer");
        break;
    }

    // 심플 전치 행렬 시간 측정
    start = clock();

    for (i = 0; i < 1000; i++)
        simpleTranspose(a, b);

    end = clock();

    simpleTransposeResult = (double)(end - start) / CLOCKS_PER_SEC; // 초 단위로 변환

    // 빠른 전치 행렬 시간 측정
    start = clock();
    for(i = 0; i < 1000; i++) {
        fastTranspose(a, b);
    }
    end = clock();
    fastTransposeResult = (double)(end - start) / CLOCKS_PER_SEC; 

    printf("simple transpose spent time: %lf seconds\n", simpleTransposeResult);
    printf("fast transpose spent time: %lf seconds\n", fastTransposeResult);

    return 0;
}

void error(char *message)
{
    fprintf(stderr, "%s\n", message);
    exit(1);
}

/**
 *  이 이하 함수는 재검토 필요
 */

/* void simpleTranspose(transpose a[], transpose b[])
{

    int n = a[0].value; // 요소 총 개수
    b[0].row = a[0].col;
    b[0].col = a[0].row;
    b[0].value = n;

    if (n > 0) { // n 의 값이 0보다 클 경우

        int curr_pos = 1; // 현재 위치 초기화
        for (int i = 0; i < a[0].col; i++)
            for (int j = 1; j <= n; j++)
                if (a[j].col == i) {
                    b[curr_pos].row = a[j].col;
                    b[curr_pos].col = a[j].row;
                    b[curr_pos].value = a[j].value;
                    curr_pos++;
                    printf("%d\n", curr_pos);
                }
    }
} */

/* void fastTranspose(transpose a[],transpose b[]) {    // 빠른 전환
    int row_transposes[MAX_COL];
    int starting_pos[MAX_COL];
    int i,j;
    int num_cols = b[0].row = a[0].col;
    int num_transposes = b[0].value = a[0].value;
    b[0].col = a[0].row;

    if(num_transposes > 0) {
        for(i = 0; i < num_cols; i++) //init
            row_transposes[i] = 0;
        for(i = 1; i <= num_transposes; i++)
            row_transposes[a[i].col]++;
        starting_pos[0] = 1;
        for(i = 1; i < num_cols; i++)
            starting_pos[i] = starting_pos[i-1] + row_transposes[i-1];

        for(i = 1; i <= num_transposes; i++) {
            for(j = 1; j <= num_transposes; j++) {
                j=starting_pos[a[i].col]++;
                b[j].row = a[i].col;
                b[j].col = a[i].row;
                b[j].value = a[i].value;
            }
        }
    }
} */