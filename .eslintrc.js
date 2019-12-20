const config = require('@sandvikcode/web-tooling/config/eslint');
module.exports = {
  ...config,
  rules: {
    ...config.rules,
    'no-console': 'off',
    'no-await-in-loop': 'off',
    'no-explicit-any': 'off',
    '@typescript-eslint/no-explicit-any': 'off',
  },
};
