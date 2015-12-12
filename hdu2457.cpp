// http://acm.hdu.edu.cn/showproblem.php?pid=2457
#define PROB "hdu2457"
#include <cstdio>
#include <cstring>
#include <queue>
using namespace std;

const int MaxN = 56;
const int MaxLen = 23;
const int MaxMLen = 1234;
const int MaxT = MaxN * MaxLen;
const int Alphabet = 4;
const int Inf = 1245012;
const int Eof = -1;

struct trie {
    trie *c[Alphabet], *fail;
    bool bad;
};

int tn;
trie node[MaxT], *root, *nil;

void init_trie() {
    tn = 0;
    nil = &node[tn++];
    for(int a = 0; a < Alphabet; ++a)
        nil->c[a] = nil;
    nil->fail = nil;
    nil->bad = false;

    root = &node[tn++];
    *root = *nil;
}

static inline
trie* new_node() {
    trie *r = &node[tn++];
    *r = *nil;
    return r;
}

static inline
trie* next(trie* q, int a) {
    return q->c[a];
}

void insert(trie* q, const int* s) {
    while(*s != Eof) {
        if(next(q, *s) == nil)
            q->c[*s] = new_node();
        q = next(q, *s++);
    }
    q->bad = true;
}

static inline
trie* delta(trie* q, int a) {
//fprintf(stderr, "delta %d %d 'f %d\n", q - node, a, q->fail - node);
    while(q != root && next(q, a) == nil)
        q = q->fail;
    if(next(q, a) != nil)
        q = next(q, a);
//fprintf(stderr, "-> %d\n", q - node);
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
                r->bad |= r->fail->bad;
                Q.push(r);
            }
        }
    }
}

int mlen;
int step[MaxMLen][MaxT];
#define Node(q) (&node[q])

void build_step(const int *s) {
    for(int i = 1; i <= mlen; ++i)
        for(int q = 1; q <= tn; ++q)
            step[i][q] = Node(q)->bad ? Inf : 0;

    for(int i = mlen - 1; i >= 0; --i) {
        for(int q = 1; q < tn; ++q) {
            step[i][q] = Inf;
            if(!Node(q)->bad) {
                for(int a = 0; a < Alphabet; ++a) {
                    int r = delta(Node(q), a) - node;
                    step[i][q] = min(step[i][q], step[i + 1][r] + (a != s[i]));
                }
            }
//            fprintf(stderr, "step %d %d = %d\n", i, q, step[i][q]);
        }
    }
}

static inline
int trans(char a) {
    switch(a) {
      case 'A':
        return 0;
      case 'C':
        return 1;
      case 'G':
        return 2;
      case 'T':
        return 3;
      default:
        return -1;
    }
}

int ztrans(const char *s, int *z) {
    int k = 0;
    for(k = 0; s[k] != '\0'; ++k)
        z[k] = trans(s[k]);
    z[k] = Eof;
    return k;
}

char s[MaxMLen], t[MaxLen];
int zs[MaxMLen], zt[MaxLen];
int n;

int main() {
    int T = 0;
    while(scanf("%d", &n), n > 0) {
        ++T;
        init_trie();
        for(int i = 0; i < n; ++i) {
            scanf("%s", t);
            ztrans(t, zt);
            insert(root, zt);
        }
        build_fail();
    
        scanf("%s", s);
        mlen = ztrans(s, zs);
        build_step(zs);
    
        int r = step[0][root - node];
        if(r == Inf)
            printf("Case %d: -1\n", T);
        else
            printf("Case %d: %d\n", T, r);
    }
    return 0;
}
