@echo off
call ..\build-config.cmd
echo #include "precompiled.h" >demo-module-use.cpp || exit /b 1
node main.js demo-module.ts >>demo-module-use.cpp || exit /b 1
