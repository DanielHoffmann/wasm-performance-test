import { WASI } from '@wasmer/wasi';
import { WasmFs } from '@wasmer/wasmfs';

type CharPointer = number;

const wasmFs = new WasmFs();
let wasi = new WASI({
  args: [],
  env: {},
  bindings: {
    ...WASI.defaultBindings,
    fs: wasmFs.fs,
  },
});

let wasm: {
  instance: {
    exports: {
      memory: {
        buffer: number;
      };
      get_memory_for_string: (size: number) => CharPointer;
      free_memory_for_string: (ptr: CharPointer) => void;
      test: () => void;
      times2: (val: number) => number;
    };
  };
} & WebAssembly.WebAssemblyInstantiatedSource;

function get_memory() {
  return new Uint8Array(wasm.instance.exports.memory.buffer);
}

const decoder = new TextDecoder('utf-8');
const encoder = new TextEncoder();

function charPtrToString(charPtr: CharPointer): string {
  const memory = get_memory();
  let length = 0;
  for (; memory[charPtr + length] !== 0; ++length) {}
  return decoder.decode(memory.subarray(charPtr, charPtr + length));
}

function stringToCharPtr(str: string): CharPointer {
  const data = encoder.encode(str + '\x00');
  const ptr = wasm.instance.exports.get_memory_for_string(data.length + 1);
  const memory = get_memory();
  memory.subarray(ptr).set(data);
  return ptr;
}

function freeCharPtr(ptr: CharPointer) {
  wasm.instance.exports.free_memory_for_string(ptr);
}

const bridge = {
  __console_log(str: CharPointer) {
    console.log(charPtrToString(str));
  },
};

const startWasiTask = async () => {
  const response = await fetch('./main.wasm');
  const responseArrayBuffer = await response.arrayBuffer();
  const wasm_bytes = new Uint8Array(responseArrayBuffer).buffer;
  // @ts-ignore
  wasm = await WebAssembly.instantiate(wasm_bytes, {
    wasi_snapshot_preview1: wasi.wasiImport,
    env: bridge,
  });
  const { instance } = wasm;
  wasi.start(instance);

  console.log(await wasmFs.getStdOut());

  instance.exports.test();

  console.log(
    'This is computed in C and returned to JS: ' + instance.exports.times2(10),
  );
};

startWasiTask();
