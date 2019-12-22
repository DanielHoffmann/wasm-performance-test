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
    console.log(
      `JS run ${i}, value: ${value}, fibnumber: ${fibnumber}, time: ${t1 -
        t0} ms`,
    );
    await wait(1000);
  }
}
let a = fibJsBenchmark;
// @ts-ignore
a = '';
console.log(a);

async function fibWasmBenchmark(
  wasm: any,
  type: 'uint32' | 'uint64' | 'float' | 'double',
  runs: number,
  times: number,
  fibnumber: number,
) {
  let fn = wasm.instance.exports.fibBenchmarkULong;
  if (type === 'uint32') {
    fn = wasm.instance.exports.fibBenchmarkUInt32;
  } else if (type === 'uint64') {
    fn = wasm.instance.exports.fibBenchmarkUInt64;
  } else if (type === 'float') {
    fn = wasm.instance.exports.fibBenchmarkFloat;
  } else if (type === 'double') {
    fn = wasm.instance.exports.fibBenchmarkDouble;
  }
  for (let i = 0; i < runs; i++) {
    const t0 = performance.now();
    const value = fn(times, fibnumber);
    const t1 = performance.now();
    console.log(
      `Wasm ${type} run ${i}, times: ${times}, fibnumber: ${fibnumber}, value: ${value}, time: ${t1 -
        t0} ms`,
    );
    await wait(1000);
  }
}

export default async function performanceTest(
  wasm: any,
  runs = 4,
  times = 100000,
  fibnumber = 40,
) {
  await wait(2000);

  console.log('---------FIBONACCI TESTS START -----------');
  await fibWasmBenchmark(wasm, 'uint32', runs, times, fibnumber);
  await fibWasmBenchmark(wasm, 'uint64', runs, times, fibnumber);
  await fibWasmBenchmark(wasm, 'float', runs, times, fibnumber);
  await fibWasmBenchmark(wasm, 'double', runs, times, fibnumber);
  // JS is A LOT slower, lower fibnumber before running!
  // await fibJsBenchmark(runs, times, fibnumber);
  console.log('---------FIBONACCI TESTS END -------------');
}
