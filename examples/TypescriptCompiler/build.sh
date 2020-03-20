#!/bin/bash
set -ue
../../build-default.sh -o main.js --pre-js main-pre.js main.cpp
