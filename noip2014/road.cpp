// 10/10
#include <cstdio>
#include <iostream>
#include <cassert>
#include <algorithm>
#include <set>
#include <queue>
using namespace std;

#ifdef Mdebug
#   define  Massert(expr)   assert(expr)
#else
#   define  Massert(expr)
#endif

#define PROB "road"
void redirect() {
    freopen(PROB ".in", "r", stdin);
    freopen(PROB ".out", "w", stdout);
    ios::sync_with_stdio(false);
}

#define MAX_N   10000
#define INF     (10 * MAX_N + 17)
int n, m;
set<int> G[MAX_N + 1], Grev[MAX_N + 1];
int dist[MAX_N + 1];
bool del[MAX_N + 1];

void bfs(const set<int> (&graph)[MAX_N + 1], int s) {
    static bool color[MAX_N + 1];
    fill_n(color, n + 1, false);

    for(int u = 1; u <= n; ++u)
        dist[u] = INF;
    dist[s] = 0;

    queue<int> q;
    q.push(s);
    color[s] = true;

    while(!q.empty()) {
        int u = q.front();
        q.pop();

        for(set<int>::iterator it = graph[u].begin(); it != graph[u].end(); ++it) {
            if(!del[*it] && !color[*it]) {
                dist[*it] = dist[u] + 1;
                q.push(*it);
                color[*it] = true;
            }
        }
    }
}

int main() {
    redirect();

    cin >> n >> m;
    for(int i = 0; i < m; ++i) {
        int x, y;
        cin >> x >> y;
        G[x].insert(y);
        Grev[y].insert(x);
    }

    int s, t;
    cin >> s >> t;

    bfs(Grev, t);
    for(int u = 1; u <= n; ++u) {
        Massert(dist[u] <= INF);
        if(dist[u] == INF)
            del[u] = true;

        for(set<int>::iterator it = G[u].begin(); it != G[u].end(); ++it)
            if(dist[*it] == INF)
                del[u] = true;
    }

    if(del[s])
        cout << -1 << endl;
    else {
        bfs(G, s);
        cout << (dist[t] < INF ? dist[t] : -1) << endl;
    }
    return 0;
}
