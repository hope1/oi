#define PROB "hdu3746"
#include <cstdio>
#include <cstring>
using namespace std;

const int MaxN = 100100;
const int MaxT = 107;

char s[MaxN];
int n, fail[MaxN];

int delta(int q, char a) {
    while(q > 0 && s[q + 1] != a)
        q = fail[q];
    if(s[q + 1] == a)
        q = q + 1;
    return q;
}

void build_fail() {
    fail[0] = 0;
    fail[1] = 0;
    for(int i = 2; i <= n; ++i)
        fail[i] = delta(fail[i - 1], s[i]);
}

int main() {
    int T;
    scanf("%d", &T);
    while(T --> 0) {
        s[0] = 0;
        scanf("%s", s + 1);
        n = strlen(s + 1);
        build_fail();

        int r = n - fail[n];
        int t = n % r;
        if(t == 0 && r < n)
            printf("0\n");
        else
            printf("%d\n", r - t);
    }
    return 0;
}
