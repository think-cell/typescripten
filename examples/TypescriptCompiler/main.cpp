#include <emscripten/val.h>
#include <stdio.h>
#include <string>
#include "range_defines.h"
#include "explicit_cast.h"
#include "range.h"
#include "typescript.d.bootstrap.h"
#include "js_callback.h"

using emscripten::val; // TODO: make unnecessary
using tc::js::js_ref;
using tc::js::IJsFunction;
using tc::js::globals::ts;
using tc::js::globals::Array;
using tc::js::globals::ReadonlyArray;
using tc::js::globals::String;

int main(int argc, char* argv[]) {
    _ASSERT(2 <= argc);

    js_ref<ts::CompilerOptions> jsCompilerOptions(val::object());
    jsCompilerOptions->noEmitOnError(true);
    jsCompilerOptions->strict(true);
    jsCompilerOptions->target(ts::ScriptTarget::ES5);
    jsCompilerOptions->module(ts::ModuleKind::CommonJS);

    js_ref<Array<js_ref<String>>> jsarrstrFileNames(val::array());
    for (int i = 1; i < argc; i++) {
        jsarrstrFileNames->push(js_ref<String>(val(std::string(argv[i]))));
    }
    js_ref<ts::Program> jsProgram = ts()->createProgram(js_ref<ReadonlyArray<js_ref<String>>>(jsarrstrFileNames.get()), jsCompilerOptions);
    js_ref<ts::EmitResult> jsEmitresult = jsProgram->emit();

    tc::for_each(
        tc::concat(ts()->getPreEmitDiagnostics(jsProgram), jsEmitresult->diagnostics()),
        [](js_ref<ts::Diagnostic> jsDiagnostic) {
            if (jsDiagnostic->file()) {
                js_ref<ts::LineAndCharacter> jsLineAndCharacter = jsDiagnostic->file().value()->getLineAndCharacterOfPosition(jsDiagnostic->start().value());
                js_ref<String> jsMessage = ts()->flattenDiagnosticMessageText(jsDiagnostic->messageText(), js_ref<String>(val("\n")));
                printf("%s (%d,%d): %s\n",
                    std::string((*jsDiagnostic->file())->fileName()).c_str(),
                    jsLineAndCharacter->line() + 1,
                    jsLineAndCharacter->character() + 1,
                    std::string(jsMessage).c_str()
                );
            } else {
                printf("%s\n", std::string(ts()->flattenDiagnosticMessageText(jsDiagnostic->messageText(), js_ref<String>(val("\n")))).c_str());
            }
        }
    );

    int iExitCode = jsEmitresult->emitSkipped() ? 1 : 0;
    printf("Process exiting with code '%d'.\n", iExitCode);
    return iExitCode;
}
