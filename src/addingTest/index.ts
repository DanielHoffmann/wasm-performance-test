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

async function jsBenchmark(runs: number, times: number) {
  for (let i = 0; i < runs; i++) {
    const t0 = performance.now();
    const value = adding(times);
    const t1 = performance.now();
    console.log(`JS run ${i}, value: ${value}, time: ${t1 - t0} ms`);
    await wait(1000);
  }
}

async function wasmBenchmark(
  wasm: any,
  type: 'uint32' | 'uint64' | 'uint' | 'ulong' | 'float' | 'double',
  runs: number,
  times: number,
) {
  const {
    instance: { exports: exp },
  } = wasm;
  let fn;
  if (type === 'uint') {
    fn = exp.addingUInt;
  } else if (type === 'ulong') {
    fn = exp.addingULong;
  } else if (type === 'uint32') {
    fn = exp.addingUInt32;
  } else if (type === 'uint64') {
    fn = exp.addingUInt64;
  } else if (type === 'float') {
    fn = exp.addingFloat;
  } else if (type === 'double') {
    fn = exp.addingDouble;
  }
  for (let i = 0; i < runs; i++) {
    const t0 = performance.now();
    const value = fn(times);
    const t1 = performance.now();
    console.log(`Wasm ${type} run ${i}, value: ${value}, time: ${t1 - t0} ms`);
    await wait(1000);
  }
}

export default async function performanceTest(
  wasm: any,
  runs = 4,
  times = 800000000,
) {
  await wait(2000);
  console.log('---------ADDING TESTS START -----------');
  await jsBenchmark(runs, times);
  await wasmBenchmark(wasm, 'uint', runs, times);
  await wasmBenchmark(wasm, 'ulong', runs, times);
  await wasmBenchmark(wasm, 'uint32', runs, times);
  await wasmBenchmark(wasm, 'uint64', runs, times);
  await wasmBenchmark(wasm, 'float', runs, times);
  await wasmBenchmark(wasm, 'double', runs, times);
  console.log('---------ADDING TESTS END -------------');
}
