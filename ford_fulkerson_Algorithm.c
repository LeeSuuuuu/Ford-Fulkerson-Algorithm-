#include <stdio.h>

#define WHITE 0
#define GRAY 1
#define BLACK 2
#define MAX_NODES 1000
#define o 1000000000

int n;  // 노드(node) 수
int e;  // 간선(edge) 수
int capacity[MAX_NODES][MAX_NODES]; // 용량 행렬
int flow[MAX_NODES][MAX_NODES];     // 흐름 행렬
int color[MAX_NODES]; // 폭 우선 탐색에 필요              
int pred[MAX_NODES];  // 확장 경로를 저장하는 배열

int min(int x, int y) {
    return x < y ? x : y;  // 작은 숫자 출력
}
int head, tail;
int q[MAX_NODES + 2];

void enqueue(int x) {
    q[tail] = x;
    tail++;
    color[x] = GRAY;
}

int dequeue() {
    int x = q[head];
    head++;
    color[x] = BLACK;
    return x;
}

// BFS를 검색 알고리즘으로 사용
int bfs(int start, int target) {
    int u, v;
    for (u = 0; u < n; u++) {
        color[u] = WHITE;
    }
    head = tail = 0;
    enqueue(start);
    pred[start] = -1;
    while (head != tail) {
        u = dequeue();
        // 용량인 경우 인저반 모든 흰색 노드v 검색
        // u에서 v까지 양수이고, v 입력
        for (v = 0; v < n; v++) {
            if (color[v] == WHITE && capacity[u][v] - flow[u][v] > 0) {
                enqueue(v);
                pred[v] = u;
            }
        }
    }

    // 대상 노드의 색상이 지금 검은색이면, 도달했다는 것을 의미한다.
    return color[target] == BLACK;
}

//ford fulkerson 알고리즘 적용
int max_flow(int source, int sink) {
    int i, j, u;
    // 빈 흐름 초기화
    int max_flow = 0;
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            flow[i][j] = 0;
        }
    }

    // 증강 결로가 존재하는 동안, 이경로를 따라 흐름을 증가
    // 즉, 간선의 잔차값 업데이트
    while (bfs(source, sink)) {
        // 흐름을 증가시킬 양을 결정
        int increment = o;
        for (u = n - 1; pred[u] >= 0; u = pred[u]) {
            increment = min(increment, capacity[pred[u]][u] - flow[pred[u]][u]);
        }

        // 흐름 증가
        for (u = n - 1; pred[u] >= 0; u = pred[u]) {
            flow[pred[u]][u] += increment;
            flow[u][pred[u]] -= increment;
        }
        max_flow += increment;
    }

    // 증가 경로 없음. 끝
    return max_flow;
}

void read_input_file() {
    int a, b, c, i, j;
    FILE* input = fopen("data.txt", "r");

    // 노드와 간선 수 읽기
    fscanf(input, "%d %d", &n, &e);
    printf("\n꼭짓점 수 : %d   간선 수 : %d", n, e);

    // 빈 용량 행렬 초기화
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            capacity[i][j] = 0;
        }
    }

    // 간선 용량 읽기
    for (i = 0; i < e; i++) {
        fscanf(input, "%d %d %d", &a, &b, &c);
        capacity[a][b] = c;
        printf("\nA : %d  B : %d  용량 : %d", a, b, c);
    }
    fclose(input);
}

int main() {
    read_input_file();
    printf("\n소스(s)와 싱크(t)를 입력하시오 :\n");
    int s = 0, t = n - 1;
    scanf("%d %d", &s, &t);
    printf("\n최대 흐름 : %d\n", max_flow(s, t));
    return 0;
}