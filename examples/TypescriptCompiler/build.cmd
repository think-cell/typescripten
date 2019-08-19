"C:\emsdk\upstream\emscripten\em++.bat" ^
    -std=c++17 --bind ^
    -g4 --source-map-base http://localhost:8080/ ^
    -s SAFE_HEAP=1 -s ASSERTIONS=2 -s WASM=1 ^
    -I ../../bootstrap/include ^
    -isystem ../../../code/Library/HeaderOnly/Public ^
    -isystem c:/Libraries/boost_1_67_0 ^
    -Wall -Wextra -Werror ^
    --pre-js ../../bootstrap/src/js_callback.js ^
    ../../bootstrap/src/js_callback.cpp ^
    --pre-js main-pre.js ^
    -o main.js ^
    main.cpp
