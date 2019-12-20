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

async function fibJsBenchmark(runs: number, times: number) {
  for (let i = 0; i < runs; i++) {
    const t0 = performance.now();
    const value = adding(times);
    const t1 = performance.now();
    console.log(`JS run ${i}, value: ${value}, time: ${t1 - t0} ms`);
    await wait(1000);
  }
}

async function fibCBenchmark(
  wasm: any,
  type: 'ulong' | 'float' | 'double',
  runs: number,
  times: number,
) {
  let fn = wasm.instance.exports.addingLong;
  if (type === 'float') {
    fn = wasm.instance.exports.addingFloat;
  } else if (type === 'double') {
    fn = wasm.instance.exports.addingDouble;
  }
  for (let i = 0; i < runs; i++) {
    const t0 = performance.now();
    const value = fn(times);
    const t1 = performance.now();
    console.log(`C ${type} run ${i}, value: ${value}, time: ${t1 - t0} ms`);
    await wait(1000);
  }
}

export default async function performanceTest(
  wasm: any,
  runs = 4,
  times = 8000000,
) {
  await wait(2000);
  console.log('---------ADDING TESTS START -----------');
  await fibCBenchmark(wasm, 'ulong', runs, times);
  await fibCBenchmark(wasm, 'float', runs, times);
  await fibCBenchmark(wasm, 'double', runs, times);
  await fibJsBenchmark(runs, times);
  console.log('---------ADDING TESTS END -------------');
}
