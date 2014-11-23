// 0/10
#include <cstdio>
#include <iostream>
#include <cassert>
#include <vector>
#include <algorithm>
using namespace std;

#define PROB "link"
void redirect() {
    freopen(PROB ".in", "r", stdin);
    freopen(PROB ".out", "w", stdout);
    ios::sync_with_stdio(false);
}

#define MAX_N   200000
#define MOD     10007
int N, W[MAX_N + 1];
int root, nil;
vector<int> G[MAX_N + 1];
int f_tab[MAX_N + 1][3], g_tab[MAX_N + 1][3];

int f(int u, int k) {
    if(k == 0)
        return W[u];

    if(f_tab[u][k] < 0)
        for(size_t i = 0; i < G[u].size(); ++i)
            f_tab[u][k] = max(f_tab[u][k], f(G[u][i], k - 1));

    return f_tab[u][k];
}

int g(int u, int k) {
    if(k == 0)
        return W[u];

    if(g_tab[u][k] < 0) {
        g_tab[u][k] = 0;
        for(size_t i = 0; i < G[u].size(); ++i)
            g_tab[u][k] = (g_tab[u][k] + g(G[u][i], k - 1)) % MOD;
    }

    return g_tab[u][k];
}

void input() {
    redirect();

    cin >> N;
    for(int i = 0; i < N - 1; ++i) {
        int u, v;
        cin >> u >> v;

        G[u].push_back(v);
        G[v].push_back(u);
    }

    for(int i = 1; i <= N; ++i)
        cin >> W[i];
}

void build_tree(int r, int p) {
    for(size_t i = 0; i < G[r].size(); ++i) {
        if(G[r][i] == p)
            G[r].erase(G[r].begin() + i--);
        else
            build_tree(G[r][i], r);
    }
}

int main() {
    input();

    nil = 0;
    root = 1;
    build_tree(root, nil);

    for(int u = 1; u <= N; ++u)
    for(int k = 0; k <= 2; ++k)
        f_tab[u][k] = -1, g_tab[u][k] = -1;

    int ans1 = 0, ans2 = 0;
    for(int u = 1; u <= N; ++u) {
        ans1 = max(ans1, f(u, 2) * W[u]);
        ans2 = (ans2 + g(u, 2) * W[u]) % MOD;
    }
    ans2 = (ans2 * 2) % MOD;

    cout << ans1 << " " << ans2 << endl;
    return 0;
}
