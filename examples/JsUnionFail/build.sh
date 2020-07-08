#!/bin/bash
set -ueo pipefail
../../build-default.sh -o main.js --pre-js main-pre.js main.cpp
