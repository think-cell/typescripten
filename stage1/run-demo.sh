#!/bin/bash
set -ue
source ../build-config.sh
echo "#include \"precompiled.h\"" >demo-module-use.cpp
node main.js demo-module.ts >>demo-module-use.cpp
