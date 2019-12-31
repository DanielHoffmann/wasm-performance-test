#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "helper.h"
#include "./addingTest/adding.h"
#include "./fibTest/fib.h"
#include "./bridgeTest/bridge.h"
#include "./render2dTest/render2d.h"

using namespace std;

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
        // seems clang, unlike JS, can optimize the recursive calls into iterative calls somehow
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
