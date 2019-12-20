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
    // using cout increates bundle size by over 900kb
    // cout << "C++ stdout test" << endl;
    return 0;
}

WASM_IMPORT void __console_log(jspointer str);

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
