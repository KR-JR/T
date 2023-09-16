#include <stdio.h>
#include <stdbool.h>

#define MAX_VERTICES 100
#define INF 1000

int distance[MAX_VERTICES]; // 최단 거리 배열
bool visited[MAX_VERTICES]; // 방문 여부 배열

int num_vertices = 5; // 정점의 개수

// 그래프의 인접 행렬
int graph[MAX_VERTICES][MAX_VERTICES] = {
    {0, 160, 230, 320, INF},
    {160, 0, 160, 150, INF},
    {230, 160, 0, 140, 190},
    {320, 150, 140, 0, 60},
    {INF, INF, 190, 60, 0}
};

// 초기화
void initialize() {
    for (int i = 0; i < num_vertices; i++) {
        distance[i] = INF;
        visited[i] = false;
    }
}

// 최소 거리 정점 선택
int getMinDistanceVertex() {
    int minDistance = INF;
    int minVertex = -1;
    for (int v = 0; v < num_vertices; v++) {
        if (!visited[v] && distance[v] < minDistance) {
            minDistance = distance[v];
            minVertex = v;
        }
    }
    return minVertex;
}

// Dijkstra의 최단 경로 알고리즘
void dijkstra(int startVertex) {
    distance[startVertex] = 0;

    for (int count = 0; count < num_vertices - 1; count++) {
        int u = getMinDistanceVertex();
        visited[u] = true;

        for (int v = 0; v < num_vertices; v++) {
            if (!visited[v] && graph[u][v] != INF && distance[u] + graph[u][v] < distance[v]) {
                distance[v] = distance[u] + graph[u][v];
            }
        }
    }
}

// 결과 출력
void printShortestDistances(int startVertex) {
    printf("from 1 minimum distance\n");
    for (int v = 0; v < num_vertices; v++) {
        printf("%d, %d, %d\n", startVertex + 1, v + 1, distance[v]);
    }
}

int main() {
    initialize();
    dijkstra(0); // 1번 정점에서 시작
    printShortestDistances(0);

    return 0;
}
