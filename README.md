![CI](https://github.com/think-cell/tcjs/workflows/CI/badge.svg)

# typescripten

Generate type-safe JavaScript bindings for C++/emscripten from TypeScript interface definition files. 

If you have a typescript module `MyLib.d.ts`

    declare namespace MyLib {
        function appendNumber(a: string, b: number): string;
    }

Running the **typescripten** compiler on this file will produce the C++ header `MyLib.d.h`

    namespace tc::js_defs {
        using namespace jst; // no ADL
        struct _impl_js_jMyLib;
        using _js_jMyLib = js_ref<_impl_js_jMyLib>;
        struct _impl_js_jMyLib : virtual IObject {
            struct _tcjs_definitions {
                static auto appendNumber(js_string a, double b) noexcept;
            };
        };
        inline auto _impl_js_jMyLib::_tcjs_definitions::appendNumber(js_string a, double b) noexcept {
            return emscripten::val::global("MyLib")["appendNumber"](a, b).template as<js_string>();
        }
    }; // namespace tc::js_defs
    namespace tc::js {
        using MyLib = js_defs::_js_jMyLib;
    } // namespace tc::js


which lets us use `MyLib` from C++ in a type-safe way

    #include "MyLib.d.h"
    #include <iostream>

    int main() {
        std::cout << tc::explicit_cast<std::string>( // Unpack the JavaScript string 
            tc::js::MyLib::appendNumber(
                tc::jst::js_string("foobar"), // Create a JavaScript string
                20 // JavaScript number type maps to double
            )
        ) << std::endl;
    }

See `stage1/tests` for more examples. Some TypeScript constructs are not yet supported. 

Close analogues are Rust's [stdweb](https://github.com/koute/stdweb) and [wasm-bindgen](https://github.com/rustwasm/wasm-bindgen).

# Dependencies

* [Python 3](https://www.python.org/downloads/)
* [Emscripten](https://emscripten.org/) 2.0.18 or newer (Node.js and NPM are included)
* [think-cell public library](https://github.com/think-cell/range/tree/clang_12_cpp2a)
* [boost 1.73.0](https://dl.bintray.com/boostorg/release/1.73.0/source/)
* [ninja](https://ninja-build.org)

# Setup

* Copy `build-config-example.*` to `build-config.*` and edit the files to set the correct paths to emscripten, boost and the think-cell library
* Run `examples/testall.py` to execute some test cases for the elementary **typescripten** C++/JavaScript interop classes in `bootstrap`
* Run `cd stage1` and `./build.sh` or `build.cmd` to build the **typescripten** compiler for typescript interface definition files.  

# Misc thoughts
* C++ callbacks passed to JS are always `noexcept` because exceptions cannot be passed between JS and C++ at the moment.
* Member functions converted to callbacks are always called with a non-const-this.
* Methods for `js_*` types do not have consistent const-qualifiers at the moment, so do not use const qualifiers on them.
* `js_ref` may be inherited from, assuming you're ok with slicing (e.g. `CUniqueDetachableJsFunction`).
  Allowed slicing is argument in favor of `std::is_convertible`/`std::is_constructible`, not
  `tc::is_safely_convertible`, `tc::is_safely_constructible`.
* It may make sense to replace `tc::remove_cvref_t` with `tc::decay_t` in some places so it works with e.g. `vector<bool>::reference`.
* It may make sense to decay subtypes of `js_ref` to `js_ref`.
* `js_union` currently does not force any specific order of arguments, probably it should to some extent
  (even though most instances will be autogenerated).
* Standard `std::variant` construction is somewhat more restrictive than `js_union`:
  the former requires successful overload resolution, the latter requires that the
  argument is convertible to at least one option.

## Namespaces vs classes
TypeScript disambiguates between instantiated namespaces (`ValueModule`) and
non-instantiated (`NamespaceModule`), see `TypeScript/src/compiler/binder.ts:getModuleInstanceStateWorker`.
The former is visible in the emitted JS, the latter is fully erased.
```
namespace Foo {  // NamespaceModule: non-instantiated in JS. May only export (recursively)
                 // interfaces, enums, type aliases. No classes or functions.
    interface MyInterface { }
}
namespace Bar {  // ValueModule: instantiated because class should become a function in JS.
    class MyClass { }
}
```

In C++, we have `namespace` and `class`.
The former, in some sense, may only contain `static` functions and variables.
It may be a good idea to emit some namespaces to `namespace` instead of `js_ref<>`
so `using namespace` works and variable declarations do not.
However, we have to disambiguate them manually and generate slightly different code
for `namespace` and `class`.

## Syntax for static functions and static variables
Interfaces are not emitted to JavaScript.

A class in JavaScript is a special constructor function which should be
called with a `new Foo()` syntax instead of `Foo()`.
Moreover, any function is an object itself, and an object may have any properties it want.
So, a class may have nested classes.
TypeScript will see it as `Class | ValueModule`:
```
export class Foo {  // Class | ValueModule
    someMethod() { }  // Method
    a: number;  // Property
}
export namespace Foo {
    export function someFunction() { }  // Function
    export var a: number;  // FunctionScopedVariable
}
```
Similarly, it's possible to have `Interface | ValueModule`, but it will be emitted
to JS as a simple namespace, because interfaces are not emitted.
```
interface Bar {
    method(): void;
}
namespace Bar {
    var a: number;
    function someFunction() {}
}
```
If we have `Bar` which is `Interface | ValueModule`, in C++ we should make sure that:

1. `Bar` is a valid base type for corresponding classes and interfaces.
2. `Bar` cannot be instantiated on its own.
   Hence, we cannot say "a TS namespace is a `js_ref<>` with default constructor",
   like it was the case with `console()->log(....)`.
3. We can somehow call static functions from `Bar`.

Hence, we conclude that standard C++ syntax should be used: `Bar::someFunction()`.
As `Bar` for interfaces is an alias for `js_ref<>`, we should make sure
`js_ref<>` has corresponding static function and does not have any members with names.

# Naming conventions
* Global variables start with `g_`.
* Member fields start with `m_`.
* All variables are prefixed with a type tag: `<type-tag>Name`.
    * `Name` is camel-case.
    * There should be no duplicates between type tag and name, e.g. `vecstrArguments`, not `vecstrArgumentsVector`.
* Functions' and variables' names are `CamelCase`.
* `int`'s type tag is `n`, e.g. `nArguments`.
* `std::string`'s type tag is `str`.
* `std::vector<T>`'s type tag is `vec<type-tag-of-T>`, name is singular, e.g. `vecstrArguments`.
* `js_ref<T>`'s type tag is `j<type-tag-of-T>`
    * `ts::Symbol`'s type tag is `sym`.
    * `js_optional<T>`'s type tag is `o<type-tag-of-T>`.
    * E.g. `js_ref<js_optional<ts::Symbol>> josymDeclaration;`
* For (almost) singleton objects from TypeScript Compiler API: `jts<exact-type-name-from-typescript>`.
    * "Almost singleton" ~ "does not require any name in local scope, type is enough".
    * The decision is made on a per-class basis, e.g. `Symbol` is never "almost singleton" to avoid confusion.
    * E.g. `(*g_ojtsTypeChecker)`.
* Optional singleton objects are `jots<exact-type-name-from-typescript>`.
