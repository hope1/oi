// 15/20
#include <cstdio>
#include <iostream>
#include <cassert>
#include <algorithm>
using namespace std;

#define PROB "bird"
void redirect() {
    freopen(PROB ".in", "r", stdin);
    freopen(PROB ".out", "w", stdout);
    ios::sync_with_stdio(false);
}

#ifdef Mdebug
#   define Massert(expr) assert(expr)
#else
#   define Massert(expr) 0
#endif

int iceil(int p, int q) {
    return (p / q) + !!(p % q);
}

#define MAX_N   10000
#define MAX_M   1000
#define MAX_K   MAX_N
#define MAX_F   MAX_N * MAX_M
#define INF     (10 * MAX_F + 17)
int n, m, k, X[MAX_N], Y[MAX_N], L[MAX_N + 1], H[MAX_N + 1];
int f[MAX_N + 1][MAX_M + 1];

void input() {
    redirect();

    cin >> n >> m >> k;
    for(int i = 0; i < n; ++i)
        cin >> X[i] >> Y[i];

    for(int i = 0; i <= n; ++i) {
        L[i] = 0;
        H[i] = m + 1;
    }

    for(int i = 0; i < k; ++i) {
        int p, l, h;
        cin >> p >> l >> h;
        L[p] = l;
        H[p] = h;
    }
}

int main() {
    input();

    for(int i = 0; i <= n; ++i)
    for(int j = 0; j <= m; ++j)
        f[i][j] = INF;

    for(int j = 1; j <= m; ++j)
        f[0][j] = 0;

    int last = 0;
    for(int i = 1; i <= n; ++i) {
        bool found = false;
        for(int j = L[i] + 1; j < H[i]; ++j) {
            if(L[i - 1] < j + Y[i - 1] && j + Y[i - 1] < H[i - 1])
                f[i][j] = f[i - 1][j + Y[i - 1]];

            if(j == m) {
                for(int k = L[i - 1] + 1; k < H[i - 1]; ++k)
                    f[i][j] = min(f[i][j], f[i - 1][k] + max(iceil(j - k, X[i - 1]), 1));
            } else {
                int k = L[i - 1] - (L[i - 1] % X[i - 1]) + (j % X[i - 1]);
                k += (k <= L[i - 1]) * X[i - 1];

                Massert(k % X[i - 1] == j % X[i - 1]);
                Massert(L[i - 1] < k);

                for(; k < H[i - 1] && k < j; k += X[i - 1])
                    f[i][j] = min(f[i][j], f[i - 1][k] + (j - k) / X[i - 1]);
            }

            if(f[i][j] < INF)
                found = true;
        }
        if(found)
            last = i;
        else
            break;
    }

    if(last != n) {
        int k = n;
        while(k > 0 && *min_element(f[k] + L[k] + 1, f[k] + H[k]) == INF) --k;

        int cnt = 0;
        for(int i = 1; i <= k; ++i)
            if(H[i] <= m)
                ++cnt;

        cout << 0 << endl << cnt << endl;
    } else
        cout << 1 << endl << *min_element(f[n] + L[n] + 1, f[n] + H[n]) << endl;

    return 0;
}
