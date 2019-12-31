#include <chrono>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../helper.h"

using namespace std;

unsigned long fib(uint32_t n)
{
    if (n < 2)
    {
        return n;
    }
    return fib(n - 1) + fib(n - 2) + 1;
}

unsigned long fib(uint64_t n)
{
    if (n < 2)
    {
        return n;
    }
    return fib(n - 1) + fib(n - 2) + 1;
}

unsigned long fib(float n)
{
    if (n < 2)
    {
        return n;
    }
    return fib(n - 1) + fib(n - 2) + 1;
}

unsigned long fib(double n)
{
    if (n < 2)
    {
        return n;
    }
    return fib(n - 1) + fib(n - 2) + 1;
}

WASM_EXPORT unsigned long fibBenchmarkUInt32(unsigned long times, int fibnumber)
{
    uint32_t value = 0;
    uint32_t fibn = (uint32_t) fibnumber;
    for (unsigned long i = 0; i < times; i++)
    {
        value = fib(fibn);
    }
    return value;
}

WASM_EXPORT unsigned long fibBenchmarkUInt64(unsigned long times, int fibnumber)
{
    uint64_t value = 0;
    uint64_t fibn = (uint64_t) fibnumber;
    for (unsigned long i = 0; i < times; i++)
    {
        value = fib(fibn);
    }
    return value;
}

WASM_EXPORT unsigned long fibBenchmarkFloat(unsigned long times, int fibnumber)
{
    float value = 0;
    float fibn = (float) fibnumber;
    for (unsigned long i = 0; i < times; i++)
    {
        value = fib(fibn);
    }
    return value;
}

WASM_EXPORT unsigned long fibBenchmarkDouble(unsigned long times, int fibnumber)
{
    double value = 0;
    double fibn = (double) fibnumber;
    for (unsigned long i = 0; i < times; i++)
    {
        value = fib(fibn);
    }
    return value;
}


#ifdef __x86_64
void fibPerformanceTest(char const type[], int runs = 4, unsigned long times = 100000, int fibnumber= 40) {
    unsigned long (*fptr)(unsigned long, int);
    if (strcmp(type, "uint32") == 0) {
        fptr = &fibBenchmarkUInt32;
    } else if (strcmp(type, "uint64") == 0) {
        fptr = &fibBenchmarkUInt64;
    } else if (strcmp(type, "float") == 0) {
        fptr = &fibBenchmarkFloat;
    } else if (strcmp(type, "double") == 0) {
        fptr = &fibBenchmarkDouble;
    } else {
        printf("invalid type!");
        return;
    } 
    for (int i = 0; i < runs; i++) {
        auto t0 = chrono::high_resolution_clock::now();
        unsigned long value = (*fptr)(times, fibnumber);
        auto t1 = chrono::high_resolution_clock::now();
        auto ms = chrono::duration_cast<chrono::milliseconds>(t1-t0).count();
        printf("x86_64 fibonacci %s run: %d, times: %lu, fibnumber: %d, value: %lu, time: %lld ms\n", type, i, times, fibnumber, value, ms);
    }
}
#endif