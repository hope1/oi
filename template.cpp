#include <cstdio>
#include <iostream>
using namespace std;
 
#define PROB "PROB"
void redirect(void) {
#ifndef ONLINE_JUDGE
    freopen(PROB ".in", "r", stdin);
    freopen(PROB ".out", "w", stdout);
#endif
    ios::sync_with_stdio(false);
}
 
#ifdef Mdebug
#   define Mdprint(...) fprintf(stderr, __VA_ARGS__)
#else
#   define Mdprint(...) 0
#endif
 
int main(void) {
    redirect();

    return 0;
}
