/* eslint-disable @typescript-eslint/no-unused-vars */
import { WASI } from '@wasmer/wasi';
import { WasmFs } from '@wasmer/wasmfs';
// tslint:disable-next-line
import addingTest from './addingTest';
// tslint:disable-next-line
import bridgeTest from './bridgeTest';
// tslint:disable-next-line
import { render2dJS, render2dWasm } from './canvas2dTest';
// tslint:disable-next-line
import { CharPointer, charPtrToString } from './chelpers';
// tslint:disable-next-line
import fibTest from './fibTest';
import './index.css';
// @ts-ignore
import Worker from './test.worker';

// tslint:disable-next-line
function workerTest(workers = 10) {
  // worker has an infinite loop consuming 100% of the cpu
  for (let i = 0; i < workers; i++) {
    (idx => {
      const worker = new Worker();
      worker.onmessage = function(event: any) {
        console.log(`w${idx} ${event.data.count}`);
      };
    })(i);
  }
}

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
  // await addingTest(wasm);
  // await fibTest(wasm);
  // await render2dJS();
  // await render2dWasm(wasm);
  // workerTest();
};

startWasiTask();
