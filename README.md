![CI](https://github.com/think-cell/tcjs/workflows/CI/badge.svg)

# typescripten

Calling JavaScript code from C++ via [emscripten::val](https://emscripten.org/docs/porting/connecting_cpp_and_javascript/embind.html#using-val-to-transliterate-javascript-to-c) offers no type-safety. 

_typescripten_ uses [TypeScript interface definition files](https://github.com/DefinitelyTyped/DefinitelyTyped) to generate type-safe C++ interfaces for JavaScript and TypeScript libraries. 
Compare the JavaScript statements in the comments to the C++ code:

    int main() {
        // var elem = document.createElement("p")
        auto elem = js::document()->createElement(js::string("p"));

        // elem.innerText = "Hello CppCon 2021"
        elem->innerText(js::string("Hello CppCon 2021"));

        // elem.style.fontSize = 20.0
        elem->style()->fontSize(js::string("20vh"));

        // document.body.appendChild(elem)
        js::document()->body()->appendChild(elem);
    }

_typescripten_ uses the [parser API provided by typescript](https://github.com/Microsoft/TypeScript/wiki/Using-the-Compiler-API). It is now self-hosting, i.e., it can the parse interface definition file for the typescript parser itself.

[A short overview of _typescripten_ has been presented at CppNow 2021](https://youtu.be/Cmud1jO__VA)  

[_typescripten_ has been presented more fully at CppCon 2021. The recording will be published later in 2021.](https://sched.co/nvAY)

# Example

Let's say you have a typescript module `MyLib.d.ts` for a JavaScript library you have written yourself, or maybe for a JavaScript API to some web service:

    declare namespace MyLib {
        function appendNumber(a: string, b: number): string;
    }

Running the _typescripten_ compiler on this file will produce the C++ header `MyLib.d.h`

    namespace tc::js_defs {
        using namespace jst; // no ADL
        struct _impl_js_jMyLib;
        using _js_jMyLib = ref<_impl_js_jMyLib>;
        struct _impl_js_jMyLib : virtual object_base {
            struct _tcjs_definitions {
                static auto appendNumber(string a, double b) noexcept;
            };
        };
        inline auto _impl_js_jMyLib::_tcjs_definitions::appendNumber(string a, double b) noexcept {
            return emscripten::val::global("MyLib")["appendNumber"](a, b).template as<string>();
        }
    }; // namespace tc::js_defs
    namespace tc::js {
        using MyLib = js_defs::_js_jMyLib;
    } // namespace tc::js

By including the generated header you can use `MyLib` from C++ in a type-safe way.

    #include "MyLib.d.h"
    #include <iostream>

    int main() {
        std::cout << tc::explicit_cast<std::string>( // Unpack the JavaScript string 
            tc::js::MyLib::appendNumber(
                tc::js::string("foobar"), // Create a JavaScript string
                20 // JavaScript number type maps to double
            )
        ) << std::endl;
    }

See `typescripten/tests` and `typescriptenc/tests` for more examples. 

Close analogues are Rust's [stdweb](https://github.com/koute/stdweb) and [wasm-bindgen](https://github.com/rustwasm/wasm-bindgen).

# TypeScript language support

`typescripten/` contains the code to support basic Typescript types and language features
- `any`, `undefined`, `null` and `string` types
- Support for mixed enums like
```
    enum E {
        a, 
        b = "I'm a string",
        c = 5.0
    }
```
- Passing C++ functions and lambdas as callbacks to JavaScript
- union types `A|B|C` expressed as template class `union_t<A, B, C>`
- JavaScript library types like `Array`, `ReadonlyArray`, `Promise` and `Iterable`. [These should be themselves generated in the future.](https://github.com/think-cell/tcjs/issues/17)

_typescripten_ supports
- type guards
- type aliases
- optional arguments and optional member properties
- generics, see [https://github.com/think-cell/tcjs/issues/3](https://github.com/think-cell/tcjs/issues/3)

Some TypeScript constructs are not yet supported. Better support for generic type constraints and indexed access types are high priority and coming soon. 

# Exploring the TypeScript compiler API

Both the [TypeScript Playground](https://www.typescriptlang.org/play) and the [TypeScript AST Viewer](https://ts-ast-viewer.com/#) (Check the JS debug console!) are invaluable when you want to improve _typescripten_. 

# Dependencies

* [Python 3](https://www.python.org/downloads/)
* [Emscripten](https://emscripten.org/) 2.0.18 or newer (Node.js and NPM are included)
* [think-cell public library](https://github.com/think-cell/range/tree/clang_12_cpp2a)
* [boost 1.73.0](https://dl.bintray.com/boostorg/release/1.73.0/source/)

# Setup

It is easiest to use `typescripten` via CMake. In your CMake file add the following:

```CMake
# Download typescripten (The name must not be changed)
include(FetchContent)
FetchContent_Declare(
  typescripten
  GIT_REPOSITORY https://github.com/think-cell/typescripten
)
FetchContent_MakeAvailable(typescripten)

# Include the CMake helper function
include(${typescripten_SOURCE_DIR}/cmake/TypeScripten.cmake)

# Install the types you need via npm
add_custom_target(install_node_packages npm install)

# Add your executable target
add_executable(test ...)

# Compile the downloaded TypeScript interface definitions (here lib.dom.d.ts)
# into a header and add dependencies so that the build order is 
# install_node_packages < typescripten compiler < test
add_typescripten_target(
  TARGET test
  INPUTS ${PROJECT_SOURCE_DIR}/node_modules/typescript/lib/lib.dom.d.ts
  OUTPUT lib.dom.d.h
  DEPENDSON install_node_packages
)
```

Now you can include `<lib.dom.d.h>` in your C++ files. In order to build your project, download [emscripten](https://emscripten.org/) and boost, and run 

```
source path/to/emsdk/emsdk_env.sh
emcmake cmake -S . -B build 
cmake --build build
```

See `example` and its associated README. 

(All tests still run with the legacy Ninja-based generator and will be moved to CMake as well.) 

# Debugging

`typescriptenc` debug builds are built with DWARF debug information. Google Chrome Dev builds can interactively debug WebAssembly applications with debug info. See [this Chrome blog entry on how to setup Chrome for debugging.](https://developer.chrome.com/blog/wasm-debugging-2020/) 

In order to build with debugging support set the `DEBUG_DEVTOOLS` options. If you build the `typescriptenc_debug` target, cmake will package `typescripten` with webpack so it can run in a browser and it will start a web server for you:

```
source path/to/emsdk/emsdk_env.sh
emcmake cmake -S . -B build -DDEBUG_DEVTOOLS=ON 
cmake --build build --target typescriptenc_debug
```

Now you should be able to open [http://localhost:8000/debug] in Chrome and in the DevTools console you should be able to see the C++ sources, set breakpoints, look at local variables etc. This debug build cannot read local files from disk, so the input file is hard-coded in `debug/index.html`. Change that file and rebuild or change the live version in `build/debug/index.html` and reload the running page.

# Naming conventions
We use [Hungarian Notation](https://en.wikipedia.org/wiki/Hungarian_notation) in our code base, i.e.,
* Global variables start with `g_`
* Member fields start with `m_`
* Constants start with `c_`

All variables are prefixed with a type tag `<type-tag>Name`
* `Name` is camel-case
* `int`'s type tag is `n`, e.g. `nArguments`
* `std::string`'s type tag is `str`
* `std::vector<T>`'s type tag is `vec<type-tag-of-T>`, name is singular, e.g. `vecstrArguments`
* `jst::ref<T>`'s type tag is `j<type-tag-of-T>`
* `ts::Symbol`'s type tag is `sym`.
* `jst::optional<T>`'s type tag is `o<type-tag-of-T>`.

**Example:** `jst::ref<jst::optional<ts::Symbol>> josymDeclaration;`

There should be no duplicates between type tag and name, e.g. `vecstrArguments`, not `vecstrArgumentsVector`

