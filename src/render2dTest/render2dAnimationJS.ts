const RAD = 2 * Math.PI;
const BLADES = 3;
const CYCLE_WIDTH = 100;
const BLADES_T_CYCLE_WIDTH = BLADES * CYCLE_WIDTH;

let width: number,
  height: number,
  ch: number,
  cw: number,
  maxDistance: number,
  offset: number,
  len: number;

let buf32: Uint32Array;

export function init2dAnimationJS(payload: {
  width: number;
  height: number;
  sharedBuffer: SharedArrayBuffer;
  offset: number;
  length: number;
}) {
  width = payload.width;
  height = payload.height;
  ch = height / 2;
  cw = width / 2;
  maxDistance = Math.sqrt(ch * ch + cw * cw);
  offset = payload.offset;
  len = payload.length;
  buf32 = new Uint32Array(payload.sharedBuffer);
}

export function renderAnimationJS(timestamp: number) {
  const scaledTimestamp = Math.floor(timestamp / 10.0 + 2000.0);
  for (let y = 0; y < height; y += 1) {
    const dy = ch - y;
    const dysq = dy * dy;
    const yw = y * width;
    for (let x = offset; x < offset + len; x++) {
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
}
