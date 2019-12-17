extract prebuilt clang binaries into /clang:

http://releases.llvm.org/download.html

clone:
https://github.com/CraneStation/wasi-libc

make install INSTALL_DIR=/wasi-libc

copy libclang_rt.builtins-wasm32.a from

https://github.com/jedisct1/libclang_rt.builtins-wasm32.a/tree/master/precompiled

into /clang/lib/9.0.0/lib/wasi/ (create wasi folder if it doesn't exist)

add /clang/bin to PATH

yarn install

yarn clang

yarn start
