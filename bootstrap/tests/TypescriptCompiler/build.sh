#!/bin/bash
set -ueo pipefail
source ../../build-config.sh
npm install
../../ninja.py main.emscripten debug