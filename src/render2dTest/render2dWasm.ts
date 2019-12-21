const canvas = document.getElementById('2dcanvas-wasm') as HTMLCanvasElement;
const fpsCounter = document.getElementById(
  '2dcanvasfps-wasm',
) as HTMLSpanElement;

const ctx = canvas.getContext('2d', {
  alpha: false,
  antialias: false,
  depth: false,
}) as CanvasRenderingContext2D;

let width: number;
let height: number;

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
