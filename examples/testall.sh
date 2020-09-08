#!/bin/bash
set -ueo pipefail
for i in *; do
    if [ -d "$i" ]; then
        pushd "$i"
        ./build.sh
        if [ -e "test.sh" ]; then
            ./test.sh
        fi
        popd
    fi
done
