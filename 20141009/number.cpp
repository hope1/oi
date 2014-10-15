/*  http://cdqz.openjudge.cn/2016/0143/

    Answer = sum_i (r(Ai) * 10^Li) % M,
    where   r(k) is the result after substitution,
    and     Li = sum_j [0, i-1] len(r(Aj)).

    Therefore r(k) can be stored modulo M, just keep len(r(k)) separately.
    len(r(k)) is used in the exponent, so it can be stored modulo M - 1:
        from Fermat's little theorem,
        a^(b(M - 1) + c) = (a^(M - 1))^b * a^c = a^c (mod M).
*/
#include <cstdio>
#include <iostream>
#include <algorithm>
#include <cstring>
#include <cstdlib>
using namespace std;

#define PROB "number"
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


int main(void) {
    redirect();

    return 0;
}
