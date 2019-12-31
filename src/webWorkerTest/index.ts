// @ts-ignore
import Worker from './test.worker';

export default function workerTest(threads = 10) {
  // worker has an infinite loop consuming 100% of the cpu
  for (let i = 0; i < threads; i++) {
    (idx => {
      const worker = new Worker();
      worker.onmessage = function(event: any) {
        console.log(`w${idx} ${event.data.count}`);
      };
    })(i);
  }
}
