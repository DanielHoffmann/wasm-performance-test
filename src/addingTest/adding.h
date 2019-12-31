#ifndef ADDING_H
#define ADDING_H

#include "../helper.h"

WASM_EXPORT long addingUInt8(unsigned long times);

WASM_EXPORT long addingUInt16(unsigned long times);

WASM_EXPORT long addingUInt32(unsigned long times);

WASM_EXPORT long addingUInt64(unsigned long times);

WASM_EXPORT long addingUInt(unsigned long times);

WASM_EXPORT long addingULong(unsigned long times);

WASM_EXPORT long addingFloat(unsigned long times);

WASM_EXPORT long addingDouble(unsigned long times);

#ifdef __x86_64
void addingPerformanceTest(char const type[], int runs = 4, unsigned long times = 800000000);
#endif

#endif