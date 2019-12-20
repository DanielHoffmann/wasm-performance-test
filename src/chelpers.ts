export type CharPointer = number;

function getMemory(wasm: any) {
  return new Uint8Array(wasm.instance.exports.memory.buffer);
}
const decoder = new TextDecoder('utf-8');
const encoder = new TextEncoder();

export function charPtrToString(wasm: any, charPtr: CharPointer): string {
  const memory = getMemory(wasm);
  let length = 0;
  for (; memory[charPtr + length] !== 0; ++length) {
    //
  }
  return decoder.decode(memory.subarray(charPtr, charPtr + length));
}

export function stringToCharPtr(wasm: any, str: string): CharPointer {
  const data = encoder.encode(`${str}\x00`);
  const ptr = wasm.instance.exports.get_memory_for_string(data.length + 1);
  const memory = getMemory(wasm);
  memory.subarray(ptr).set(data);
  return ptr;
}

export function freeCharPtr(wasm: any, ptr: CharPointer) {
  wasm.instance.exports.free_memory_for_string(ptr);
}
