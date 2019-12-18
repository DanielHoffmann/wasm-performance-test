function adding(times: number): number {
  let value = 0;
  for (let i = 0; i < times; i++) {
    if (i % 2 === 1) {
      value += i;
    } else {
      value -= i;
    }
  }
  return value;
}

function wait(time: number) {
  return new Promise(resolve => {
    setTimeout(resolve, time);
  });
}

const TIMES = 2000000;

async function fibJsBenchmark(runs: number) {
  for (let i = 0; i < runs; i++) {
    let value;
    const t0 = performance.now();
    value = adding(TIMES);
    const t1 = performance.now();
    console.log(`JS run ${i}, value: ${value}, time: ${t1 - t0} ms`);
    await wait(1000);
  }
}

async function fibCBenchmark(
  wasm: any,
  type: 'ulong' | 'float' | 'double',
  runs: number,
) {
  let fn = wasm.instance.exports.addingLong;
  if (type === 'float') {
    fn = wasm.instance.exports.addingFloat;
  } else if (type === 'double') {
    fn = wasm.instance.exports.addingDouble;
  }
  for (let i = 0; i < runs; i++) {
    const t0 = performance.now();
    const value = fn(TIMES);
    const t1 = performance.now();
    console.log(`C ${type} run ${i}, value: ${value}, time: ${t1 - t0} ms`);
    await wait(1000);
  }
}

export default async function performanceTest(wasm: any) {
  await wait(2000);
  console.log('---------ADDING TESTS START -----------');
  await fibCBenchmark(wasm, 'ulong', 4);
  await fibCBenchmark(wasm, 'float', 4);
  await fibCBenchmark(wasm, 'double', 4);
  await fibJsBenchmark(4);
  console.log('---------ADDING TESTS END -------------');
}
