#ifndef HELPER_H
#define HELPER_H

#define jspointer void *
#define WASM_EXPORT extern "C"
#define WASM_IMPORT extern "C"

WASM_EXPORT char *get_memory_for_string(int size);

WASM_EXPORT void free_memory_for_string(char *str);

WASM_EXPORT void __console_log(jspointer str);

void __console_log(char const *str);

void __console_log(int number);

void __console_log(unsigned long number);

void __console_log(float number);

void __console_log(double number);

#endif