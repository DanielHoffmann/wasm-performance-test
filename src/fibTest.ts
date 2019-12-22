function fib(n: number): number {
  if (n < 2) {
    return n;
  }
  // adding +1 to prevent tail-call optmization
  return fib(n - 1) + fib(n - 2) + 1;
}

function wait(time: number) {
  return new Promise(resolve => {
    setTimeout(resolve, time);
  });
}

async function fibJsBenchmark(runs: number, times: number, fibnumber: number) {
  for (let i = 0; i < runs; i++) {
    let value = 0;
    const t0 = performance.now();
    for (let j = 0; j < times; j++) {
      value += fib(fibnumber);
    }
    const t1 = performance.now();
    console.log(`JS run ${i}, value: ${value}, time: ${t1 - t0} ms`);
    await wait(1000);
  }
}

async function fibWasmBenchmark(
  wasm: any,
  type: 'ulong' | 'float' | 'double',
  runs: number,
  times: number,
  fibnumber: number,
) {
  let fn = wasm.instance.exports.fibBenchmarkULong;
  if (type === 'float') {
    fn = wasm.instance.exports.fibBenchmarkFloat;
  } else if (type === 'double') {
    fn = wasm.instance.exports.fibBenchmarkDouble;
  }
  for (let i = 0; i < runs; i++) {
    const t0 = performance.now();
    const value = fn(times, fibnumber);
    const t1 = performance.now();
    console.log(`C ${type} run ${i}, value: ${value}, time: ${t1 - t0} ms`);
    await wait(1000);
  }
}

export default async function performanceTest(
  wasm: any,
  runs = 4,
  times = 20000,
  fibnumber = 20,
) {
  await wait(2000);

  console.log('---------FIBONACCI TESTS START -----------');
  await fibWasmBenchmark(wasm, 'ulong', runs, times, fibnumber);
  await fibWasmBenchmark(wasm, 'float', runs, times, fibnumber);
  await fibWasmBenchmark(wasm, 'double', runs, times, fibnumber);
  await fibJsBenchmark(runs, times, fibnumber);
  console.log('---------FIBONACCI TESTS END -------------');
}
