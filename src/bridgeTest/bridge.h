#ifndef BRIDGEAA_H
#define BRIDGEAA_H

#include "../helper.h"

WASM_EXPORT void bridgeTest(char *jsString);

template <typename T>
class MyClass
{
private:
    T prop;

public:
    MyClass(T p);

    T getProperty();
};

WASM_EXPORT void bridgeTestCppFeatures();

WASM_EXPORT int times2(int val);

#endif