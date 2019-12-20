/* eslint-disable no-constant-condition */
let count = 0;

while (true) {
  count++;
  if (count % 500000000 === 0) {
    self.postMessage({ count });
  }
}
