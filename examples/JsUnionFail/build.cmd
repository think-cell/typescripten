@call ..\..\build-config.cmd
%EMCXX% %DEFAULT_EMCXXFLAGS% ^
    -I %BOOTSTRAP_PATH%\include ^
    --pre-js %BOOTSTRAP_PATH%\src\js_callback.js ^
    %BOOTSTRAP_PATH%\src\js_callback.cpp ^
    --pre-js main-pre.js ^
    --bind ^
    -o main.js ^
    main.cpp
