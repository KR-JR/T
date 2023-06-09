#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void shellSort(int arr[], int n) {
    int gap, i, j, temp;

    // 셸 정렬에 사용할 gap 설정
    for (gap = n / 2; gap > 0; gap /= 2) {
        for (i = gap; i < n; i++) {
            temp = arr[i];

            for (j = i; j >= gap && arr[j - gap] > temp; j -= gap) {
                arr[j] = arr[j - gap];
            }

            arr[j] = temp;
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

    // 셸 정렬 시간 측정 시작
    clock_t start = clock();

    // 셸 정렬 호출
    shellSort(arr, size);

    // 셸 정렬 시간 측정 종료
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
