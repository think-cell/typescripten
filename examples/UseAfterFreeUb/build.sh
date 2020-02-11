#!/bin/bash
set -ue
source ../../build-config.sh
"$EMCXX" $EMCXX_FLAGS \
    -I "$BOOTSTRAP_PATH/include" \
    --pre-js "$BOOTSTRAP_PATH/src/js_callback.js" \
    "$BOOTSTRAP_PATH/src/js_callback.cpp" \
    -o main-pre.js \
    main.cpp
