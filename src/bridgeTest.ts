import { freeCharPtr, stringToCharPtr } from './chelpers';

export default async function bridgeTest(wasm: any, wasmFs: any) {
  const {
    instance: { exports: exp },
  } = wasm;

  console.log('---------BRIDGE TESTS START -----------');
  console.log(await wasmFs.getStdOut());

  console.log(`This is computed in C and returned to JS: ${exp.times2(10)}`);

  const pointer = stringToCharPtr(wasm, 'JS unicode test áéíäåöç');
  exp.bridgeTest(pointer);
  freeCharPtr(wasm, pointer);

  exp.bridgeTestCppFeatures();
  console.log('---------BRIDGE TESTS END -------------');
}
