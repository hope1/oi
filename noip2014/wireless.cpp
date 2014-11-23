// 10/10
#include <cstdio>
#include <iostream>
#include <cassert>
#include <algorithm>
using namespace std;

#ifdef Mdebug
#   define  Massert(expr)   assert(expr)
#else
#   define  Massert(expr)
#endif

#define PROB "wireless"
void redirect() {
    freopen(PROB ".in", "r", stdin);
    freopen(PROB ".out", "w", stdout);
    ios::sync_with_stdio(false);
}

#define SIZE    129
#define MAX_N   20
int X[MAX_N], Y[MAX_N], K[MAX_N];

int main() {
    redirect();

    int d, n;
    cin >> d >> n;
    for(int i = 0; i < n; ++i)
        cin >> X[i] >> Y[i] >> K[i];

    int ans = 0, cnt = 0;
    for(int i = 0; i < SIZE; ++i) {
        for(int j = 0; j < SIZE; ++j) {
            int s = 0;
            for(int k = 0; k < n; ++k)
                if(abs(i - X[k]) <= d && abs(j - Y[k]) <= d)
                    s += K[k];

            if(s == ans)
                ++cnt;
            else if(s > ans) {
                ans = s;
                cnt = 1;
            }
        }
    }

    cout << cnt << " " << ans << endl;
    return 0;
}
