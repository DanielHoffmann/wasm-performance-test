extract prebuilt clang binaries into /clang:

http://releases.llvm.org/download.html

add /clang/bin to PATH

extract libclang_rt.builtins-wasm32-wasi-VERSION.tar.gz from:

https://github.com/CraneStation/wasi-sdk/releases

into /clang/lib/9.0.0/lib/wasi/ (create wasi folder if it doesn't exist)

extract wasi-sysroot-VERSION-OS.tar.gz from:

https://github.com/CraneStation/wasi-sdk/releases

into /wasi-sysroot

yarn install

yarn clang # run this again on every .cpp file change

yarn start
