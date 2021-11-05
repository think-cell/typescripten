To try out the example, [get emscripten](https://emscripten.org/docs/getting_started/downloads.html) and a recent [CMake](https://cmake.org), and run the following in your shell

```
cd example

source ~/emsdk-2.0.27/emsdk_env.sh
set CMAKE_TOOLCHAIN_FILE= ~/emsdk-2.0.27/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake

emcmake cmake -S . -B build
cmake --build build

python3 -m http.server 
```

and open [http://localhost:8000](http://localhost:8000)