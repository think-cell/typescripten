@call ../../../build-config.cmd
@call ../../node_modules/typescript/bin/tsc --strict --declaration
python ../../../ninja.py main.emscripten debug
