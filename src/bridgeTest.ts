export default async function bridgeTest(wasm: any, wasmFs: any) {
  const { instance } = wasm;
  console.log(await wasmFs.getStdOut());

  instance.exports.test();

  console.log(
    'This is computed in C and returned to JS: ' +
      instance.exports.intTimes2(10),
  );

  console.log(
    'This is computed in C and returned to JS: ' +
      instance.exports.uLongTimes2(10),
  );
}
