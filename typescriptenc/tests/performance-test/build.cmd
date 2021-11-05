@call ../../../build-config.cmd
@call node ../../../node_modules/typescript/bin/tsc --strict --declaration MyLib.ts
python ../../../ninja.py main.emscripten debug
