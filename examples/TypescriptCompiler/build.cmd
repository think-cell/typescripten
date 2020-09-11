@call ../../build-config.cmd
@call npm install
python ../../ninja.py main.emscripten debug
