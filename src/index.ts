import { WASI } from '@wasmer/wasi';
import { WasmFs } from '@wasmer/wasmfs';
import fibTest from './fibTest';
import addingTest from './addingTest';
import bridgeTest from './bridgeTest';
import { CharPointer, charPtrToString } from './cstring';

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
      bridgeTest: (ptr: CharPointer) => void;
      intTimes2: (val: number) => number;
      uLongTimes2: (val: number) => number;
      fibBenchmark: (fibNumber: number, times: number) => void;
    };
  };
} & WebAssembly.WebAssemblyInstantiatedSource;

const bridge = {
  __console_log(str: CharPointer) {
    console.log(charPtrToString(wasm, str));
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
  wasi.start(wasm.instance);

  await bridgeTest(wasm, wasmFs);
  await addingTest(wasm);
  await fibTest(wasm);
};

startWasiTask();
