#ifndef FIB_H
#define FIB_H

#include "../helper.h"

unsigned long fib(uint32_t n);

unsigned long fib(uint64_t n);

unsigned long fib(float n);

unsigned long fib(double n);

WASM_EXPORT unsigned long fibBenchmarkUInt32(unsigned long times, int fibnumber);

WASM_EXPORT unsigned long fibBenchmarkUInt64(unsigned long times, int fibnumber);

WASM_EXPORT unsigned long fibBenchmarkFloat(unsigned long times, int fibnumber);

WASM_EXPORT unsigned long fibBenchmarkDouble(unsigned long times, int fibnumber);


#ifdef __x86_64
void fibPerformanceTest(char const type[], int runs = 4, unsigned long times = 100000, int fibnumber= 40);
#endif


#endif