// http://www.lydsy.com/JudgeOnline/problem.php?id=2434
// NOI 2011 typewriter
#define PROB "bzoj2434"
#include <cstdio>
#include <cstring>
#include <algorithm>
#include <queue>
#include <vector>
using namespace std;

const int MaxN = 123456;
const int Alphabet = 26;

struct trie {
    trie *c[Alphabet], *p, *fail;
    vector<trie*> fc;
};

int tn;
trie node[MaxN], *nil, *root;

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
    nil->p = nil;
    nil->fail = nil;

    root = new_node();
}

#define next(q, a) ((q)->c[a])

trie* delta(trie* q, int a) {
    while(q != root && next(q, a) == nil)
        q = q->fail;
    if(next(q, a) != nil)
        q = next(q, a);
    return q;
}

void build_fail() {
    queue<trie*> Q;
    Q.push(root);
    root->fail = root;
    root->p = root;

    while(!Q.empty()) {
        trie *q = Q.front();
        Q.pop();

        for(int a = 0; a < Alphabet; ++a) {
            trie *r = next(q, a);
            if(r != nil) {
                Q.push(r);

                r->p = q;
                if(q == root)
                    r->fail = root;
                else
                    r->fail = delta(q->fail, a);
                r->fail->fc.push_back(r);
            }
        }
    }
}

#define I(q) ((q) - node)
#define N(q) (&node[q])
int subl[MaxN], subr[MaxN], tick;

void dfs(trie* q) {
    ++tick;
    subl[I(q)] = tick;
    for(vector<trie*>::iterator it = q->fc.begin(); it != q->fc.end(); ++it)
        dfs(*it);
    subr[I(q)] = tick;
}

unsigned lowbit(unsigned k) {
    return k - (k & (k - 1));
}

int bit[MaxN];

int bit_query(int p) {
    int ans = 0;
    for(; p > 0; p -= lowbit(p))
        ans += bit[p];
    return ans;
}

void bit_modify(int p, int v) {
    for(; p < tn; p += lowbit(p))
        bit[p] += v;
}

const int MaxM = 123456;
struct query {
    int x, y;
    int ans;
};

char s[MaxN];
int ln, line[MaxN];

int m;
query Q[MaxM];
vector<int> qline[MaxN];

void iwalk(const char* s) {
    trie *q = root;
    while(*s != '\0') {
        if(*s == 'B') {
            q = q->p;
        } else if(*s == 'P') {
            line[++ln] = I(q);
        } else {
            trie *&r = next(q, *s - 'a');
            if(r == nil) {
                r = new_node();
                r->p = q;
            }
            q = r;
        }
        ++s;
    }
}

/*
    The number of occurances of `x' in `y' is equivalent to the number
    of nodes on the trie-path from root to `y' that are descendants of
    `x' in the fail-tree of the AC automaton.

    To find the above information for each query, maintain a marking of
    the nodes on the current prefix (path to root), so for query `(x, y)'
    the answer is the number of marked nodes in the fail-subtree of `x'
    when the current state is at `y'. Mark the nodes on the DFS order
    sequence of the fail-tree to allow for fast subtree queries.
*/
void qwalk(const char* s) {
    trie *q = root;
    int y = 0;
    while(*s != '\0') {
        if(*s == 'P') {
            ++y;
            for(vector<int>::iterator it = qline[y].begin(); it != qline[y].end(); ++it) {
                int r = line[Q[*it].x];
                Q[*it].ans = bit_query(subr[r]) - bit_query(subl[r] - 1);
            }
        } else if(*s == 'B') {
            bit_modify(subl[I(q)], -1);
            q = q->p;
        } else {
            q = next(q, *s - 'a');
            bit_modify(subl[I(q)], 1);
        }
        ++s;
    }
}

int main() {
    scanf("%s%d", s, &m);
    new_trie();
    iwalk(s);

    build_fail();
    tick = 0;
    dfs(root);

    for(int i = 0; i < m; ++i) {
        scanf("%d%d", &Q[i].x, &Q[i].y);
        qline[Q[i].y].push_back(i);
    }
    qwalk(s);

    for(int i = 0; i < m; ++i)
        printf("%d\n", Q[i].ans);

    return 0;
}
