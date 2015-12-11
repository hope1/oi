#define PROB "poj2752"
#include <cstdio>
#include <cstring>
using namespace std;

const int MaxN = 400100;
char s[MaxN];
int n, fail[MaxN];
int out[MaxN], outp;

int delta(int q, char a) {
    while(q > 0 && s[q + 1] != a)
        q = fail[q];
    if(s[q + 1] == a)
        ++q;
    return q;
}

void build_fail() {
    fail[0] = 0;
    fail[1] = 0;
    for(int i = 2; i <= n; ++i)
        fail[i] = delta(fail[i - 1], s[i]);
}

int main() {
    while(scanf("%s", s + 1) != EOF) {
        n = strlen(s + 1);
        build_fail();

        outp = 0;
        int q = n;
        do {
            out[++outp] = q;
            q = fail[q];
        } while(q > 0);

        for(int i = outp; i > 0; --i)
            printf("%d%s", out[i], i == 1 ? "\n" : " ");
    }
    return 0;
}
