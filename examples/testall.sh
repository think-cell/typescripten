#!/bin/bash
set -ueo pipefail
for i in *; do
    if [ -d "$i" ]; then
        pushd "$i"
        if [ -e "get-dependencies.sh" ]; then
            ./get-dependencies.sh
        fi
        ./build.sh
        if [ -e "test.sh" ]; then
            ./test.sh
        fi
        if [ -e "build-make.sh" ]; then
            ./build-make.sh clean
            ./build-make.sh
            if [ -e "test.sh" ]; then
                ./test.sh
            fi
        fi
        popd
    fi
done
