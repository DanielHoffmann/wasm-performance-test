/* eslint-disable */
const createConfig = require('@sandvikcode/web-tooling/config/webpack');

const conf = createConfig({
  cssVariablesPath: 'src/variables.css',
  envVariables: {},
  devServerOptions: {
    port: 1323,
  },
});

conf.module.rules.push({ test: /\.(glsl|vs|fs)$/, loader: 'ts-shader-loader' });

module.exports = conf;
