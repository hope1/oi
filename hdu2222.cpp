http://acm.hdu.edu.cn/showproblem.php?pid=2222
#define PROB "hdu2222"
#include <cstdio>
#include <cstring>
#include <cassert>
#include <algorithm>
#include <queue>
using namespace std;

const int MaxLen = 1001001;
const int MaxKey = 123;
const int MaxN = 10100;
const int MaxT = MaxKey * MaxN;
const int Alphabet = 26;
const char Base = 'a';

struct trie {
    trie *c[Alphabet], *fail;
    int terminal;
};

int tn;
trie node[MaxT], *root, *nil;

void init_trie() {
    tn = 0;
    nil = &node[tn++];
    for(int a = 0; a < Alphabet; ++a)
        nil->c[a] = nil;
    nil->fail = nil;
    nil->terminal = 0;

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
    assert(a < Alphabet && a >= 0);
    return q->c[a];
}

void insert(trie* q, const int* s) {
    while(*s != -1) {
        if(next(q, *s) == nil)
            q->c[*s] = new_node();
        q = next(q, *s++);
    }
    ++q->terminal;
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
                Q.push(r);
            }
        }
    }
}

// state q matches all string on its path to root
int clear_path(trie* q) {
    int r = 0;
    for(; q != root; q = q->fail) {
        r += q->terminal;
        q->terminal = 0;
    }
    return r;
}

int match(const int *s) {
    int r = 0;
    trie *q = root;

    r += q->terminal;
    q->terminal = 0;
    while(*s != -1) {
        q = delta(q, *s++);
        r += clear_path(q);
    }
    return r;
}

char s[MaxLen], sk[MaxLen];
int zs[MaxLen], zsk[MaxLen];
int n;

int main() {
    int T;
    scanf("%d\n", &T);
    while(T --> 0) {
        init_trie();

        scanf("%d", &n);
        for(int i = 0; i < n; ++i) {
            scanf("%s", &sk);
            int k = 0;
            for(; sk[k] != '\0'; ++k)
                zsk[k] = sk[k] - 'a';
            zsk[k] = -1;
            insert(root, zsk);
        }
        build_fail();

        scanf("%s", &s);
        int k = 0;
        for(; s[k] != '\0'; ++k)
            zs[k] = s[k] - 'a';
        zs[k] = -1;
        int r = match(zs);
        printf("%d\n", r);

    }
    return 0;
}
