#!/bin/bash
# Copy this file to build-config.cmd and edit three variables below
EMCXX=<path-to-em++>
BOOST_PATH=<path-to-boost-includes>
THINK_CELL_PUBLIC_PATH=<path-to-think-cell-public-library-includes>
BOOTSTRAP_PATH=$(dirname "$0")/bootstrap
EMCXX_FLAGS="\
    -s SAFE_HEAP=1 -s ASSERTIONS=2 -s WASM=1\
    -Wall -Wextra -Werror\
    -g4 --source-map-base http://localhost:8080/\
    -std=c++17 --bind\
    -isystem $THINK_CELL_PUBLIC_PATH\
    -isystem $BOOST_PATH"
