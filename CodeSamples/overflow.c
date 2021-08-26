#include <stdio.h>
#include <stdlib.h>


int main(int argc, char *argv[])
{
    int8_t num_8bit = INT8_MAX;
    printf("%d\n", num_8bit);
    num_8bit += 1;
    printf("%d\n", num_8bit);

    int64_t num_64bit = INT64_MAX;
    printf("%lld\n", num_64bit);
    num_64bit += 1;
    printf("%lld\n", num_64bit);

    return 0;
}

