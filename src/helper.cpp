#include <stdlib.h>
#include <stdio.h>

#define jspointer void *
#define WASM_EXPORT extern "C"
#define WASM_IMPORT extern "C"

#ifdef __x86_64
void __console_log(jspointer str) {
    printf("%s", (char *)str);
}
#else
WASM_IMPORT void __console_log(jspointer str);
#endif

WASM_EXPORT char *get_memory_for_string(int size)
{
    return (char *)malloc(sizeof(char) * size);
}

WASM_EXPORT void free_memory_for_string(char *str)
{
    free(str);
}


void __console_log(char const *str)
{
    __console_log((jspointer)str);
}

void __console_log(int number)
{
    char str[1024] = "";
    sprintf(str, "%d", number);
    __console_log((jspointer)str);
}

void __console_log(unsigned long number)
{
    char str[1024] = "";
    sprintf(str, "%lu", number);
    __console_log((jspointer)str);
}

void __console_log(float number)
{
    char str[1024] = "";
    sprintf(str, "%f", number);
    __console_log((jspointer)str);
}

void __console_log(double number)
{
    char str[1024] = "";
    sprintf(str, "%f", number);
    __console_log((jspointer)str);
}