// http://www.lydsy.com/JudgeOnline/problem.php?id=1500
// Splay
#define PROB "bzoj1500" 
#include <cassert>
#include <ctime>
#include <cstdio>
#include <algorithm>
#include <vector>
#include <climits>
#include <cstring>
using namespace std;

#ifdef Mdebug
#   define Mprint(...) fprintf(stderr, __VA_ARGS__)
#else
#   define Mprint(...) 0
#endif

struct node {
    node *c[2], *p;
    int size, a, sum, pre, post, msum, max;
    int lza;
    bool flza, frev; 
};

const int MaxN = 500100;
int tn;
node Pool[MaxN], *empty, *nil;
vector<node*> recycle_bin;

#define lc c[0]
#define rc c[1]

#define I(q) ((q) - Pool)

#ifdef Mdebug
static inline
    void print_node(node *x) {
        Mprint("%d c %d %d p %d s %d ; %d sum %d pre %d post %d msum %d max %d ; lza %d %d rev %d\n"
    , I(x), I(x->lc), I(x->rc), I(x->p), x->size, x->a, x->sum, x->pre, x->post, x->msum, x->max, (int)x->flza, x->lza, (int)x->frev);
    }
    
    void print_spaly_rec(node *x) {
        if(x != nil) {
            print_spaly_rec(x->lc);
            print_node(x);
            print_spaly_rec(x->rc);
        }
    }
    void print_spaly(node *x) {
        Mprint("$: ");
        print_node(empty);
        Mprint("nil: ");
        print_node(nil);
        print_spaly_rec(x);
    }
#else
#   define print_spaly(x) ((void)0)
#endif

void init_spaly() {
    tn = 0;
    empty = &Pool[tn++];
    nil = &Pool[tn++];
    empty->lc = nil;
    empty->rc = nil;
    empty->p = nil;
    empty->size = 0;
    empty->a = 0;
    empty->sum = 0;
    empty->pre = 0;
    empty->post = 0;
    empty->msum = 0;
    empty->max = INT_MIN;
    empty->lza = 0;
    empty->flza = false;
    empty->frev = false;
    *nil = *empty;
}

static inline
node* new_node() {
    node *x;
    if(!recycle_bin.empty()) {
        x = recycle_bin.back();
        recycle_bin.pop_back();
        if(x->lc != nil) recycle_bin.push_back(x->lc);
        if(x->rc != nil) recycle_bin.push_back(x->rc);
    } else
        x = &Pool[tn++];

    *x = *empty;
    x->size = 1;
    return x;
}

static inline
void mark_lza(node *x, int v) {
    if(x == nil)
        return;

    x->flza = true;
    x->lza = v;

    x->a = x->lza;
    x->max = x->a;
    x->sum = x->a * x->size;

    int k = (x->a < 0 ? 0 : x->sum);
    x->pre = k;
    x->post = k;
    x->msum = k;
}

static inline
void mark_rev(node *x) {
    if(x == nil)
        return;

    x->frev ^= true;
    swap(x->lc, x->rc);
    swap(x->pre, x->post);
}

static inline
void lazy(node* x) {
    if(x == nil)
        return;

    if(x->flza) {
        mark_lza(x->lc, x->lza);
        mark_lza(x->rc, x->lza);
        x->flza = false;
    }

    if(x->frev) {
        mark_rev(x->lc);
        mark_rev(x->rc);
        x->frev = false;
    }
}

static inline
void maintain(node* x) {
    if(x == nil)
        return;

    assert(!x->flza && !x->frev);

    x->size = x->lc->size + 1 + x->rc->size;
    x->sum = x->lc->sum + x->a + x->rc->sum;
    x->pre = max(x->lc->pre, x->lc->sum + x->a + x->rc->pre);
    x->post = max(x->rc->post, x->rc->sum + x->a + x->lc->post);
    x->msum = max(x->lc->post + x->a + x->rc->pre, max(x->lc->msum, x->rc->msum));
    x->max = max(x->a, max(x->lc->max, x->rc->max));
}

static inline
int dir(node* x) {
    return x->p->c[0] == x ? 0 : 1;
}

static inline
void rotate(node* x) {
    node *p = x->p;
    lazy(p);
    lazy(x);

    int k = dir(x);
    node *beta = x->c[k ^ 1];
    beta->p = p;
    p->c[k] = beta;

    p->p->c[dir(p)] = x;
    x->p = p->p;
    p->p = x;
    x->c[k ^ 1] = p;

    maintain(p);
    maintain(x);
}

static inline
void splay(node* x, node* z) {
    if(x == z)
        return;

    lazy(x);
    while(x->p != z) {
        if(x->p->p == z) {
            rotate(x);
        } else {
            lazy(x->p->p); // frev only modifies first level subtree order
            lazy(x->p);
            if(dir(x) == dir(x->p)) {
                rotate(x->p);
                rotate(x);
            } else {
                rotate(x);
                rotate(x);
            }
        }
    }
}

struct cut2 {
    node *x, *y;
};

struct cut3 {
    node *x, *y, *z;
};

// [0, k]
cut2 split2(node* x, int k) {
    if(x == nil)
        return (cut2){nil, nil};
    if(k < 0)
        return (cut2){nil, x};

    node *z = x->p;
    while(true) {
        lazy(x);
        if(x->lc->size == k)
            break;
        if(k < x->lc->size)
            x = x->lc;
        else {
            k -= x->lc->size + 1;
            x = x->rc;
        }
    }
    splay(x, z);

    node *r = x->rc;
    x->rc = nil;
    r->p = nil;
    maintain(x);
    return (cut2){x, r};
}

node* maximum(node *x) {
    node *z = x->p;
    lazy(x);
    while(x->rc != nil) {
        x = x->rc;
        lazy(x);
    }
    splay(x, z);
    return x;
}

node* merge2(const cut2& c) {
    if(c.x == nil)
        return c.y;
    if(c.y == nil)
        return c.x;

    node *r = maximum(c.x);
    splay(r, nil);
    splay(c.y, nil);
    r->rc = c.y;
    c.y->p = r;
    maintain(r);
    return r;
}

static inline
cut3 split3(node* x, int l, int r) {
    cut2 p = split2(x, l - 1);
    cut2 q = split2(p.y, r - l);
    return (cut3){p.x, q.x, q.y};
}

static inline
node* merge3(const cut3& c) {
    return merge2((cut2){c.x, merge2((cut2){c.y, c.z})});
}

node* build_binary(int* A, int k) {
    if(k == 0)
        return nil;
    else {
        node *x = new_node();
        if(k == 1) {
            mark_lza(x, *A);
            lazy(x);
        } else {
            x->lc = build_binary(A, k / 2);
            x->lc->p = x;

            x->a = A[k / 2];
            x->rc = build_binary(A + k / 2 + 1, k - k / 2 - 1);
            x->rc->p = x;
            maintain(x);
        }
        return x;
    }
}

int n, m;
int A[MaxN];
node *root;

void op_insert(int p, int* A, int t) {
    cut2 r = split2(root, p);
    node *a = build_binary(A, t);
    root = merge3((cut3){r.x, a, r.y});
}

void op_delete(int p, int t) {
    cut3 r = split3(root, p, p + t - 1);
    recycle_bin.push_back(r.y);
    root = merge2((cut2){r.x, r.z});
}

void op_make_same(int p, int t, int v) {
    cut3 r = split3(root, p, p + t - 1);
    mark_lza(r.y, v);
    root = merge3(r);
}

void op_reverse(int p, int t) {
    cut3 r = split3(root, p, p + t - 1);
    mark_rev(r.y);
    root = merge3(r);
}

int op_get_sum(int p, int t) {
    cut3 r = split3(root, p, p + t - 1);
    int ans = r.y->sum;
    root = merge3(r);
    return ans;
}

int op_max_sum() {
    cut3 r = split3(root, 1, n);
    int ans = r.y->max < 0 ? r.y->max : r.y->msum;
    root = merge3(r);
    return ans;
}

int main() {
    init_spaly();
    root = nil;
    op_insert(-1, A, 2);

    scanf("%d%d", &n, &m);
    for(int i = 0; i < n; ++i)
        scanf("%d", &A[i]);
    op_insert(0, A, n);

    for(int i = 0; i < m; ++i) {
        char op[20];
        scanf("%s", op);

        if(!strcmp(op, "INSERT")) {
            int pos, tot;
            scanf("%d%d", &pos, &tot);
            for(int i = 0; i < tot; ++i)
                scanf("%d", &A[i]);
            op_insert(pos, A, tot);
            n += tot;
        }

        if(!strcmp(op, "DELETE")) {
            int pos, tot;
            scanf("%d%d", &pos, &tot);
            op_delete(pos, tot);
            n -= tot;
        }

        if(!strcmp(op, "MAKE-SAME")) {
            int pos, tot, c;
            scanf("%d%d%d", &pos, &tot, &c);
            op_make_same(pos, tot, c);
        }

        if(!strcmp(op, "REVERSE")) {
            int pos, tot;
            scanf("%d%d", &pos, &tot);
            op_reverse(pos, tot);
        }

        if(!strcmp(op, "GET-SUM")) {
            int pos, tot;
            scanf("%d%d", &pos, &tot);

            int ans = op_get_sum(pos, tot);
            printf("%d\n", ans);
        }

        if(!strcmp(op, "MAX-SUM")) {
            int ans = op_max_sum();
            printf("%d\n", ans);
        }
    }

    return 0;
}
