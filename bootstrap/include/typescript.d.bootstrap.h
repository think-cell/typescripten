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
            return m_emval.call<js_ref<LineAndCharacter>>("getLineAndCharacterOfPosition", pos);
        }
    };

    struct SourceFile : virtual Declaration, /* deduced */ virtual SourceFileLike {
        auto fileName() { return m_emval["fileName"].template as<js_ref<String>>(); }
        void fileName(js_ref<String> v) { m_emval.set("fileName", v); }
    };

    struct Program : virtual IJsBase {
        auto emit() { return m_emval.call<js_ref<EmitResult>>("emit"); }
    };

    struct EmitResult : virtual IJsBase {
        auto emitSkipped() { return m_emval["emitSkipped"].template as<bool>(); }
        void emitSkipped(bool v) { m_emval.set("emitSkipped", v); }

        auto diagnostics() { return m_emval["diagnostics"].template as<js_ref<ReadonlyArray<js_ref<Diagnostic>>>>(); }
        void diagnostics(js_ref<ReadonlyArray<js_ref<Diagnostic>>> v) { m_emval.set("diagnostics", v); }
    };

    struct DiagnosticRelatedInformation : virtual IJsBase {
        auto file() { return m_emval["file"].template as<std::optional<js_ref<SourceFile>>>(); }
        void file(std::optional<js_ref<SourceFile>> v) { m_emval.set("file", v); }

        auto start() { return m_emval["start"].template as<std::optional<int>>(); }
        void start(std::optional<int> v) { m_emval.set("start", v); }

        /* string | DiagnosticMessageChain; */
        auto messageText() { return m_emval["messageText"].template as<js_ref<IAny>>(); }
        void messageText(js_ref<IAny> v) { m_emval.set("messageText", v); }
    };

    struct Diagnostic : virtual DiagnosticRelatedInformation {
    };

    struct CompilerOptions : virtual IJsBase {
        auto noEmitOnError() { return m_emval["noEmitOnError"].template as<std::optional<bool>>(); }
        void noEmitOnError(std::optional<bool> v) { m_emval.set("noEmitOnError", v); }

        auto module() { return m_emval["module"].template as<std::optional<bool>>(); }
        void module(std::optional<ModuleKind> v) { m_emval.set("module", v); }

        auto strict() { return m_emval["strict"].template as<std::optional<bool>>(); }
        void strict(std::optional<bool> v) { m_emval.set("strict", v); }

        auto target() { return m_emval["target"].template as<std::optional<ScriptTarget>>(); }
        void target(std::optional<ScriptTarget> v) { m_emval.set("target", v); }
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
        auto line() { return m_emval["line"].template as<int>(); }
        void line(int v) { m_emval.set("line", v); }

        auto character() { return m_emval["character"].template as<int>(); }
        void character(int v) { m_emval.set("character", v); }
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
        return m_emval.call<js_ref<LineAndCharacter>>("getLineAndCharacterOfPosition", sourceFile, position);
    }

    auto createProgram(js_ref<ReadonlyArray<js_ref<String>>> rootNames, js_ref<CompilerOptions> compilerOptions) {
        return m_emval.call<js_ref<Program>>("createProgram", rootNames, compilerOptions);
    }

    auto getPreEmitDiagnostics(js_ref<Program> program) {
        return m_emval.call<js_ref<ReadonlyArray<js_ref<Diagnostic>>>>("getPreEmitDiagnostics", program);
    }

    auto flattenDiagnosticMessageText(js_ref<IAny> messageText, js_ref<String> newLine) {
        return m_emval.call<js_ref<String>>("flattenDiagnosticMessageText", messageText, newLine);
    }
};
} // namespace no_adl
using no_adl::ts;

inline auto ts() { return js_ref<struct ts>(emscripten::val::global("ts")); }
} // namespace globals

template<> struct IsJsIntegralEnum<globals::ts::ModuleKind> : std::true_type {};
template<> struct IsJsIntegralEnum<globals::ts::ScriptTarget> : std::true_type {};
} // namespace tc::js
