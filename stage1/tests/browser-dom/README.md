1. Copy `lib.dom.d.h` from TypeScript distribution (I use 3.8.3).
2. `build-make` will fail because of:
    1. redefinitions (different overloads of the same function may be the same after conversion).
    2. `Document` is defined as `FunctionScopedVariable | Interface`, which is weird.
