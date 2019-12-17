const createConfig = require('@sandvikcode/web-tooling/config/webpack');

module.exports = createConfig({
  cssVariablesPath: 'src/variables.css',
  envVariables: {
    VARIABLE_NAME: 'true',
  },
  devServerOptions: {
    port: 1323,
  },
});
