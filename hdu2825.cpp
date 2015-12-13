#define PROB "hdu2825"
#include <cassert>
#include <ctime>
#include <cstdio>
#include <cstring>
#include <algorithm>
#include <queue>
using namespace std;

const int MaxN = 27;
const int MaxM = 12;
const int MaxW = 12;
const int MaxT = MaxW * MaxM;
const int Alphabet = 26;
const int Mod = 20090717;

const int MaxS = 1 << MaxM;
typedef unsigned bitvec;

struct trie {
    trie *c[Alphabet];
    trie *fail;
    bitvec inc;
};

int tn;
trie node[MaxT], *nil, *root;

static inline
trie* new_node() {
    trie *q = &node[tn++];
    *q = *nil;
    return q;
}

void new_trie() {
    tn = 0;
    nil = &node[tn++];
    for(int a = 0; a < Alphabet; ++a)
        nil->c[a] = nil;
    nil->fail = nil;
    nil->inc = 0;

    root = new_node();
}

static inline
trie* next(trie* q, int a) {
    return q->c[a];
}

void insert(trie* q, const char *s, int i) {
    while(*s != '\0') {
        if(next(q, *s - 'a') == nil)
            q->c[*s - 'a'] = new_node();
        q = next(q, *s - 'a');
        ++s;
    }
    q->inc |= (1u << i);
}

static inline
trie* delta(trie *q, int a) {
    while(q != root && next(q, a) == nil)
        q = q->fail;
    if(next(q, a) != nil)
        q = next(q, a);
    return q; 
}

void build_fail() {
    queue<trie*> Q;
    root->fail = root;
    for(int a = 0; a < Alphabet; ++a) {
        trie *r = next(root, a);
        if(r != nil) {
            r->fail = root;
            Q.push(r);
        }
    }

    while(!Q.empty()) {
        trie *q = Q.front();
        Q.pop();

        for(int a = 0; a < Alphabet; ++a) {
            trie *r = next(q, a);
            if(r != nil) {
                r->fail = delta(q->fail, a);
                r->inc |= r->fail->inc;
                Q.push(r);
            }
        }
    }
}

int n, m, k;
char magic[MaxW];
int dp[MaxN][MaxS][MaxT];

static inline
int popcnt(bitvec s) {
    int r = 0;
    do
        r += (s & 1);
    while(s >>= 1);
    return r;
}

#define Node(q) (&node[q])

static inline
void relax(int &v, int w) {
    v = (v + w) % Mod;
}

void build_dp() {
    for(int i = 0; i <= n; ++i)
        for(int q = 1; q < tn; ++q)
            for(bitvec s = 0; s < (1u << m); ++s)
                dp[i][s][q] = 0;
    dp[0][0][root - node] = 1;

    for(int i = 0; i < n; ++i) {
        for(bitvec s = 0; s < (1u << m); ++s) {
            for(int q = 1; q < tn; ++q) {
                if(dp[i][s][q] > 0) {
                    for(int a = 0; a < Alphabet; ++a) {
                        int p = delta(Node(q), a) - node;
                        relax(dp[i + 1][s | Node(p)->inc][p], dp[i][s][q]);
                    }
                }
            }
        }
    }
}

int main() {
    while(scanf("%d%d%d", &n, &m, &k), !(n == 0 && m == 0 && k == 0)) {
        new_trie();
        for(int i = 0; i < m; ++i) {
            scanf("%s", magic);
            insert(root, magic, i);
        }
        build_fail();

        build_dp();
        int ans = 0;
        for(bitvec s = 0; s < (1u << m); ++s) {
            int t = popcnt(s);
            if(t >= k) {
                for(int q = 1; q < tn; ++q) {
                    relax(ans, dp[n][s][q]);
                }
            }
        }

        printf("%d\n", ans);
    }
#ifdef Etimer
    fprintf(stderr, "Time: %.3lf s\n", static_cast<double>(clock()) / CLOCKS_PER_SEC);
#endif
    return 0;
}
