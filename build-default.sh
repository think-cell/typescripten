#!/bin/bash/
set -ue
source "$(dirname ${BASH_SOURCE[0]})/build-config.sh"
$EMCXX \
    $EMCXXFLAGS -std=c++17\
    -isystem "$THINK_CELL_PUBLIC_PATH"\
    -isystem "$BOOST_PATH"\
    $EMLDFLAGS --bind
    "$@"
