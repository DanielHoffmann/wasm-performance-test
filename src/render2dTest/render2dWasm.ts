const canvas = document.getElementById('2dcanvas-wasm') as HTMLCanvasElement;
const fpsCounter = document.getElementById(
  '2dcanvasfps-wasm',
) as HTMLSpanElement;

const ctx = canvas.getContext('2d', {
  alpha: false,
  antialias: false,
  depth: false,
}) as CanvasRenderingContext2D;

function wait(time: number) {
  return new Promise(resolve => {
    setTimeout(resolve, time);
  });
}

let width: number;
let height: number;

export async function render2dWasmPerformanceTest(
  wasm: any,
  resolution = 600,
  runs = 4,
  frames = 500,
) {
  const {
    instance: {
      exports: { render2d, init2d },
    },
  } = wasm;

  await wait(2000);
  console.log('---------render2dWasmPerformanceTest START -----------');
  for (let i = 0; i < runs; i++) {
    init2d(resolution, resolution);
    const t0 = performance.now();
    for (let j = 0; j < frames; j++) {
      render2d(j * 1000);
    }
    const t1 = performance.now();
    console.log(
      `C render2dWasm ${resolution}x${resolution}, ${frames} frames, run ${i}, time: ${(
        t1 - t0
      ).toFixed(0)} ms, fps: ${((1000 * frames) / (t1 - t0)).toFixed(2)}`,
    );
    await wait(1000);
  }
  console.log('---------render2dWasmPerformanceTest END -------------');
}

export async function render2dWasm(wasm: any, cssanimate = false) {
  (document.getElementById(
    '2dcanvas-wasm-container',
  ) as HTMLElement).style.display = 'flex';

  if (cssanimate) {
    canvas.classList.add('animate');
  }

  width = canvas.clientWidth;
  height = canvas.clientHeight;
  canvas.width = width;
  canvas.height = height;

  const {
    instance: { exports: exp },
  } = wasm;
  const pointer = exp.init2d(width, height);
  const data = new Uint8ClampedArray(
    exp.memory.buffer,
    pointer,
    width * height * 4,
  );
  const img = new ImageData(data, width, height);
  let lastCalledTime: number = performance.now();
  let count = 0;
  const render = (timestamp: number) => {
    const now = performance.now();
    const delta = (now - lastCalledTime) / 1000;
    lastCalledTime = now;
    if (count === 10) {
      count = 0;
      fpsCounter.innerText = (1 / delta).toFixed(2);
    } else {
      count++;
    }
    exp.render2d(timestamp);
    ctx.putImageData(img, 0, 0);
    window.requestAnimationFrame(render);
  };
  window.requestAnimationFrame(render);
}
