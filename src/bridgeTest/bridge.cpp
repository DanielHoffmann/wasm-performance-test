#include <memory>
#include <string.h>
#include <stdlib.h>
#include "../helper.h"

using namespace std;

WASM_EXPORT void bridgeTest(char *jsString)
{
    char finalStr[1024] = "";
    strcpy(finalStr, "C unicode test áéíäåöç ");
    __console_log(strcat(finalStr, jsString));
    free(finalStr);
}

template <typename T>
class MyClass
{
private:
    T prop;

public:
    MyClass(T p)
    {
        prop = p;
    }

    T getProperty()
    {
        return prop;
    }
};

WASM_EXPORT void bridgeTestCppFeatures()
{
    MyClass<int> obj(10);
    __console_log("C++ MyClass<int> obj(10): ");
    __console_log(obj.getProperty());

    MyClass<int> *classPtr = new MyClass<int>(11);
    __console_log("C++ new MyClass<int>(11): ");
    __console_log(classPtr->getProperty());
    delete classPtr;

    unique_ptr<MyClass<double>> classUniquePtr(new MyClass<double>(12.2));
    __console_log("C++ unique_ptr<MyClass<double>>(12.2): ");
    __console_log(classUniquePtr->getProperty());
}

WASM_EXPORT int times2(int val)
{
    return val * 2;
}
