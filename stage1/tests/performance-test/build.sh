#!/bin/bash
set -ueo pipefail
source ../../../build-config.sh
../../node_modules/typescript/bin/tsc --strict --declaration MyLib.ts
../../../ninja.py main.emscripten release
