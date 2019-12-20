@REM Copy this file to build-config.cmd and edit three lines below
@call <path-to-emsdk_env.bat> >nul 2>&1 || exit /b 1
@set BOOST_PATH=<path-to-boost-includes>
@set THINK_CELL_PUBLIC_PATH=<path-to-think-cell-public-library-includes>
@set EMCXX=em++
@set BOOTSTRAP_PATH=%~dp0\bootstrap
@set EMCXX_FLAGS=^
    -s SAFE_HEAP=1 -s ASSERTIONS=2 -s WASM=1 -s DEMANGLE_SUPPORT=1 ^
    -Wall -Wextra -Werror ^
    -g4 --source-map-base http://localhost:8080/ ^
    -std=c++17 --bind ^
    -isystem %THINK_CELL_PUBLIC_PATH% ^
    -isystem %BOOST_PATH%
