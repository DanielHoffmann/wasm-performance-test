/* eslint-disable @typescript-eslint/no-unused-vars */
import { WASI } from '@wasmer/wasi';
import { WasmFs } from '@wasmer/wasmfs';
import addingTest from './addingTest';
import bridgeTest from './bridgeTest/bridgeTest';
import { CharPointer, charPtrToString } from './chelpers';
import fibTest from './fibTest';
import {
  render2dJS,
  render2dJSPerformanceTest,
  render2dWasm,
  render2dWasmPerformanceTest,
} from './render2dTest';
import './index.css';
import { render3dTest } from './render3dTest';
import webWorkerTest from './webWorkerTest';

// preventing unused variable TS errors
let a: any = addingTest;
a = bridgeTest;
a = render2dJS;
a = render2dWasm;
a = fibTest;
a = webWorkerTest;
a = render2dWasmPerformanceTest;
a = render2dJSPerformanceTest;
a = render3dTest;
a = '';
console.log(a);

const wasmFs = new WasmFs();
const wasi = new WASI({
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
  // @ts-ignore
  wasm = await WebAssembly.instantiate(
    new Uint8Array(responseArrayBuffer).buffer,
    {
      wasi_unstable: wasi.wasiImport,
      env: bridge,
    },
  );
  wasi.start(wasm.instance);

  await bridgeTest(wasm, wasmFs);
  // webWorkerTest();

  //////// RENDERING TESTS /////////
  await render2dJS(1);
  await render2dWasm(wasm);
  await render3dTest();

  /////// PERFORMANCE TESTS ////////
  // await addingTest(wasm);
  // await fibTest(wasm);
  // await render2dWasmPerformanceTest(wasm);
  // await render2dJSPerformanceTest();
};

startWasiTask();
