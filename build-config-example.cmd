@REM Copy this file to build-config.cmd and edit three lines below
@call <path-to-emsdk_env.bat> >nul 2>&1 || exit /b 1
@set BOOST_PATH=<path-to-boost-includes>
@set THINK_CELL_PUBLIC_PATH=<path-to-think-cell-public-library-includes>
@set EMCXX=em++
@set BOOTSTRAP_PATH=%~dp0\bootstrap
@set EMCXXFLAGS=^
	-D_DEBUG -Wall -Wextra -Werror ^
	-g -s SAFE_HEAP=1 -s ASSERTIONS=2 -s DEMANGLE_SUPPORT=1
@set EMLDFLAGS=^
    -g4 --source-map-base http://localhost:8080/
