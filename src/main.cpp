#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define WASM_EXPORT __attribute__((visibility("default"))) extern "C"
#define WASM_IMPORT extern "C"

#define jspointer int

int main()
{
    printf("This is C's stdout!\n");
    return 0;
}

WASM_IMPORT void __console_log(jspointer str);

WASM_EXPORT char *get_memory_for_string(int size)
{
    return (char *)(sizeof(char) * size);
}

WASM_EXPORT void free_memory_for_string(char *str)
{
    free(str);
}

WASM_EXPORT void test()
{
    __console_log((jspointer) "This is C calling JS!");
}

WASM_EXPORT int times2(int val)
{
    return val * 2;
}
