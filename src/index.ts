/* eslint-disable @typescript-eslint/no-unused-vars */
import { WASI } from '@wasmer/wasi';
import { WasmFs } from '@wasmer/wasmfs';
import addingTest from './addingTest';
import bridgeTest from './bridgeTest';
import { CharPointer, charPtrToString } from './chelpers';
import fibTest from './fibTest';
import {
  render2dJS,
  render2dJSPerformanceTest,
  render2dWasm,
  render2dWasmPerformanceTest,
} from './render2dTest';
import './index.css';
// @ts-ignore
import Worker from './test.worker';

function workerTest(threads = 10) {
  // worker has an infinite loop consuming 100% of the cpu
  for (let i = 0; i < threads; i++) {
    (idx => {
      const worker = new Worker();
      worker.onmessage = function(event: any) {
        console.log(`w${idx} ${event.data.count}`);
      };
    })(i);
  }
}

// preventing unused variable TS errors
let a: any = addingTest;
a = bridgeTest;
a = render2dJS;
a = render2dWasm;
a = fibTest;
a = workerTest;
a = render2dWasmPerformanceTest;
a = render2dJSPerformanceTest;
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
  // await render2dJS(4);
  // await render2dWasm(wasm);
  // workerTest();

  /////// PERFORMANCE TESTS ////////
  await addingTest(wasm);
  // await fibTest(wasm);
  // await render2dWasmPerformanceTest(wasm);
  // await render2dJSPerformanceTest();
};

startWasiTask();
