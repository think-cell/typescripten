@call %~dp0\build-config.cmd || exit /b 1
%EMCXX% ^
    %EMCXXFLAGS% -std=c++20 ^
    -isystem %THINK_CELL_PUBLIC_PATH% ^
    -isystem %BOOST_PATH% ^
    %EMLDFLAGS% --bind ^
    -I %BOOTSTRAP_PATH%\include ^
    --pre-js %BOOTSTRAP_PATH%\src\js_callback.js ^
    %BOOTSTRAP_PATH%\src\js_callback.cpp ^
    %*
