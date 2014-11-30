// 10/10
#include <cstdio>
#include <iostream>
#include <cassert>
#include <algorithm>
#include <vector>
#include <cstring>
#include <ctime>
#include <cstdlib>
#include <functional>
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

#define MAX_PRIME   30000
#define MIN_PRIME   1000
#define MAX_NPRIME  5000
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
    int *it = remove_if(prime, prime + nprime, bind2nd(less<int>(), MIN_PRIME));
    random_shuffle(prime, it);
    nprime = min(nprime, k);
}

int mod(long long a, int p) {
    if(a < 0)
        a += ((-a / p) + 1) * p;
    Massert(a >= 0);
    return a % p;
}

#define K       10
#define MAX_N   100
#define MAX_LEN 10000
int n, m, a_len[MAX_N + 1];
char a[MAX_N + 1][MAX_LEN + 10];
int b[K][MAX_N + 1];
int r[K][MAX_PRIME + 1];

int eval(int i, int x) {
    if(r[i][x] < 0) {
        long long z = b[i][n];
        for(int k = n - 1; k >= 0; --k)
            z = mod(z * x + b[i][k], prime[i]);
        r[i][x] = z;
    }
    return r[i][x];
}

bool test(int x) {
    for(int i = 0; i < nprime; ++i)
        if(eval(i, x % prime[i]) != 0)  // f(x + kp) = f(x) (mod p)
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

    for(int i = 0; i < nprime; ++i)
        for(int x = 0; x <= MAX_PRIME; ++x)
            r[i][x] = -1;

    vector<int> root;
    for(int x = 1; x <= m; ++x)
        if(test(x))
            root.push_back(x);

    cout << root.size() << endl;
    for(size_t i = 0; i < root.size(); ++i)
        cout << root[i] << endl;

    return 0;
}
