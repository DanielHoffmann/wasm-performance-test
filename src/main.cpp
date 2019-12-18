#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define WASM_EXPORT __attribute__((visibility("default"))) extern "C"
#define WASM_IMPORT extern "C"

#define jspointer int

int main()
{
    printf("This is C's stdout!\n");
    return 0;
}

WASM_IMPORT void __console_log(jspointer str);

WASM_EXPORT char *get_memory_for_string(int size)
{
    return (char *)malloc(sizeof(char) * size);
}

WASM_EXPORT void free_memory_for_string(char *str)
{
    free(str);
}

WASM_EXPORT void test()
{
    __console_log((jspointer) "This is C calling JS!");
}

WASM_EXPORT int intTimes2(int val)
{
    return val * 2;
}

WASM_EXPORT unsigned long uLongTimes2(unsigned long val)
{
    return val * 2;
}

unsigned long fib(unsigned long n, unsigned long nMinus1, int idx)
{
    if (idx <= 2)
    {
        return n;
    }
    return fib(n + nMinus1, n, idx - 1);
}

WASM_EXPORT int fibBenchmark(unsigned long times, int fibnumber)
{
    int value = 0;
    for (unsigned long i = 0; i < times; i++)
    {
        value = fib(1, 1, fibnumber);
    }
    return value;
}
