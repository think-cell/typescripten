const path = require("path");

module.exports = {
  mode: "development",
  entry: ['./debug/debug.js'],
  output: {
    filename: 'main.js',
    path: path.resolve(__dirname, 'debug')
  },
  module: {
    rules: [
      {
        test: /typescriptenc\.wasm$/,
        type: "javascript/auto",
        loader: "file-loader",
        options: {
          name: 'typescriptenc.wasm',
        }
      }
    ]
  },
  resolve: {
    fallback: { 
      "crypto": false,
      "path": false,
      "fs": false,
      "perf_hooks": false
    }
  }
};
