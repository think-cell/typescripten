#!/bin/bash
set -ue
../build-default.sh -o demo-module-use.js -include-pch obj/precompiled.h.pch demo-module-use.cpp
