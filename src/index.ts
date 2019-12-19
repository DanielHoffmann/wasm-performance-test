import { WASI } from '@wasmer/wasi';
import { WasmFs } from '@wasmer/wasmfs';
import fibTest from './fibTest';
import addingTest from './addingTest';
import bridgeTest from './bridgeTest';
import { render2dWasm, render2dJS } from './canvas2dTest';
import { CharPointer, charPtrToString } from './chelpers';

const wasmFs = new WasmFs();
let wasi = new WASI({
  args: [],
  env: {},
  bindings: {
    ...WASI.defaultBindings,
    fs: wasmFs.fs,
  },
});

let wasm: WebAssembly.WebAssemblyInstantiatedSource;

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
    wasi_unstable: wasi.wasiImport,
    env: bridge,
  });
  wasi.start(wasm.instance);

  await bridgeTest(wasm, wasmFs);
  // await addingTest(wasm);
  // await fibTest(wasm);
  // await render2dWasm(wasm);
  // await render2dJS();
};

startWasiTask();
