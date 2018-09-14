const presets = [ "@babel/env" ];
const plugins = [
  ["@babel/transform-runtime", {
    "corejs": 2,
    "helpers": true,
    "regenerator": true,
    "useESModules": false,
  }],
  "@babel/transform-async-to-generator",
];
module.exports = { presets, plugins };
