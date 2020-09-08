#!/bin/bash
set -ueo pipefail
source ../../build-config.sh
../../ninja.py main.emscripten debug
