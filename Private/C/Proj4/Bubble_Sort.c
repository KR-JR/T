#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void bubbleSort(int arr[], int n) {
    int i, j;
    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                // 두 원소의 위치를 교환
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

int main() {
    FILE *file;
    int *arr;
    int size, i;

    // 텍스트 파일 열기
    file = fopen("input.txt", "r");
    if (file == NULL) {
        printf("파일을 열 수 없습니다.");
        return 1;
    }

    // 파일에서 크기 읽기
    fscanf(file, "%d", &size);

    // 배열 동적 할당
    arr = (int *)malloc(size * sizeof(int));
    if (arr == NULL) {
        printf("메모리를 할당할 수 없습니다.");
        return 1;
    }

    // 파일에서 데이터 읽기
    for (i = 0; i < size; i++) {
        fscanf(file, "%d", &arr[i]);
    }

    // 파일 닫기
    fclose(file);

    // 버블 정렬 시간 측정 시작
    clock_t start = clock();

    // 버블 정렬 호출
    bubbleSort(arr, size);

    // 버블 정렬 시간 측정 종료
    clock_t end = clock();

    // 정렬된 결과 출력
    printf("정렬된 결과: ");
    for (i = 0; i < size; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    // 걸린 시간 계산 및 출력 (초 단위)
    double cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Execution Time: %fSec\n", cpu_time_used);

    // 배열 메모리 해제
    free(arr);

    return 0;
}
