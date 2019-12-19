#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
using namespace std;

#define WASM_EXPORT extern "C"
#define WASM_IMPORT extern "C"

#define jspointer void *

int main()
{
    printf("C stdout test\n");
    cout << "C++ stdout test" << endl;
    int *p = NULL;
    p = new int;
    *p = 11;
    printf("C++ allocated value: %d\n", *p);
    delete p;

    return 0;
}

WASM_IMPORT void __console_log(jspointer str);

void __console_log(char str[])
{
    __console_log((jspointer)str);
}

void __console_log(int number)
{
    char *str = (char *)malloc(sizeof(char) * 1000);
    sprintf(str, "%d", number);
    __console_log((jspointer)str);
    free(str);
}

void __console_log(unsigned long number)
{
    char *str = (char *)malloc(sizeof(char) * 1000);
    sprintf(str, "%lu", number);
    __console_log((jspointer)str);
    free(str);
}

WASM_EXPORT char *get_memory_for_string(int size)
{
    return (char *)malloc(sizeof(char) * size);
}

WASM_EXPORT void free_memory_for_string(char *str)
{
    free(str);
}

WASM_EXPORT void bridgeTest(char *jsString)
{
    char *finalStr = (char *)malloc(sizeof(char) * 1000);
    strcpy(finalStr, "C unicode test áéíäåöç ");
    __console_log((jspointer)strcat(finalStr, jsString));
    free(finalStr);
}

WASM_EXPORT int times2(int val)
{
    return val * 2;
}

/* BENCHMARKING FUNCTIONS */

unsigned long fib(unsigned long n)
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

WASM_EXPORT int fibBenchmarkULong(unsigned long times, unsigned long fibnumber)
{
    unsigned long acc = 0;
    for (unsigned long i = 0; i < times; i++)
    {
        acc += fib(fibnumber);
    }
    return acc;
}

WASM_EXPORT int fibBenchmarkFloat(unsigned long times, float fibnumber)
{
    float acc = 0;
    for (unsigned long i = 0; i < times; i++)
    {
        acc += fib(fibnumber);
    }
    return acc;
}

WASM_EXPORT int fibBenchmarkDouble(unsigned long times, double fibnumber)
{
    double acc = 0;
    for (unsigned long i = 0; i < times; i++)
    {
        acc += fib(fibnumber);
    }
    return acc;
}

WASM_EXPORT int addingLong(unsigned long times)
{
    long value = 0;
    for (unsigned long i = 0; i < times; i++)
    {
        if (i % 2 == 1)
        {
            value += i;
        }
        else
        {
            value -= i;
        }
    }
    return value;
}

WASM_EXPORT int addingFloat(unsigned long times)
{
    float value = 0;
    for (unsigned long i = 0; i < times; i++)
    {
        if (i % 2 == 1)
        {
            value += i;
        }
        else
        {
            value -= i;
        }
    }
    return value;
}

WASM_EXPORT int addingDouble(unsigned long times)
{
    double value = 0;
    for (unsigned long i = 0; i < times; i++)
    {
        if (i % 2 == 1)
        {
            value += i;
        }
        else
        {
            value -= i;
        }
    }
    return value;
}
