# Wasmtest

Project for testing wasm features and toolchain, several performance test cases are available.

# Running

There is a pre-built file /public/main.wasm commited in the repository so there is no need to compile it

yarn install

yarn start

# Test Cases

To select which test case to run go to index.ts and at the end of the file uncomment the appropriate test and refresh the page.

Most test cases have a JS and a Wasm implementation for comparison, some have multiple wasm implementations using different C types (long, float, double, etc).

## addingTest()

Simple function that adds and substracts to a variable inside a long-running loop, implemented in JS and in C using unsigned longs, floats and doubles

## bridgeTest()

Testing JS/Wasm communication

## render2dTestJS(threads)

Complex animation written in JS that renders to a 600x600 canvas. This is implemented using webworkers and can be parallelized to improve performance (first parameter is number of threads, defaults to 1)

## render2dTestWasm()

Same as render2dTestJS, but written in C but rendering to a browser canvas by passing a C buffer to it. This version is not multithreaded because wasm doesn't support threads yet

## workerTest()

Spawns multiple JS workers that run an infinite loop that logs every so often to console. Meant to check if the browser can actually run it multi-threaded and to test if the main thread freezes

# Compiling C/C++ code

If you wish to make changes to src/main.cpp code you will need to recompile it. To do that:

1. extract prebuilt clang binaries for your operating system (tested only on macos):

http://releases.llvm.org/download.html

into /clang/

2. extract libclang_rt.builtins-wasm32-wasi-VERSION.tar.gz from:

https://github.com/CraneStation/wasi-sdk/releases

into /clang/lib/9.0.0/lib/wasi/ (create wasi folder if it doesn't exist)

3. extract wasi-sysroot-VERSION-OS.tar.gz from:

https://github.com/CraneStation/wasi-sdk/releases

into /wasi-sysroot

4. yarn run clang

Run `yarn run clang` again on every .cpp file change. No need to restart the dev server. You might want to change the optimization flag (-O2) in the `clang` package.json script to test the performance implications and final .wasm file size

Run `yarn run run-native` to compile the C++ to x64 and run it. Entry point is ./src/main.cpp main() function. The main() will run performance tests against the same functions that are exported from the wasm module. Useful to compare performance between the wasm-compiled code versus the natively-compiled code

# Conclusions

## C types

wasm32:

```
sizeof unsigned int: 4
sizeof unsigned long: 4
sizeof uint8_t : 1
sizeof uint16_t : 2
sizeof uint32_t : 4
sizeof uint64_t : 8
sizeof float: 4
sizeof double: 8
```

x86_64:

```
sizeof unsigned int: 4
sizeof unsigned long: 8
sizeof uint8_t : 1
sizeof uint16_t : 2
sizeof uint32_t : 4
sizeof uint64_t : 8
sizeof float: 4
sizeof double: 8
```

Due to wasm being compiled to 32 bits unsgined long is only 32 bits in memory. But since the wasm file is then interpreted in a browser that can be 64 bits the code could get translated to x64 native instructions even if the toolchain compiles to 32bits. Apparently the only effect of the toolchain compiling to 64 bits is the ability for the memory to exceed 4GB.

Due to these things it seems the more resonable approach is to always use uintX_t types so your code doesn't change in behaviour when the wasm64 toolchain gets released
