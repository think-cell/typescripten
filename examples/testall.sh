#!/bin/bash
for i in *; do
    if [ -d "$i" ]; then
        pushd "$i"
        if [ -e "get-dependencies.sh" ]; then
            ./get-dependencies.sh || exit 1
        fi
        ./build.sh || exit 1
        if [ -e "test.sh" ]; then
            ./test.sh || exit 1
        fi
        if [ -e "build-make.sh" ]; then
            ./build-make.sh clean || exit 1
            ./build-make.sh
            if [ -e "test.sh" ]; then
                ./test.sh || exit 1
            fi
        fi
        popd
    fi
done
