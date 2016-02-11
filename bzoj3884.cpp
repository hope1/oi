// http://www.lydsy.com/JudgeOnline/problem.php?id=3884
#define PROB "bzoj3884"
#include <cstdio>
#include <cmath>
#include <vector>
#include <algorithm>
using namespace std;

typedef unsigned long long uint64;
int modpow(int b, int e, int m) {
    if(m == 1)
        return 0;

    int r = 1;
    for(; e > 0; e >>= 1, b = ((uint64)b * b % m))
        if(e & 1)
            r = ((uint64)r * b % m);
    return r;
}

int mod(int a, int m) {
    int r = a % m;
    return r < 0 ? r + abs(m) : r;
}

/*
    Let f(n) = 2^2^... mod n, and p = 2^t * s where (p, s) = 1.
    f(p) = 2^t (2^(2^2^... - t) mod q)
         = 2^t (2^W mod q)
    where W = (2^2^... - t) mod \phi(q) = (f(\phi(q)) - t) mod \phi(q).

    Observe that f(1) = 0, and that the argument is halved in most steps
    where \phi(q) would be even. The number of recursions is O(log n).
*/

// O(p log p + T)
namespace Student {
    const int BigNewsLimit = 10000001; // 10^7
    const int PiLimit = 1000001; // > pi(10^7)

    int bignews[BigNewsLimit], phi[BigNewsLimit];
    short a1[BigNewsLimit];
    bool composite[BigNewsLimit];
    int p[PiLimit], pi;

    void tabulate(int z) {
        bignews[1] = 0;
        composite[1] = 0;
        a1[1] = 0;
        phi[1] = 1;

        for(int s = 2; s <= z; ++s) {
            if(!composite[s]) {
                p[pi++] = s;
                a1[s] = 1;
                phi[s] = s - 1;
            }

            for(int i = 0; i < pi; ++i) {
                int k = p[i] * s;
                if(k > z)
                    break;
                composite[k] = true;

                if(s % p[i] != 0) {
                    a1[k] = 1;
                    phi[k] = phi[p[i]] * phi[s];
                } else {
                    a1[k] = a1[s] + 1;
                    phi[k] = p[i] * phi[s];
                    break;
                }
            }

            int t = 0, q = s;
            if(s % 2 == 0) {
                t = a1[s];
                q = s >> t;
            }
            bignews[s] = modpow(2, mod(bignews[phi[q]] - t, phi[q]), q) << t;
        }
    }
}

// O(Tsqrt(p)log(p))
namespace Elder {
    int phi(int n) {
        int r = 1;
        for(int s = 2; s * s <= n; ++s) {
            if(n % s == 0) {
                r *= s - 1;
                n /= s;
                while(n % s == 0) {
                    r *= s;
                    n /= s;
                }
            }
        }
        if(n > 1)
            r *= n - 1;
        return r;
    }

    int recurse(int n) {
        if(n == 1)
            return 0;

        int t = 0;
        for(int z = n; (z & 1) == 0; z >>= 1)
            ++t;
        int q = n >> t;
        int phi_q = phi(q);

        return modpow(2, mod(recurse(phi_q) - t, phi_q), q) << t;
    }
}

namespace Reporter {
    void run() {
        vector<int> vec;
        int T, n, z = 0;

        scanf("%d", &T);
        for(int i = 0; i < T; ++i) {
            scanf("%d", &n);
            vec.push_back(n);
            z = max(z, n);
        }
/*
        Student::tabulate(z);
        for(int i = 0; i < T; ++i)
            printf("%d", Student::bignews[vec[i]] == Elder::recurse(vec[i]));
*/
        for(int i = 0; i < T; ++i)
            printf("%d\n", Elder::recurse(vec[i]));

    }
};

int main() {
    Reporter::run();
    return 0;
}
