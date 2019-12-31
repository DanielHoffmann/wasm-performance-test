#include <chrono>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../helper.h"

using namespace std;

WASM_EXPORT long addingUInt8(unsigned long times)
{
    uint8_t value = 0;
    // uint8_t is too small to hold times
    // so this benchmark doesn't really demonstrates the performance difference of uint8
    for (uint8_t i = 0; i < times; i++)
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
    return (unsigned long) value;
}

WASM_EXPORT long addingUInt16(unsigned long times)
{
    uint16_t value = 0;
    // uint16_t is too small to hold times
    // so this benchmark doesn't really demonstrates the performance difference of uint16
    for (uint16_t i = 0; i < times; i++)
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
    return (unsigned long) value;
}

WASM_EXPORT long addingUInt32(unsigned long times)
{
    uint32_t value = 0;
    uint32_t t = (uint32_t) times;
    for (uint32_t i = 0; i < t; i++)
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
    return (unsigned long) value;
}

WASM_EXPORT long addingUInt64(unsigned long times)
{
    uint64_t value = 0;
    uint64_t t = (uint64_t) times;
    for (uint64_t i = 0; i < t; i++)
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
    return (unsigned long) value;
}

WASM_EXPORT long addingUInt(unsigned long times)
{
    unsigned int value = 0;
    unsigned int t = (unsigned int) times;
    for (unsigned int i = 0; i < t; i++)
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
    return (unsigned long) value;
}

WASM_EXPORT long addingULong(unsigned long times)
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

WASM_EXPORT long addingFloat(unsigned long times)
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

WASM_EXPORT long addingDouble(unsigned long times)
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

#ifdef __x86_64
void addingPerformanceTest(char const type[], int runs = 4, unsigned long times = 800000000) {
    long (*fptr)(unsigned long);
    if (strcmp(type, "uint") == 0) {
        fptr = &addingUInt;
    } else if (strcmp(type, "ulong") == 0) {
        fptr = &addingULong;
    } else if (strcmp(type, "uint8") == 0) {
        fptr = &addingUInt8;
    } else if (strcmp(type, "uint16") == 0) {
        fptr = &addingUInt16;
    } else if (strcmp(type, "uint32") == 0) {
        fptr = &addingUInt32;
    } else if (strcmp(type, "uint64") == 0) {
        fptr = &addingUInt64;
    } else if (strcmp(type, "float") == 0) {
        fptr = &addingFloat;
    } else if (strcmp(type, "double") == 0) {
        fptr = &addingDouble;
    } else {
        printf("invalid type!\n");
        return;
    }
    for (int i = 0; i < runs; i++) {
        auto t0 = chrono::high_resolution_clock::now();
        int value = (*fptr)(times);
        auto t1 = chrono::high_resolution_clock::now();
        auto ms = chrono::duration_cast<chrono::milliseconds>(t1-t0).count();
        printf("x86_64 adding %s run: %d, times: %lu, value: %d, time: %lld ms\n", type, i, times, value, ms);
    }
}

#endif