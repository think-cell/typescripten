#!/bin/bash
# Copy this file to build-config.cmd and edit three lines below
source <path-to-emsdk_env.sh> >/dev/null 2>&1 || exit 1
export BOOST_PATH=<path-to-boost-includes>
export THINK_CELL_PUBLIC_PATH=<path-to-think-cell-public-library-includes>
export EMCXX=em++
export BOOTSTRAP_PATH=$(dirname "$0")/bootstrap
export EMCXXFLAGS="\
	-D_DEBUG -Wall -Wextra -Werror\
	-g -s SAFE_HEAP=1 -s ASSERTIONS=2 -s DEMANGLE_SUPPORT=1"
export EMLDFLAGS="\
    -g4 --source-map-base http://localhost:8080/"
