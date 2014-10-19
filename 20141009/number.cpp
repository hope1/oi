// http://cdqz.openjudge.cn/2016/0143/
#include <cstdio>
#include <iostream>
#include <utility>
#include <string>
#include <vector>
#include <tr1/tuple>
using namespace std;
using namespace std::tr1;

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

typedef long long ll;

int madd(int a, int b, int m) {
    return ((ll)a + b) % m;
}

int mmul(int a, int b, int m) {
    return ((ll)a * b) % m;
}

int mexp(int b, int e, int m) {
    int r = 1;
    for(; e; e >>= 1, b = mmul(b, b, m))
        if(e & 1)
            r = mmul(r, b, m);
    return r;
}

#define M 1000000007 // 10^9 + 7

#define MAX_LEN 100001 // 10^5 + '\0'
#define MAX_SUB 100000 // 10^5

int Q, a[MAX_SUB];
string S, b[MAX_SUB];

void input(void) {
    redirect();
    cin >> S;

    cin >> Q;
    for(int i = 0; i < Q; ++i) {
        int head;
        static char body[MAX_LEN + 2];
        cin >> head >> body;
        a[i] = head;
        b[i] = body;
        b[i] = b[i].substr(2); // ->
    }
}

/*
    First note that all substitutions are processed in order, and the problem
    is equivalent with any starting string X and a special starting rule X -> S.

    For a substitution (a -> b[]),
    let r'(a) be the result of applying all subsequent substitutions on a,
    and r(a) be the same but including the current one,
    then    r(a) = sum_i (r'(b[i]) * 10^L(i)),
    where   L(i) = sum_p [i + 1, #B) len(r'(b[p])).

    Therefore r(k) can be stored modulo M, only len(r(k)) need to be kept separately.
    len(r(k)) is used in the exponent, so it can be stored modulo M - 1, since
        a^(b(M - 1) + c) = (a^(M - 1))^b * a^c = a^c (mod M).

    r[i][k] = r(k) with substitution [i, Q) applied (in order).
*/
int r[MAX_SUB + 1][10], len[MAX_SUB + 1][10];

// (ans, len)
pair<int, int> calc(string s, int *r, int *len) {
    int ans = 0, L = 0;
    for(string::reverse_iterator c = s.rbegin(); c != s.rend(); ++c) {
        ans = madd(ans, mmul(r[*c - '0'], mexp(10, L, M), M), M);
        L = madd(L, len[*c - '0'], M - 1);
    }
    return make_pair(ans, L);
}

int main(void) {
    input();

    for(int i = 0; i < 10; ++i)
        r[Q][i] = i, len[Q][i] = 1;

    for(int i = Q - 1; i >= 0; --i) {
        for(int k = 0; k < 10; ++k)
            r[i][k] = r[i + 1][k], len[i][k] = len[i + 1][k];
        tie(r[i][a[i]], len[i][a[i]]) = calc(b[i], r[i + 1], len[i + 1]);
    }

    cout << calc(S, r[0], len[0]).first << endl;
    return 0;
}
