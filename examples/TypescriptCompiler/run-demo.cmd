@call ..\..\build-config.cmd || exit /b 1
@echo Should print compilation errors and exit with code 1.
node main.js demo-program.ts
