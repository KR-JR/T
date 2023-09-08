#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

void heapify(int arr[], int n, int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n && arr[left] > arr[largest])
        largest = left;

    if (right < n && arr[right] > arr[largest])
        largest = right;

    if (largest != i) {
        swap(&arr[i], &arr[largest]);
        heapify(arr, n, largest);
    }
}

void heapSort(int arr[], int n) {
    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(arr, n, i);

    for (int i = n - 1; i > 0; i--) {
        swap(&arr[0], &arr[i]);
        heapify(arr, i, 0);
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

    // 힙 정렬 시간 측정 시작
    clock_t start = clock();

    // 힙 정렬 호출
    heapSort(arr, size);

    // 힙 정렬 시간 측정 종료
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