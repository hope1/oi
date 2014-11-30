// 10/10
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
#define M       10007
int N, W[MAX_N + 1];
vector<int> G[MAX_N + 1];

bool cmpgt_w(int lhs, int rhs) {
    return W[lhs] > W[rhs];
}

int mod(int a, int m) {
    if(a < 0)
        a += ((-a / m) + 1) * m;
    return a % m;
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

int main() {
    input();

    int wmax = 0, wsum = 0;
    for(int i = 1; i <= N; ++i) {
        if(G[i].size() >= 2) {
            partial_sort(G[i].begin(), G[i].begin() + 2, G[i].end(), cmpgt_w);
            wmax = max(wmax, (W[G[i][0]] % M) * (W[G[i][1]] % M));

            //   sum w(u, v)
            // = 2 * sum (w(u, v) [u < v])
            // = (sum Wu)^2 - sum (Wu^2)
            int sum = 0, sqrsum = 0;
            for(int k = 0; k < G[i].size(); ++k) {
                sum = mod(sum + W[G[i][k]], M);
                sqrsum = mod(sqrsum + W[G[i][k]] * W[G[i][k]], M);
            }
            wsum = mod(wsum + sum * sum - sqrsum, M);
        }
    }

    cout << wmax << " " << wsum << endl;
    return 0;
}
