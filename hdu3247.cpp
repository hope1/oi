// http://acm.hdu.edu.cn/showproblem.php?pid=3247
#define PROB "hdu3247"
#include <cstdio>
#include <cstring>
#include <algorithm>
#include <queue>
using namespace std;

const int MaxN = 12;
const int MaxLen = 1234;
const int MaxM = 1234;
const int MaxVirus = 56789;
const int MaxQ = MaxVirus + MaxN * MaxLen;
const int Alphabet = 2;
const int Inf = 12 * MaxQ;

const int MaxS = 1 << MaxN;
typedef unsigned bitvec;

struct trie {
    trie *c[Alphabet];
    trie *fail;
    bool bad;
};

int tn;
trie node[MaxQ], *nil, *root;

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
    nil->bad = false;

    root = new_node();
}

#define next(q, a) ((q)->c[a])

trie* insert(trie* q, const char* s) {
    while(*s != '\0') {
        if(next(q, *s - '0') == nil)
            next(q, *s - '0') = new_node();
        q = next(q, *s - '0');
        ++s;
    }
    return q;
}

static inline
trie* delta(trie* q, int a) {
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
                r->bad |= r->fail->bad; // badness propagates through suffix relations
                Q.push(r);
            }
        }
    }
}

/*
    After 'bad' nodes on the Aho-Corasick automaton are excluded,
    including all 'good' strings is equivalent to walking a path from root
    which includes all corresponding terminal nodes, and we wish to minimize
    the total number of nodes walked.

    Notice that after preprocessing the ordered-pairwise distance between
    terminal nodes, this minimization becomes a variant of the TSP, which
    could be solved with bitmask DP in O(2^n n^2) time. The preprocessing
    is simply n BFSs on the AC automaton.
*/
int dist[MaxQ];
bool vis[MaxQ];

#define Node(q) (&node[q])

void bfs(int s) {
    for(int q = 1; q < tn; ++q) {
        dist[q] = Inf;
        vis[q] = false;
    }
    dist[s] = 0;
    vis[s] = true;

    queue<int> Q;
    Q.push(s);
    while(!Q.empty()) {
        int q = Q.front();
        Q.pop();

        for(int a = 0; a < Alphabet; ++a) {
            int r = delta(Node(q), a) - node;
            if(!Node(r)->bad && !vis[r]) {
                vis[r] = true;
                dist[r] = dist[q] + 1;
                Q.push(r);
            }
        }
    }
}

int n, m;
char s[MaxVirus];

int pos[MaxN];
int adj[MaxN][MaxN];

int dp[MaxN][MaxS];
int pc[MaxS], o[MaxS];

int popcnt(bitvec s) {
    int r = 0;
    do
        r += (s & 1);
    while(s >>= 1);
    return r;
}

bool state_order(bitvec s, bitvec t) {
    return pc[s] < pc[t];
}

void build_adj() {
    for(int i = 0; i <= n; ++i) {
        bfs(pos[i]);
        for(int j = 0; j <= n; ++j) {
            adj[i][j] = dist[pos[j]];
        }
    }
}

void build_dp() {
    for(int i = 0; i <= n; ++i)
        for(bitvec s = 0; s < (1u << n); ++s)
            dp[i][s] = Inf;

    for(bitvec s = 0; s < (1u << n); ++s) {
        o[s] = s;
        pc[s] = popcnt(s);
    }
    sort(o, o + (1u << n), state_order);

    dp[n][0] = 0;
    for(bitvec s = 0; s < (1u << n); ++s) {
        for(int i = 0; i <= n; ++i) {
            if(dp[i][o[s]] < Inf) {
                for(int j = 0; j <= n; ++j) {
                    bitvec t = o[s];
                    if(j < n)
                        t |= (1u << j);
                    dp[j][t] = min(dp[j][t], dp[i][o[s]] + adj[i][j]);
                }
            }
        }
    }
}

int main() {
    while(scanf("%d%d", &n, &m), !(n == 0 && m == 0)) {
        new_trie();
        for(int i = 0; i < n; ++i) {
            scanf("%s", s);
            pos[i] = insert(root, s) - node;
        }
        pos[n] = root - node;
        for(int i = 0; i < m; ++i) {
            scanf("%s", s);
            insert(root, s)->bad = true;
        }
        build_fail();
        build_adj();

        build_dp();
        int r = Inf;
        for(int i = 0; i <= n; ++i)
            r = min(r, dp[i][(1u << n) - 1]);

        printf("%d\n", r);
    }
    return 0;
}
