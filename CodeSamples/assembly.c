#include <stdlib.h>
#include <stdio.h>


int main(int argc, char *argv[]) {
    int64_t a = 1000;
    int64_t b = 958;
    __asm__("subq %1,%2" :
            "=r" (a) :
            "r" (b), "0" (a));

    printf("%lld\n", a);
    return 0;
}

