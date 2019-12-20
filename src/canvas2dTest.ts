const canvaswasm = document.getElementById(
  '2dcanvas-wasm',
) as HTMLCanvasElement;
const fpsCounterwasm = document.getElementById(
  '2dcanvasfps-wasm',
) as HTMLSpanElement;

const canvasjs = document.getElementById('2dcanvas-js') as HTMLCanvasElement;
const fpsCounterjs = document.getElementById(
  '2dcanvasfps-js',
) as HTMLSpanElement;

const ctxwasm = canvaswasm.getContext('2d', {
  alpha: false,
  antialias: false,
  depth: false,
}) as CanvasRenderingContext2D;
const ctxjs = canvasjs.getContext('2d', {
  alpha: false,
  antialias: false,
  depth: false,
}) as CanvasRenderingContext2D;

const width = canvaswasm.width;
const height = canvaswasm.height;

export async function render2dWasm(wasm: any) {
  (document.getElementById(
    '2dcanvas-wasm-container',
  ) as HTMLElement).style.display = 'flex';
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
      fpsCounterwasm.innerText = (1 / delta).toFixed(2);
    } else {
      count++;
    }
    exp.render2d(timestamp);
    ctxwasm.putImageData(img, 0, 0);
    window.requestAnimationFrame(render);
  };
  window.requestAnimationFrame(render);
}

const RAD = 2 * Math.PI;
const BLADES = 3;
const CYCLE_WIDTH = 100;
const BLADES_T_CYCLE_WIDTH = BLADES * CYCLE_WIDTH;
const ch = height / 2;
const cw = width / 2;
const maxDistance = Math.sqrt(ch * ch + cw * cw);

let imageData: ImageData;
let buf: ArrayBuffer;
let buf8: Uint8ClampedArray;
let buf32: Uint32Array;

export function render2dJS() {
  (document.getElementById(
    '2dcanvas-js-container',
  ) as HTMLElement).style.display = 'flex';
  imageData = ctxjs.getImageData(0, 0, width, height);
  buf = new ArrayBuffer(imageData.data.length);
  buf8 = new Uint8ClampedArray(buf);
  buf32 = new Uint32Array(buf);
  window.requestAnimationFrame(render2dJSActual);
}

let lastCalledTime: number = performance.now();
let count = 0;
function render2dJSActual(timestamp: number) {
  const now = performance.now();
  const delta = (now - lastCalledTime) / 1000;
  lastCalledTime = now;
  if (count === 10) {
    count = 0;
    fpsCounterjs.innerText = (1 / delta).toFixed(2);
  } else {
    count++;
  }
  const scaledTimestamp = Math.floor(timestamp / 10.0 + 2000.0);
  for (let y = 0; y < height; y += 1) {
    const dy = ch - y;
    const dysq = dy * dy;
    const yw = y * width;
    for (let x = 0; x < width; x += 1) {
      const dx = cw - x;
      const dxsq = dx * dx;
      const angle = Math.atan2(dx, dy) / RAD;
      const asbs = dxsq + dysq;
      const distanceFromCenter = Math.sqrt(asbs);
      const scaledDistance = asbs / 400.0 + distanceFromCenter;
      const lerp =
        1.0 -
        (Math.abs(
          scaledDistance - scaledTimestamp + angle * BLADES_T_CYCLE_WIDTH,
        ) %
          CYCLE_WIDTH) /
          CYCLE_WIDTH;
      const absoluteDistanceRatioGB = 1.0 - distanceFromCenter / maxDistance;
      const absoluteDistanceRatioR = absoluteDistanceRatioGB * 0.8 + 0.2;
      const fadeB = 50.0 * lerp * absoluteDistanceRatioGB;
      const fadeR = 240.0 * lerp * absoluteDistanceRatioR * (1.0 + lerp) * 0.5;
      const fadeG = 120.0 * lerp * lerp * lerp * absoluteDistanceRatioGB;
      buf32[yw + x] =
        (255 << 24) | // A
        (fadeB << 16) | // B
        (fadeG << 8) | // G
        fadeR; // R
    }
  }
  imageData.data.set(buf8);
  ctxjs.putImageData(imageData, 0, 0);
  window.requestAnimationFrame(render2dJSActual);
}
