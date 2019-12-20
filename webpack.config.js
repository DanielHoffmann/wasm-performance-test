/* eslint-disable */
const createConfig = require('@sandvikcode/web-tooling/config/webpack');

module.exports = createConfig({
  cssVariablesPath: 'src/variables.css',
  envVariables: {},
  devServerOptions: {
    port: 1323,
  },
});
