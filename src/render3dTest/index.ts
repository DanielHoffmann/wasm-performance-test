// @ts-ignore
import fragmentShader from './fragment.glsl';
// @ts-ignore
import vertexShader from './vertex.glsl';

const canvas = document.getElementById('3dcanvas-js') as HTMLCanvasElement;
const fpsCounter = document.getElementById('3dcanvasfps-js') as HTMLSpanElement;

const gl = canvas.getContext('webgl') as WebGL2RenderingContext;

let width: number;
let height: number;
let aspectRatio: number;
let currentRotation = [0, 1];
let currentScale = [1.0, 1.0];

// Vertex information

let vertexArray: Float32Array;
let vertexBuffer: WebGLBuffer;
let vertexNumComponents: number;
let vertexCount: number;

// Rendering data shared with the
// scalers.

let uScalingFactor: WebGLUniformLocation;
let uGlobalColor: WebGLUniformLocation;
let uRotationVector: WebGLUniformLocation;
let aVertexPosition: number;

// Animation timing

let previousTime = 0.0;
const degreesPerSecond = 90.0;
export function render3dTest(cssanimate = false) {
  (document.getElementById(
    '3dcanvas-js-container',
  ) as HTMLElement).style.display = 'flex';

  if (cssanimate) {
    canvas.classList.add('animate');
  }
  width = canvas.clientWidth;
  height = canvas.clientHeight;
  canvas.width = width;
  canvas.height = height;
  aspectRatio = width / height;
  currentRotation = [0, 1];
  currentScale = [1.0, aspectRatio];

  const program = gl.createProgram() as WebGLProgram;
  const vShader = gl.createShader(gl.VERTEX_SHADER);
  const fShader = gl.createShader(gl.FRAGMENT_SHADER);
  if (vShader === null || fShader === null || program === null) {
    throw new Error('something is null');
  }
  gl.shaderSource(vShader, vertexShader);
  gl.compileShader(vShader);
  if (!gl.getShaderParameter(vShader, gl.COMPILE_STATUS)) {
    const info = gl.getShaderInfoLog(vShader);
    throw new Error(`Could not compile WebGL program. \n\n${info}`);
  }
  gl.shaderSource(fShader, fragmentShader);
  gl.compileShader(fShader);
  if (!gl.getShaderParameter(fShader, gl.COMPILE_STATUS)) {
    const info = gl.getShaderInfoLog(fShader);
    throw new Error(`Could not compile WebGL program. \n\n${info}`);
  }
  gl.attachShader(program, vShader);
  gl.attachShader(program, fShader);
  gl.linkProgram(program);
  if (!gl.getProgramParameter(program, gl.LINK_STATUS)) {
    console.log(gl.getProgramInfoLog(program));
    throw new Error('Error linking shader program:');
  }
  aspectRatio = width / height;
  currentRotation = [0, 1];
  currentScale = [1.0, aspectRatio];
  gl.useProgram(program);
  vertexArray = new Float32Array([
    -0.5,
    0.5,
    0.5,
    0.5,
    0.5,
    -0.5,
    -0.5,
    0.5,
    0.5,
    -0.5,
    -0.5,
    -0.5,
  ]);

  vertexBuffer = gl.createBuffer() as WebGLBuffer;
  gl.bindBuffer(gl.ARRAY_BUFFER, vertexBuffer);
  gl.bufferData(gl.ARRAY_BUFFER, vertexArray, gl.STATIC_DRAW);
  vertexNumComponents = 2;
  vertexCount = vertexArray.length / vertexNumComponents;

  let currentAngle = 0.0;

  let lastCalledTime = performance.now();
  let count = 0;
  function animateScene() {
    const now = performance.now();
    const delta = (now - lastCalledTime) / 1000;
    lastCalledTime = now;
    if (count === 10) {
      count = 0;
      fpsCounter.innerText = (1 / delta).toFixed(2);
    } else {
      count++;
    }
    gl.viewport(0, 0, width, height);
    gl.clearColor(0.8, 0.9, 1.0, 1.0);
    gl.clear(gl.COLOR_BUFFER_BIT);

    const radians = (currentAngle * Math.PI) / 180.0;
    currentRotation[0] = Math.sin(radians);
    currentRotation[1] = Math.cos(radians);

    gl.useProgram(program);

    uScalingFactor = gl.getUniformLocation(
      program,
      'uScalingFactor',
    ) as WebGLUniformLocation;
    uGlobalColor = gl.getUniformLocation(
      program,
      'uGlobalColor',
    ) as WebGLUniformLocation;
    uRotationVector = gl.getUniformLocation(
      program,
      'uRotationVector',
    ) as WebGLUniformLocation;

    gl.uniform2fv(uScalingFactor, currentScale);
    gl.uniform2fv(uRotationVector, currentRotation);
    gl.uniform4fv(uGlobalColor, [0.1, 0.7, 0.2, 1.0]);

    gl.bindBuffer(gl.ARRAY_BUFFER, vertexBuffer);

    aVertexPosition = gl.getAttribLocation(program, 'aVertexPosition');

    gl.enableVertexAttribArray(aVertexPosition);
    gl.vertexAttribPointer(
      aVertexPosition,
      vertexNumComponents,
      gl.FLOAT,
      false,
      0,
      0,
    );

    gl.drawArrays(gl.TRIANGLES, 0, vertexCount);

    window.requestAnimationFrame((currentTime: any) => {
      const deltaAngle =
        ((currentTime - previousTime) / 1000.0) * degreesPerSecond;

      currentAngle = (currentAngle + deltaAngle) % 360;

      previousTime = currentTime;
      animateScene();
    });
  }
  animateScene();
}
