#!/bin/bash
set -ueo pipefail
source ../../../build-config.sh
echo Should print compilation errors and exit with code 1.
node main.js demo-program.ts
