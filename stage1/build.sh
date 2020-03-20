#!/bin/bash
set -ue
source ../build-config.sh
"$EMCXX" $DEFAULT_EMCXXFLAGS \
    -I "$BOOTSTRAP_PATH/include" \
    --pre-js "$BOOTSTRAP_PATH/src/js_callback.js" \
    "$BOOTSTRAP_PATH/src/js_callback.cpp" \
    -o main.js \
    --pre-js main-pre.js \
    main.cpp
