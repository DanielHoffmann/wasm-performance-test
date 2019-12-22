# Wasmtest

Project for testing wasm features and toolchain, several performance test cases are available. Uses clang, wasi-lib and wasmer/wasi

# Running

There is a pre-built file /public/main.wasm commited in the repository so there is no need to compile it

yarn install

yarn start

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

Run `yarn run run-native` to compile the C++ to x64 and run it immediately. Entry point is ./src/main.cpp main() function. The main() will run performance tests against the same functions that are exported from the wasm module. Useful to compare performance between the wasm-compiled code versus the natively-compiled code.

# Test Cases

To select which test case to run go to index.ts and at the end of the file uncomment the appropriate test and refresh the page.

Most test cases have a JS and a Wasm implementation for comparison, some have multiple wasm implementations using different C types (long, float, double, etc).

## addingTest()

Simple function that adds and substracts to a variable inside a long-running loop, implemented in JS and in C using unsigned longs, floats and doubles

## bridgeTest()

Testing JS/Wasm communication

## render2dJS(threads=1)

Complex animation written in JS that renders to a 600x600 canvas. This is implemented using webworkers and can be parallelized to improve performance (first parameter is number of threads, defaults to 1). Uses SharedArrayBuffer to have a shared memory area between the workers to avoid memory copying.

## render2dWasm()

Same as render2dTestJS, but written in C and compiled to Wasm. It renders to a browser canvas by creating C buffer and sending its pointer to Javascript and associating that buffer with the `<canvas>` tag. This version is not multithreaded because WASM doesn't support threads yet.

## workerTest()

Spawns multiple JS workers that run an infinite loop that logs every so often to console. Meant to check if the browser can actually run it multi-threaded and to test if the main thread freezes

# C types

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

Due to wasm being compiled to 32 bits unsigned long is only 32 bits in memory. But since the wasm file is then interpreted in a browser that can be itself 64 bits the code could get translated to x64 native instructions even if the toolchain compiles to 32bits. Apparently in the final WASM instructions the only performance effect of the toolchain compiling to 64 bits is the ability for the memory to exceed 4GB in size. Although the language itself might have different semantics if compiling to 32 bits (for example longs being 32 bits instead of 64 in C).

Due to these things it seems the more resonable approach is to always use uintX_t types so your code doesn't change in behaviour when the wasm64 toolchain gets released. With uint64_t seems to be the best performing for 64bit browsers (see following sections).

# .wasm file size

Adding a simple `std::cout << "Hello World!";` to the .cpp file increases bundle size by over 900kb, while using `printf()` doesn't meaningfully change the size. Apparently `std::cout` imports currency and date formating code as well as localization code and therefor and blow up your .wasm file size.

One must be careful about the libc and libc++ features one uses (especially libc++).

# Performance

All performance tests were made using -O3 optimization flag on an intel processor, macOS and Chrome 64bits

## addingTest()

Results:

```
JS run 0, value: 400000000, time: 1041.5849999990314 ms
JS run 1, value: 400000000, time: 1029.3750000419095 ms
JS run 2, value: 400000000, time: 1495.7200000062585 ms
JS run 3, value: 400000000, time: 1491.2500000209548 ms
Wasm uint run 0, value: 400000000, time: 631.4950000378303 ms
Wasm uint run 1, value: 400000000, time: 635.1400000276044 ms
Wasm uint run 2, value: 400000000, time: 620.7399999839254 ms
Wasm uint run 3, value: 400000000, time: 636.3550000241958 ms
Wasm ulong run 0, value: 400000000, time: 643.2199999690056 ms
Wasm ulong run 1, value: 400000000, time: 666.2099999957718 ms
Wasm ulong run 2, value: 400000000, time: 634.1850000317208 ms
Wasm ulong run 3, value: 400000000, time: 625.5549999768846 ms
Wasm uint32 run 0, value: 400000000, time: 652.2000000113621 ms
Wasm uint32 run 1, value: 400000000, time: 629.735000024084 ms
Wasm uint32 run 2, value: 400000000, time: 617.8899999940768 ms
Wasm uint32 run 3, value: 400000000, time: 644.0200000070035 ms
Wasm uint64 run 0, value: 400000000, time: 581.6749999648891 ms
Wasm uint64 run 1, value: 400000000, time: 573.7300000037067 ms
Wasm uint64 run 2, value: 400000000, time: 581.4099999843165 ms
Wasm uint64 run 3, value: 400000000, time: 574.205000011716 ms
Wasm float run 0, value: 400000000, time: 854.010000010021 ms
Wasm float run 1, value: 400000000, time: 853.6250000470318 ms
Wasm float run 2, value: 400000000, time: 856.6450000507757 ms
Wasm float run 3, value: 400000000, time: 849.1950000170618 ms
Wasm double run 0, value: 400000000, time: 1149.5300000533462 ms
Wasm double run 1, value: 400000000, time: 1150.3549999906681 ms
Wasm double run 2, value: 400000000, time: 1146.3349999976344 ms
Wasm double run 3, value: 400000000, time: 1150.6749999825843 ms
x86_64 adding uint run: 0, times: 800000000, value: 400000000, time: 198 ms
x86_64 adding uint run: 1, times: 800000000, value: 400000000, time: 201 ms
x86_64 adding uint run: 2, times: 800000000, value: 400000000, time: 201 ms
x86_64 adding uint run: 3, times: 800000000, value: 400000000, time: 202 ms
x86_64 adding ulong run: 0, times: 800000000, value: 400000000, time: 67 ms
x86_64 adding ulong run: 1, times: 800000000, value: 400000000, time: 66 ms
x86_64 adding ulong run: 2, times: 800000000, value: 400000000, time: 66 ms
x86_64 adding ulong run: 3, times: 800000000, value: 400000000, time: 67 ms
x86_64 adding uint32 run: 0, times: 800000000, value: 400000000, time: 199 ms
x86_64 adding uint32 run: 1, times: 800000000, value: 400000000, time: 204 ms
x86_64 adding uint32 run: 2, times: 800000000, value: 400000000, time: 199 ms
x86_64 adding uint32 run: 3, times: 800000000, value: 400000000, time: 200 ms
x86_64 adding uint64 run: 0, times: 800000000, value: 400000000, time: 68 ms
x86_64 adding uint64 run: 1, times: 800000000, value: 400000000, time: 67 ms
x86_64 adding uint64 run: 2, times: 800000000, value: 400000000, time: 66 ms
x86_64 adding uint64 run: 3, times: 800000000, value: 400000000, time: 67 ms
x86_64 adding float run: 0, times: 800000000, value: 400000000, time: 767 ms
x86_64 adding float run: 1, times: 800000000, value: 400000000, time: 763 ms
x86_64 adding float run: 2, times: 800000000, value: 400000000, time: 764 ms
x86_64 adding float run: 3, times: 800000000, value: 400000000, time: 784 ms
x86_64 adding double run: 0, times: 800000000, value: 400000000, time: 1015 ms
x86_64 adding double run: 1, times: 800000000, value: 400000000, time: 1018 ms
x86_64 adding double run: 2, times: 800000000, value: 400000000, time: 1016 ms
x86_64 adding double run: 3, times: 800000000, value: 400000000, time: 1020 ms
```

The WASM overall performance is significantly lower than x86_64 and about 20% better than JS for doubles. uint64_t is about x2 faster in wasm than the JS implementation.

For floting points float compared to double is faster in x86_64 (about 25% faster), and wasm (by about 25% as well).

Comparing to JS the 64bits integer types seem to be faster than floating points in both x86_64 (by about x3 times) and wasm (by about 10%).

## fibTest()

This algorithm was implemented recursively and +1 is added to the return to prevent tail call optimization. The WASM implementation is about x500 to x10000 faster than the JS implementation (varies a lot depending on how deep the recursion goes).

Results (no JS results because it is just too slow):

```
Wasm uint32 run 0, times: 100000, fibnumber: 40, value: 267914295, time: 886.5249999798834 ms
Wasm uint32 run 1, times: 100000, fibnumber: 40, value: 267914295, time: 877.6150000048801 ms
Wasm uint32 run 2, times: 100000, fibnumber: 40, value: 267914295, time: 877.4299999931827 ms
Wasm uint32 run 3, times: 100000, fibnumber: 40, value: 267914295, time: 871.190000034403 ms
Wasm uint64 run 0, times: 100000, fibnumber: 40, value: 267914295, time: 879.2649999959394 ms
Wasm uint64 run 1, times: 100000, fibnumber: 40, value: 267914295, time: 883.3400000003166 ms
Wasm uint64 run 2, times: 100000, fibnumber: 40, value: 267914295, time: 881.2850000103936 ms
Wasm uint64 run 3, times: 100000, fibnumber: 40, value: 267914295, time: 882.4500000337139 ms
Wasm float run 0, times: 100000, fibnumber: 40, value: 267914288, time: 1302.7749999891967 ms
Wasm float run 1, times: 100000, fibnumber: 40, value: 267914288, time: 1315.0000000023283 ms
Wasm float run 2, times: 100000, fibnumber: 40, value: 267914288, time: 1305.0050000310875 ms
Wasm float run 3, times: 100000, fibnumber: 40, value: 267914288, time: 1304.210000031162 ms
Wasm double run 0, times: 100000, fibnumber: 40, value: 267914295, time: 1373.055000032764 ms
Wasm double run 1, times: 100000, fibnumber: 40, value: 267914295, time: 1371.5949999750592 ms
Wasm double run 2, times: 100000, fibnumber: 40, value: 267914295, time: 1374.619999958668 ms
Wasm double run 3, times: 100000, fibnumber: 40, value: 267914295, time: 1371.6049999929965 ms
x86_64 fibonacci uint32 run: 0, times: 100000, fibnumber: 40, value: 267914295, time: 608 ms
x86_64 fibonacci uint32 run: 1, times: 100000, fibnumber: 40, value: 267914295, time: 605 ms
x86_64 fibonacci uint32 run: 2, times: 100000, fibnumber: 40, value: 267914295, time: 608 ms
x86_64 fibonacci uint32 run: 3, times: 100000, fibnumber: 40, value: 267914295, time: 599 ms
x86_64 fibonacci uint64 run: 0, times: 100000, fibnumber: 40, value: 267914295, time: 562 ms
x86_64 fibonacci uint64 run: 1, times: 100000, fibnumber: 40, value: 267914295, time: 558 ms
x86_64 fibonacci uint64 run: 2, times: 100000, fibnumber: 40, value: 267914295, time: 556 ms
x86_64 fibonacci uint64 run: 3, times: 100000, fibnumber: 40, value: 267914295, time: 558 ms
x86_64 fibonacci float run: 0, times: 100000, fibnumber: 40, value: 267914288, time: 811 ms
x86_64 fibonacci float run: 1, times: 100000, fibnumber: 40, value: 267914288, time: 816 ms
x86_64 fibonacci float run: 2, times: 100000, fibnumber: 40, value: 267914288, time: 819 ms
x86_64 fibonacci float run: 3, times: 100000, fibnumber: 40, value: 267914288, time: 818 ms
x86_64 fibonacci double run: 0, times: 100000, fibnumber: 40, value: 267914295, time: 818 ms
x86_64 fibonacci double run: 1, times: 100000, fibnumber: 40, value: 267914295, time: 824 ms
x86_64 fibonacci double run: 2, times: 100000, fibnumber: 40, value: 267914295, time: 820 ms
x86_64 fibonacci double run: 3, times: 100000, fibnumber: 40, value: 267914295, time: 822 ms
```

I assume the recursion gets somehow optimized into an iterative call by clang and maybe JS has a lot more overhead when performing function calls. Since the fibonacci algorithm is doubly recursive this makes JS several times slower. It could also be JS memory allocator trying to resize the JS memory stack size as the recursion goes along, the wasm code gets compiled to use a fixed 8MB of memory for the stack (-Wl,-z,stack-size in the clang compilation command)

The x86_64 compared to WASM is about 20% faster for integer types and about 60% faster for floating points. Floating point implementation seem to be about 50% slower than integer implementation in both architectures.

Note that tweaking the parameters in this test it is pretty easy to make the values overflow, it seems the overflow behaviour in wasm is different than in x86_64 in that overflown floating point values become 0. Because of this performance comparison when values overflow are not accurate to real-world scenarios where overflows are rare, the wasm runtimes probably add extra checks when values overflow, slowing down the operations when overflows happen.

## render2d tests

A fairly heavy animation was implemented in both JS and C, the follow methods use it:

1. render2dJS(): renders an animation to a canvas in the screen using JS. This is parallelized using webworkers

2. render2dWasm(): renders an animation to a canvas in the screen using Wasm

3. render2dJSPerformanceTest(): tracks how long it takes to render a certain amount of frames at a certain resolution using Wasm. This is NOT parallelized.

4. render2dWasmPerformanceTest(): tracks how long it takes to render a certain amount of frames at a certain resolution using Wasm

Due to webassembly limitations the WASM rendering can not be parallelized like the JS can, therefor the performance tests are not parallelized.

Results:

```
JS render2d 600x600, 500 frames, run 0, time: 13105 ms, fps: 38.15
JS render2d 600x600, 500 frames, run 1, time: 13039 ms, fps: 38.35
JS render2d 600x600, 500 frames, run 2, time: 13057 ms, fps: 38.29
JS render2d 600x600, 500 frames, run 3, time: 13045 ms, fps: 38.33
Wasm render2d 600x600, 500 frames, run 0, time: 14859 ms, fps: 33.65
Wasm render2d 600x600, 500 frames, run 1, time: 14764 ms, fps: 33.87
Wasm render2d 600x600, 500 frames, run 2, time: 14718 ms, fps: 33.97
Wasm render2d 600x600, 500 frames, run 3, time: 14734 ms, fps: 33.94
x86_64 render2d  600 x 600, run: 0, frames: 500, time: 12763 ms, fps: 39.175742
x86_64 render2d  600 x 600, run: 1, frames: 500, time: 12752 ms, fps: 39.209536
x86_64 render2d  600 x 600, run: 2, frames: 500, time: 12774 ms, fps: 39.142007
x86_64 render2d  600 x 600, run: 3, frames: 500, time: 12732 ms, fps: 39.271128
```

The results of this test are very surprinsingly, not only is JS significantly faster than WASM it is only marginally worse than x86_64. In C this animation function uses a lot of functions from libc (abs, round, fmod, fabs, sqrt), I wonder if one of these functions is significantly slower in libc than in Chrome.

# Conclusions

Using WASM for optimizing performance is not a good idea at the moment, the wasm runtimes still have a long way to go. Note that some of these tests do not rely on WASI and yet the performance gains are not that meaningful (except when using a lot of integer arithmetics). The ones that do rely on WASI the final .wasm file is worse in performance than the JS.

WASM main use-case right now is to make code portable, especially C/C++ code for 2d and 3d graphics since it is relatively easy to write to the browser `<canvas>` frame buffer from C++.

Another use-case is sandboxing 3rd party code, WASM code runs sandboxed and can never write to any memory outside the memory buffer you provide to it. The host environment (JS in the case of the browser), has full access to all the memory the WASM code is using, but the opposite is not true.

Also note that these tests run several hundred times in a loop and therefor get optimized by the JS JIT, very heavy functions that are called only a few times during program execution usually don't get optimized by the JIT and therefor are a good use-case for WASM.

Not to thrown down WASM, WASM is great and it IS faster than JS (especially when using a lot of integer arithmetics), but then runtimes and building toolchains are still not very well optimized. But the JS JIT is just really really good, so much so that in some cases it can even compete with x86_64 code.
