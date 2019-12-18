function fib(n: number, nMinus1: number, idx: number): number {
  if (idx <= 2) {
    return n;
  }
  return fib(n + nMinus1, n, idx - 1);
}

const TIMES = 500000;
const FIBNUMBER = 70;

function fibJsBenchmark(): number {
  let value;
  const t0 = performance.now();
  for (let i = 0; i < TIMES; i++) {
    value = fib(1, 1, FIBNUMBER);
  }
  const t1 = performance.now();
  console.log(`value ${value}`);
  return t1 - t0;
}

function fibCBenchmark(wasm: any): number {
  const t0 = performance.now();
  const value = wasm.instance.exports.fibBenchmark(TIMES, FIBNUMBER);
  const t1 = performance.now();
  console.log(`value ${value}`);
  return t1 - t0;
}

function wait(time: number) {
  return new Promise(resolve => {
    setTimeout(resolve, time);
  });
}

export default async function performanceTest(wasm: any) {
  await wait(3000);
  console.log(`C cold run: ${fibCBenchmark(wasm)} ms`);
  await wait(1000);
  console.log(`C hot run: ${fibCBenchmark(wasm)} ms`);
  await wait(1000);
  console.log(`C hot2 run: ${fibCBenchmark(wasm)} ms`);
  await wait(1000);
  console.log(`C hot3 run: ${fibCBenchmark(wasm)} ms`);
  await wait(1000);

  console.log(`js cold run: ${fibJsBenchmark()} ms`);
  await wait(1000);
  console.log(`js hot run: ${fibJsBenchmark()} ms`);
  await wait(1000);
  console.log(`js hot2 run: ${fibJsBenchmark()} ms`);
  await wait(1000);
  console.log(`js hot3 run: ${fibJsBenchmark()} ms`);
}
