@call ../build-config.cmd
npm install
python ../ninja.py main.emscripten debug
