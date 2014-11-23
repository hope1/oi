#include <cstdio>
#include <iostream>
#include <cassert>
using namespace std;

#define PROB "rps"
void redirect() {
    freopen(PROB ".in", "r", stdin);
    freopen(PROB ".out", "w", stdout);
    ios::sync_with_stdio(false);
}

static const int tab[5][5] = {
    { 0, -1, 1, 1, -1 },
    { 1, 0, -1, 1, -1 },
    { -1, 1, 0, -1, 1 },
    { -1, -1, 1, 0, 1 },
    { 1, 1, -1, -1, 0 }
};

void validate() {
#ifdef Mdebug
    for(int i = 0; i < 5; ++i)
    for(int j = 0; j < 5; ++j)
        assert(tab[i][j] == -tab[j][i]);
#endif
}

#define MAX_N 200
int N, NA, NB, TA[200], TB[200];

int main() {
    validate();
    redirect();

    cin >> N >> NA >> NB;
    for(int i = 0; i < NA; ++i)
        cin >> TA[i];
    for(int i = 0; i < NB; ++i)
        cin >> TB[i];

    int SA = 0, SB = 0;
    for(int i = 0; i < N; ++i) {
        int r = tab[TA[i % NA]][TB[i % NB]];
        if(r > 0)
            ++SA;
        if(r < 0)
            ++SB;
    }

    cout << SA << " " << SB << endl;
    return 0;
}
