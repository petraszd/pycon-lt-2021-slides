#include <stdio.h>
#include <stdlib.h>


int main(int argc, char *argv[])
{
    int8_t int_8bit = INT8_MAX;
    int16_t int_16bit = INT16_MAX;
    int32_t int_32bit = INT32_MAX;
    int64_t int_64bit = INT64_MAX;

    printf("  8bit = %d\n", int_8bit);
    printf(" 16bit = %d\n", int_16bit);
    printf(" 32bit = %d\n", int_32bit);
    printf(" 64bit = %lld\n", int_64bit);

    return 0;
}

