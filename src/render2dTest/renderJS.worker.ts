import { init2dAnimationJS, renderAnimationJS } from './render2dAnimationJS';

self.onmessage = (message: any) => {
  switch (message.data.type) {
    case 'init':
      init2dAnimationJS(message.data.payload);
      break;
    case 'renderframe':
      const { timestamp } = message.data.payload;
      renderAnimationJS(timestamp);
      self.postMessage({ type: 'frameready' });
  }
};
