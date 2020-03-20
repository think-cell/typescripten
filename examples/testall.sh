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
        popd
    fi
done
