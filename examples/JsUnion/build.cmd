@call ..\..\build-config.cmd
%EMCXX% %EMCXX_FLAGS% ^
    -I %BOOTSTRAP_PATH%\include ^
    --pre-js %BOOTSTRAP_PATH%\src\js_callback.js ^
    %BOOTSTRAP_PATH%\src\js_callback.cpp ^
    -o main.js ^
    main.cpp
