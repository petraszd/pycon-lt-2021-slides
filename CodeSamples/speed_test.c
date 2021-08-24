#include <stdio.h>


int main(int argc, char *argv[])
{
    int64_t i = 0;
    int64_t res = 0;
    while(i < 1e9) {
        if (i % 2 == 0) {
            res += i;
        } else {
            res -= i;
        }
        i++;
    }
    printf("%lld\n", res);
    return 0;
}
