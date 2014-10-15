// http://cdqz.openjudge.cn/2016/0142/
#include <cstdio>
#include <iostream>
#include <algorithm>
#include <cstring>
#include <cstdlib>
using namespace std;

#define PROB "seq"
void redirect(void) {
#ifndef ONLINE_JUDGE
    freopen(PROB ".in", "r", stdin);
    freopen(PROB ".out", "w", stdout);
#endif
    ios::sync_with_stdio(false);
}

#ifdef Mdebug
#   define Mdprint(...) fprintf(stderr, __VA_ARGS__)
#else
#   define Mdprint(...) 0
#endif

#define Mrelax(cond, r, v) \
    do { \
        int &_old = (r), _new = (v); \
        if(cond) \
            _old = _new;\
    } while(0)

#define MAX_SIZE    100
#define MAX_VALUE   60
#define MAX_NPRIME  20  // pi(MAX_VALUE) + 1
#define MAX_COST    MAX_VALUE * MAX_SIZE
#define MAX_STATE   (1 << MAX_NPRIME)

static const int prime[] = { // max prime[] > MAX_VALUE
    2, 3, 5, 7, 11, 13, 17, 19, 23, 29,
    31, 37, 41, 43, 47, 51, 53, 57, 59, 61
};

typedef unsigned long bset; // binary state

bset calc_prime_factor(int k) {
    bset r = 0;
    for(int i = 0; prime[i] <= k; ++i)
        if(!(k % prime[i]))
            r |= (1 << i);
    return r;
}

// better just change to 1 if b[i] > 2 * a[i] - 2
#define Max_bi(ai) (ai == 1 ? 1 : ai * 2 - 2)

int n, a[MAX_SIZE];
bset p[MAX_VALUE + 1];
/*  f[i][s] = min cost at state s for the first i elements.
    A state is a bit vector representing whether each prime
    has appeared as a factor of previous resulting elements. */
int f[2][MAX_STATE];

int main(void) {
    for(int k = 1; k <= MAX_VALUE; ++k)
        p[k] = calc_prime_factor(k);

    redirect();
    cin >> n;
    for(int i = 0; i < n; ++i)
        cin >> a[i];
    // use the least possible bits for a state
    int max_pi = lower_bound(prime, prime + MAX_NPRIME, Max_bi(*max_element(a, a + n))) \
                 - prime;
    int max_state = (1 << max_pi);

    fill_n(f[0], 2 * max_state, MAX_COST);
    f[0][0] = 0;

    int c = 0;
    for(int i = 0; i < n; ++i, c ^= 1) {
        fill_n(f[c ^ 1], max_state, MAX_COST);

        for(bset s = 0; s < max_state; ++s)
        if(f[c][s] < MAX_COST) {
            for(int k = 1; k <= Max_bi(a[i]); ++k) // simply try each b[i]
            if(!(s & p[k]))
                Mrelax(_new < _old, f[c ^ 1][s | p[k]], f[c][s] + abs(a[i] - k));
        }
    }

    cout << *min_element(f[c], f[c] + max_state) << endl;
    return 0;
}
