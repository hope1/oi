#define PROB "poj2406"
#include <cstdio>
#include <cstring>
#include <algorithm>
using namespace std;

const int MaxN = 1001001;

char s[MaxN];
int n, fail[MaxN];

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
    while(scanf("%s", s + 1)) {
        n = strlen(s + 1);
        if(n == 1 && s[1] == '.')
            break;

        build_fail();
        int r = n - fail[n];
        if(n % r > 0)
            printf("1\n");
        else
            printf("%d\n", n / r);
    }
    return 0;
}
