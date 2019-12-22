#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <limits.h>
using namespace std;

#define WASM_EXPORT extern "C"
#define WASM_IMPORT extern "C"

#define jspointer void *

#ifdef __x86_64
void __console_log(jspointer str) {
    printf("%s", (char *)str);
}
#else
WASM_IMPORT void __console_log(jspointer str);
#endif

void __console_log(char const *str)
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

void __console_log(float number)
{
    char *str = (char *)malloc(sizeof(char) * 1000);
    sprintf(str, "%f", number);
    __console_log((jspointer)str);
    free(str);
}

void __console_log(double number)
{
    char *str = (char *)malloc(sizeof(char) * 1000);
    sprintf(str, "%f", number);
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

/* 2d CANVAS RENDERING FUNCTIONS */
#define PI 3.14159265358979323846
#define RAD 6.283185307179586
#define COEFF_1 0.7853981633974483
#define COEFF_2 2.356194490192345
#define BLADES 3
#define CYCLE_WIDTH 100
#define BLADES_T_CYCLE_WIDTH 300

int d2width;
int d2height;
int d2pixelCount;
int d2ch;
int d2cw;
double d2maxDistance;

int *d2buf;

WASM_EXPORT jspointer init2d(int cwidth, int cheight)
{
    d2width = cwidth;
    d2height = cheight;
    d2pixelCount = d2width * d2height;
    d2ch = d2height >> 1;
    d2cw = d2width >> 1;
    d2maxDistance = sqrt(d2ch * d2ch + d2cw * d2cw);
    free(d2buf);
    d2buf = (int *)malloc(sizeof(int) * d2width * d2height * 100);
    return &d2buf[0];
}

double customAtan2(int y, int x)
{
    double abs_y = abs(y) + 1e-10;
    double angle;
    if (x >= 0)
    {
        double r = (x - abs_y) / (x + abs_y);
        angle = 0.1963 * r * r * r - 0.9817 * r + COEFF_1;
    }
    else
    {
        double r = (x + abs_y) / (abs_y - x);
        angle = 0.1963 * r * r * r - 0.9817 * r + COEFF_2;
    }
    return y < 0 ? -angle : angle;
}

WASM_EXPORT void render2d(double timestamp)
{
    // random static algorithm:
    // for (int y = 0; y < d2height; y++)
    // {
    //     int yw = y * d2width;
    //     for (int x = 0; x < d2width; x++)
    //     {

    //         d2buf[yw + x] =
    //             ((rand() % 255) << 24) | // A
    //             ((rand() % 255) << 16) | // B
    //             ((rand() % 255) << 8) |  // G
    //             1;                       // R
    //     }
    // }
    int scaledTimestamp = floor(timestamp / 10.0 + 2000.0);
    for (int y = 0; y < d2height; y++)
    {
        int dy = d2ch - y;
        int dysq = dy * dy;
        int yw = y * d2width;
        for (int x = 0; x < d2width; x++)
        {
            int dx = d2cw - x;
            int dxsq = dx * dx;
            double angle = customAtan2(dx, dy) / RAD;
            int asbs = dxsq + dysq;
            double distanceFromCenter = sqrt(asbs);
            double scaledDistance = asbs / 400.0 + distanceFromCenter;
            double lerp = 1.0 - (fmod(fabs(scaledDistance - scaledTimestamp + angle * BLADES_T_CYCLE_WIDTH), CYCLE_WIDTH)) / CYCLE_WIDTH;
            double absoluteDistanceRatioGB = 1.0 - distanceFromCenter / d2maxDistance;
            double absoluteDistanceRatioR = absoluteDistanceRatioGB * 0.8 + 0.2;
            int fadeB = round(50.0 * lerp * absoluteDistanceRatioGB);
            int fadeR = round(240.0 * lerp * absoluteDistanceRatioR * (1.0 + lerp) / 2.0);
            int fadeG = round(120.0 * lerp * lerp * lerp * absoluteDistanceRatioGB);

            d2buf[yw + x] =
                (255 << 24) |   // A
                (fadeB << 16) | // B
                (fadeG << 8) |  // G
                fadeR;          // R
        }
    }
}

/* BENCHMARKING FUNCTIONS */

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
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(t1-t0).count();
        printf("x86_64 adding %s run: %d, times: %lu, value: %d, time: %lld ms\n", type, i, times, value, ms);
    }
}

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
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(t1-t0).count();
        printf("x86_64 fibonacci %s run: %d, times: %lu, fibnumber: %d, value: %lu, time: %lld ms\n", type, i, times, fibnumber, value, ms);
    }
}

// native performance comparison functions
void render2dPerformanceTest(int resolution= 600, int runs=4, int frames=500) {
    for (int i = 0; i < runs; i++) {
        init2d(resolution, resolution);
        auto t0 = chrono::high_resolution_clock::now();
        for (int j = 0; j < frames; j++) {
            render2d((double)(j*1000.0));
        }
        auto t1 = chrono::high_resolution_clock::now();
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(t1-t0).count();
        double fps =  ((double)(1000 * frames) / (double)ms);
        printf("x86_64 render2d %d x %d, run: %d, frames: %d, time: %lld ms, fps: %f \n", resolution, resolution, i, frames, ms, fps);
    }
}
#endif

int main()
{
    printf("UINT_MAX: %u ULONG_MAX: %lu\n", UINT_MAX, ULONG_MAX);
    printf("sizeof unsigned int: %lu\n", sizeof(unsigned int));
    printf("sizeof unsigned long: %lu\n", sizeof(unsigned long));
    printf("sizeof uint8_t : %lu\n", sizeof(uint8_t));
    printf("sizeof uint16_t : %lu\n", sizeof(uint16_t));
    printf("sizeof uint32_t : %lu\n", sizeof(uint32_t));
    printf("sizeof uint64_t : %lu\n", sizeof(uint64_t));
    printf("sizeof float: %lu\n", sizeof(float));
    printf("sizeof double: %lu\n", sizeof(double));
    #ifdef __x86_64
        // // native compilation performance frames
        addingPerformanceTest("uint");
        addingPerformanceTest("ulong");
        // uint8 and uint16 are taking too long possibly due to overflowing
        addingPerformanceTest("uint32");
        addingPerformanceTest("uint64");
        addingPerformanceTest("float");
        addingPerformanceTest("double");
        // the fib tests are a lot faster than the other ones
        // seems, unlike JS, clang can optimize the recursive calls into iterative calls somehow
        // using -O0 makes them slower than the adding performance test
        fibPerformanceTest("uint32");
        fibPerformanceTest("uint64");
        fibPerformanceTest("float");
        fibPerformanceTest("double");
        render2dPerformanceTest();
    #else
        printf("C stdout test\n");
        // using cout increases bundle size by over 900kb for some reason
        // cout << "C++ stdout test" << endl;
    #endif
    return 0;
}
