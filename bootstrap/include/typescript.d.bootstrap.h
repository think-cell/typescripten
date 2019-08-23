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
struct _jsdefs_ts {
    struct _js_TextRange;
    struct _js_Node;
    struct _js_Declaration;
    struct _js_SourceFileLike;
    struct _js_SourceFile;
    struct _js_Program;
    struct _js_EmitResult;
    struct _js_DiagnosticRelatedInformation;
    struct _js_Diagnostic;
    struct _js_CompilerOptions;
    enum class ModuleKind;
    struct _js_LineAndCharacter;
    enum class ScriptTarget;

    using TextRange = js_ref<_js_TextRange>;
    using Node = js_ref<_js_Node>;
    using Declaration = js_ref<_js_Declaration>;
    using SourceFileLike = js_ref<_js_SourceFileLike>;
    using SourceFile = js_ref<_js_SourceFile>;
    using Program = js_ref<_js_Program>;
    using EmitResult = js_ref<_js_EmitResult>;
    using DiagnosticRelatedInformation = js_ref<_js_DiagnosticRelatedInformation>;
    using Diagnostic = js_ref<_js_Diagnostic>;
    using CompilerOptions = js_ref<_js_CompilerOptions>;
    using LineAndCharacter = js_ref<_js_LineAndCharacter>;

    struct _js_TextRange : virtual IUnknown {
    };

    struct _js_Node : virtual _js_TextRange {
    };

    struct _js_Declaration : virtual _js_Node {
    };

    struct _js_SourceFileLike : virtual IUnknown {
        auto getLineAndCharacterOfPosition(int pos) {
            return _call<LineAndCharacter>("getLineAndCharacterOfPosition", pos);
        }
    };

    struct _js_SourceFile : virtual _js_Declaration, /* deduced */ virtual _js_SourceFileLike {
        auto fileName() { return _getProperty<String>("fileName"); }
        void fileName(String v) { _setProperty("fileName", v); }
    };

    struct _js_Program : virtual IUnknown {
        auto emit() { return _call<EmitResult>("emit"); }
    };

    struct _js_EmitResult : virtual IUnknown {
        auto emitSkipped() { return _getProperty<bool>("emitSkipped"); }
        void emitSkipped(bool v) { _setProperty("emitSkipped", v); }

        auto diagnostics() { return _getProperty<ReadonlyArray<Diagnostic>>("diagnostics"); }
        void diagnostics(ReadonlyArray<Diagnostic> v) { _setProperty("diagnostics", v); }
    };

    struct _js_DiagnosticRelatedInformation : virtual IUnknown {
        auto file() { return _getProperty<std::optional<SourceFile>>("file"); }
        void file(std::optional<SourceFile> v) { _setProperty("file", v); }

        auto start() { return _getProperty<std::optional<int>>("start"); }
        void start(std::optional<int> v) { _setProperty("start", v); }

        /* string | DiagnosticMessageChain; */
        auto messageText() { return _getProperty<js_unknown>("messageText"); }
        void messageText(js_unknown v) { _setProperty("messageText", v); }
    };

    struct _js_Diagnostic : virtual _js_DiagnosticRelatedInformation {
    };

    struct _js_CompilerOptions : virtual IUnknown {
        auto noEmitOnError() { return _getProperty<std::optional<bool>>("noEmitOnError"); }
        void noEmitOnError(std::optional<bool> v) { _setProperty("noEmitOnError", v); }

        auto module() { return _getProperty<std::optional<bool>>("module"); }
        void module(std::optional<ModuleKind> v) { _setProperty("module", v); }

        auto strict() { return _getProperty<std::optional<bool>>("strict"); }
        void strict(std::optional<bool> v) { _setProperty("strict", v); }

        auto target() { return _getProperty<std::optional<ScriptTarget>>("target"); }
        void target(std::optional<ScriptTarget> v) { _setProperty("target", v); }

        static auto _construct() noexcept { return emscripten::val::object(); }
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

    struct _js_LineAndCharacter : virtual IUnknown {
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
};

struct _js_ts : virtual IUnknown, _jsdefs_ts {
    auto getLineAndCharacterOfPosition(SourceFileLike sourceFile, int position) {
        return _call<LineAndCharacter>("getLineAndCharacterOfPosition", sourceFile, position);
    }

    auto createProgram(ReadonlyArray<String> rootNames, CompilerOptions compilerOptions) {
        return _call<Program>("createProgram", rootNames, compilerOptions);
    }

    auto getPreEmitDiagnostics(Program program) {
        return _call<ReadonlyArray<Diagnostic>>("getPreEmitDiagnostics", program);
    }

    auto flattenDiagnosticMessageText(js_unknown messageText, String newLine) {
        return _call<String>("flattenDiagnosticMessageText", messageText, newLine);
    }

   static emscripten::val _construct() { return emscripten::val::global("ts"); }
};

struct ts : js_ref<_js_ts>, _jsdefs_ts {
};
} // namespace no_adl
using no_adl::ts;
} // namespace globals

template<> struct IsJsIntegralEnum<globals::ts::ModuleKind> : std::true_type {};
template<> struct IsJsIntegralEnum<globals::ts::ScriptTarget> : std::true_type {};
} // namespace tc::js
