#!/bin/bash
# Copy this file to build-config.cmd and edit three lines below
source <path-to-emsdk_env.sh> >/dev/null 2>&1 || exit 1
BOOST_PATH=<path-to-boost-includes>
THINK_CELL_PUBLIC_PATH=<path-to-think-cell-public-library-includes>
EMCXX=em++
BOOTSTRAP_PATH=$(dirname "$0")/bootstrap
EMCXX_FLAGS="\
    -s SAFE_HEAP=1 -s ASSERTIONS=2 -s DEMANGLE_SUPPORT=1\
    -D_DEBUG\
    -Wall -Wextra -Werror\
    -g4 --source-map-base http://localhost:8080/\
    -std=c++17 --bind\
    -isystem $THINK_CELL_PUBLIC_PATH\
    -isystem $BOOST_PATH"
