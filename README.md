# Ford Fulkerson Algorithm
정보통신공학과 202101624 이수빈


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

> 세부사항  
 초기에는 흐름이 없고 초기 잔존 용량이 원래 용량과 동일하므로 잔차 그래프를 원래 그래프로 초기화한다.   
 BFS를 사용하면 소스(s)에서 싱크(t)까지의 거리를 경로의 유무를 알 수 있다. 그래서 DFS보다 BFS를 더 많이 사용한다.   
 잔차 그래프의 두 꼭짓점 사이에 모서리가 없는 경우 잔차 용량은 0이다.   
 BFS가 구축하는 배열을 사용하여 발견된 경로와 통과한 경로를 따라 최소 잔류 용량을 찾아 가능한 흐름을 찾는다.
 나중에 발견된 경로 흐름은 전체 흐름에 추가한다.


## Ford Fulkerson Algorithm Code
* * *
필요한 변수 및 큐
```c
#define WHITE 0
#define GRAY 1
#define BLACK 2
#define MAX_NODES 1000
#define o 1000000000

int n;  // 노드(node) 수
int e;  // 간선(edge) 수
int capacity[MAX_NODES][MAX_NODES]; // 용량 행렬
int flow[MAX_NODES][MAX_NODES];     // 흐름 행렬
int color[MAX_NODES]; // 폭 우선 탐색(BFS)에 필요              
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
```
폭 우선 탐색(BFS) 사용
```c
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
```
포드-폴커스 알고리즘
```c
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
```
파일 입출력 함수 및 main 함수
```c
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
```
#### 실험 결과
- - -
 1. data.txt 실행   
![txt](https://github.com/LeeSuuuuu/Ford-Fulkerson-Algorithm-/blob/bafe4f2c5d17d17690abbb386d391846200781e9/%EC%9D%B4%EB%AF%B8%EC%A7%80/data.png)
![graph](https://github.com/LeeSuuuuu/Ford-Fulkerson-Algorithm-/blob/bafe4f2c5d17d17690abbb386d391846200781e9/%EC%9D%B4%EB%AF%B8%EC%A7%80/data%20graph.jpg)
![실행 결과](https://github.com/LeeSuuuuu/Ford-Fulkerson-Algorithm-/blob/bafe4f2c5d17d17690abbb386d391846200781e9/%EC%9D%B4%EB%AF%B8%EC%A7%80/data%20%EC%8B%A4%ED%96%89%20%EA%B2%B0%EA%B3%BC.png)

2. data1.txt 실행 결과   
![txt](https://github.com/LeeSuuuuu/Ford-Fulkerson-Algorithm-/blob/bafe4f2c5d17d17690abbb386d391846200781e9/%EC%9D%B4%EB%AF%B8%EC%A7%80/data1.png)
![graph](https://github.com/LeeSuuuuu/Ford-Fulkerson-Algorithm-/blob/bafe4f2c5d17d17690abbb386d391846200781e9/%EC%9D%B4%EB%AF%B8%EC%A7%80/data1%20graph.jpg)
![실행결과](https://github.com/LeeSuuuuu/Ford-Fulkerson-Algorithm-/blob/bafe4f2c5d17d17690abbb386d391846200781e9/%EC%9D%B4%EB%AF%B8%EC%A7%80/data1%20%EC%8B%A4%ED%96%89%20%EA%B2%B0%EA%B3%BC.png)

## 성능
- - -    
 * 시간 복잡성   
  증강 경로가 있는 동안 루프를 실행한다. 최악의 경우 반복할 때마다 단위 흐름을 1개 추가할 수 있다.   
    따라서 시간 복잡도는 O(max_flow * E)가 됩니다.
* data1.txt의 중복되는 경로가 data.txt보다 적으므로 data1.txt의 시간 복잡도가 더 작다.








