#pragma once

#include <emscripten/val.h>
#include <optional>
#include <string>
#include "js_types.h"
#include "js_callback.h"
#include "js_bootstrap.h"

namespace tc::js {
namespace globals {
namespace no_adl {
struct ts : virtual IJsBase {
    struct TextRange;
    struct Node;
    struct Declaration;
    struct SourceFileLike;
    struct SourceFile;
    struct Program;
    struct EmitResult;
    struct DiagnosticRelatedInformation;
    struct Diagnostic;
    struct CompilerOptions;
    enum class ModuleKind;
    struct LineAndCharacter;
    enum class ScriptTarget;

    struct TextRange : virtual IJsBase {
    };

    struct Node : virtual TextRange {
    };

    struct Declaration : virtual Node {
    };

    struct SourceFileLike : virtual IJsBase {
        auto getLineAndCharacterOfPosition(int pos) {
            return _call<js_ref<LineAndCharacter>>("getLineAndCharacterOfPosition", pos);
        }
    };

    struct SourceFile : virtual Declaration, /* deduced */ virtual SourceFileLike {
        auto fileName() { return _getProperty<js_ref<String>>("fileName"); }
        void fileName(js_ref<String> v) { _setProperty("fileName", v); }
    };

    struct Program : virtual IJsBase {
        auto emit() { return _call<js_ref<EmitResult>>("emit"); }
    };

    struct EmitResult : virtual IJsBase {
        auto emitSkipped() { return _getProperty<bool>("emitSkipped"); }
        void emitSkipped(bool v) { _setProperty("emitSkipped", v); }

        auto diagnostics() { return _getProperty<js_ref<ReadonlyArray<js_ref<Diagnostic>>>>("diagnostics"); }
        void diagnostics(js_ref<ReadonlyArray<js_ref<Diagnostic>>> v) { _setProperty("diagnostics", v); }
    };

    struct DiagnosticRelatedInformation : virtual IJsBase {
        auto file() { return _getProperty<std::optional<js_ref<SourceFile>>>("file"); }
        void file(std::optional<js_ref<SourceFile>> v) { _setProperty("file", v); }

        auto start() { return _getProperty<std::optional<int>>("start"); }
        void start(std::optional<int> v) { _setProperty("start", v); }

        /* string | DiagnosticMessageChain; */
        auto messageText() { return _getProperty<js_ref<IAny>>("messageText"); }
        void messageText(js_ref<IAny> v) { _setProperty("messageText", v); }
    };

    struct Diagnostic : virtual DiagnosticRelatedInformation {
    };

    struct CompilerOptions : virtual IJsBase {
        auto noEmitOnError() { return _getProperty<std::optional<bool>>("noEmitOnError"); }
        void noEmitOnError(std::optional<bool> v) { _setProperty("noEmitOnError", v); }

        auto module() { return _getProperty<std::optional<bool>>("module"); }
        void module(std::optional<ModuleKind> v) { _setProperty("module", v); }

        auto strict() { return _getProperty<std::optional<bool>>("strict"); }
        void strict(std::optional<bool> v) { _setProperty("strict", v); }

        auto target() { return _getProperty<std::optional<ScriptTarget>>("target"); }
        void target(std::optional<ScriptTarget> v) { _setProperty("target", v); }
    };

    enum class ModuleKind {
        None = 0,
        CommonJS = 1,
        AMD = 2,
        UMD = 3,
        System = 4,
        ES2015 = 5,
        ESNext = 6
    };

    struct LineAndCharacter : virtual IJsBase {
        auto line() { return _getProperty<int>("line"); }
        void line(int v) { _setProperty("line", v); }

        auto character() { return _getProperty<int>("character"); }
        void character(int v) { _setProperty("character", v); }
    };

    enum class ScriptTarget {
        ES3 = 0,
        ES5 = 1,
        ES2015 = 2,
        ES2016 = 3,
        ES2017 = 4,
        ES2018 = 5,
        ES2019 = 6,
        ESNext = 7,
        JSON = 100,
        Latest = 7
    };

    auto getLineAndCharacterOfPosition(js_ref<SourceFileLike> sourceFile, int position) {
        return _call<js_ref<LineAndCharacter>>("getLineAndCharacterOfPosition", sourceFile, position);
    }

    auto createProgram(js_ref<ReadonlyArray<js_ref<String>>> rootNames, js_ref<CompilerOptions> compilerOptions) {
        return _call<js_ref<Program>>("createProgram", rootNames, compilerOptions);
    }

    auto getPreEmitDiagnostics(js_ref<Program> program) {
        return _call<js_ref<ReadonlyArray<js_ref<Diagnostic>>>>("getPreEmitDiagnostics", program);
    }

    auto flattenDiagnosticMessageText(js_ref<IAny> messageText, js_ref<String> newLine) {
        return _call<js_ref<String>>("flattenDiagnosticMessageText", messageText, newLine);
    }
};
} // namespace no_adl
using no_adl::ts;

inline auto ts() { return js_ref<struct ts>(emscripten::val::global("ts")); }
} // namespace globals

template<> struct IsJsIntegralEnum<globals::ts::ModuleKind> : std::true_type {};
template<> struct IsJsIntegralEnum<globals::ts::ScriptTarget> : std::true_type {};
} // namespace tc::js
