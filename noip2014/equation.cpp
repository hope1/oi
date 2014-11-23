// 7/10
#include <cstdio>
#include <iostream>
#include <cassert>
#include <algorithm>
#include <vector>
#include <cstring>
#include <ctime>
#include <cstdlib>
using namespace std;

#ifdef Mdebug
#   define  Massert(expr)   assert(expr)
#else
#   define  Massert(expr)
#endif

#define PROB "equation"
void redirect() {
    freopen(PROB ".in", "r", stdin);
    freopen(PROB ".out", "w", stdout);
    ios::sync_with_stdio(false);
}

#define MAX_PRIME   10000
#define MAX_NPRIME  2000
int prime[MAX_NPRIME], nprime;
void sieve(int n) {
    static bool tab[MAX_PRIME + 1];
    fill_n(tab, n + 1, false);

    for(int p = 2; p * p <= n; ++p)
        if(!tab[p])
            for(int i = p; i * p <= n; ++i)
                tab[i * p] = true;

    for(int i = 2; i <= n; ++i)
        if(!tab[i])
            prime[nprime++] = i;
}

void generate_prime(int n, int k) {
    sieve(n);
    random_shuffle(prime, prime + nprime);
    nprime = min(nprime, k);
}

int mod(long long a, int p) {
    if(a < 0)
        a += ((-a / p) + 1) * p;
    Massert(a >= 0);
    return a % p;
}

#define K       20
#define MAX_N   100
#define MAX_LEN 10000
int n, m, a_len[MAX_N + 1];
char a[MAX_N + 1][MAX_LEN + 10];
int b[K][MAX_N + 1];

int eval(int *c, int p, int x) {
    long long r = c[n];
    for(int k = n - 1; k >= 0; --k)
        r = mod(r * x + c[k], p);
    return r;
}

bool test(int x) {
    for(int i = 0; i < nprime; ++i)
        if(eval(b[i], prime[i], x % prime[i]) != 0)
            return false;
    return true;
}

int main() {
    srand(time(NULL));
    redirect();
    cin >> n >> m;

    for(int i = 0; i <= n; ++i) {
        cin >> a[i];
        a_len[i] = strlen(a[i]);
    }

    generate_prime(MAX_PRIME, K);
    for(int i = 0; i < nprime; ++i) {
        for(int k = 0; k <= n; ++k) {
            for(int p = (a[k][0] == '-'); p < a_len[k]; ++p)
                b[i][k] = mod(b[i][k] * 10 + (a[k][p] - '0'), prime[i]);

            if(a[k][0] == '-')
                b[i][k] = mod(-b[i][k], prime[i]);
        }
    }

    vector<int> root;
    for(int x = 1; x <= m; ++x)
        if(test(x))
            root.push_back(x);

    cout << root.size() << endl;
    for(size_t i = 0; i < root.size(); ++i)
        cout << root[i] << endl;

    return 0;
}
