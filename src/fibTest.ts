function fib(n: number): number {
  if (n < 2) {
    return n;
  }
  return fib(n - 1) + fib(n - 2) + 1;
}

function wait(time: number) {
  return new Promise(resolve => {
    setTimeout(resolve, time);
  });
}

const TIMES = 20000;
const FIBNUMBER = 20;

async function fibJsBenchmark(runs: number) {
  for (let i = 0; i < runs; i++) {
    let value = 0;
    const t0 = performance.now();
    for (let j = 0; j < TIMES; j++) {
      value += fib(FIBNUMBER);
    }
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
  let fn = wasm.instance.exports.fibBenchmarkULong;
  if (type === 'float') {
    fn = wasm.instance.exports.fibBenchmarkFloat;
  } else if (type === 'double') {
    fn = wasm.instance.exports.fibBenchmarkDouble;
  }
  for (let i = 0; i < runs; i++) {
    const t0 = performance.now();
    const value = fn(TIMES, FIBNUMBER);
    const t1 = performance.now();
    console.log(`C ${type} run ${i}, value: ${value}, time: ${t1 - t0} ms`);
    await wait(1000);
  }
}

export default async function performanceTest(wasm: any) {
  await wait(2000);

  console.log('---------FIBONACCI TESTS START -----------');
  await fibCBenchmark(wasm, 'ulong', 4);
  await fibCBenchmark(wasm, 'float', 4);
  await fibCBenchmark(wasm, 'double', 4);
  await fibJsBenchmark(4);
  console.log('---------FIBONACCI TESTS END -------------');
}
