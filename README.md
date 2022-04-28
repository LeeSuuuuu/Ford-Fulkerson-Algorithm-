# Ford Fulkerson Algorithm
202101624 이수빈


## 포드-폴커스 알고리즘(Ford Fulkerson Algorithm)이란?
- - -
포드-폴커슨 알고리즘은 네트워크 또는 그래프에서 가능한 최대 흐름을 계산하기 위한 접근법이다.   
그래프에서 두 꼭짓점 소스(s)와 싱크(t)가 주어지면 다음과 같은 조건을 가진 st에서 가능한 최대 흐름을 구한다.
> 첫째, 가장자리의 흐름은 지정된 가장자리 용량을 초과하지 않는다.   
둘째, 들어오는 흐름은 s와 t를 제외한 모든 정점에 대한 나가는 흐름과 같다.


## 코드 동작 방식
- - -
1. 모든 간선의 흐름을 0으로 초기화한다.
2. 소스와 싱크 사이에 결로가 있는 동안 이 경로에 흐름을 추간한다.
3. 잔차 그래프를 업데이트 한다.
* 잔차 그래프란?
추가 가능한 흐름이 있는 흐름 네트워크를 의미한다.
* 흐름 네트워크란? 각각의 간선에 정해진 용량보다 작은 흐름이 주어진 방향 그래프를 의미한다.


Ford Fulkerson Algorithm Code
* * *
```c
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
        // u에서 v까지 양수이고, v 제거
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
    FILE* input = fopen("data1.txt", "r");

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
    printf("\n소스(s)와 싱크(t)를 입력하시오 : ");
    int s = 0, t = n - 1;
    scanf("%d %d", &s, &t);
    printf("\n최대 흐름 : %d\n", max_flow(s, t));
    return 0;
}
```

> 해당 코드는 사이트의 코드를 기반으로 약간의 변화를 준 것이다. 
[이 사이트 참고](https://www.programiz.com/dsa/ford-fulkerson-algorithm)

> 실행 결과

> 성능 분석   

 * 시간 복잡성   
   상기 알고리즘의 시간 복잡도는 O(max_flow * E)이다. 우리는 증강 경로가 있는 동안 루프를 실행합니다. 최악의 경우 반복할 때마다 단위 흐름을 1개 추가할 수 있습니다. 따라서 시간 복잡도는 O(max_flow * E)가 됩니다.

* 








