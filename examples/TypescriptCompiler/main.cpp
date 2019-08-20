#include <emscripten/val.h>
#include <stdio.h>
#include <string>
#include "range_defines.h"
#include "explicit_cast.h"
#include "range.h"
#include "typescript.d.bootstrap.h"
#include "js_callback.h"

using tc::js::js_ref;
using tc::js::IJsFunction;
using tc::js::globals::ts;
using tc::js::globals::Array;
using tc::js::globals::ReadonlyArray;
using tc::js::globals::String;

int main(int argc, char* argv[]) {
    _ASSERT(2 <= argc);

    js_ref<ts::CompilerOptions> jsCompilerOptions(emscripten::val::object());
    jsCompilerOptions->noEmitOnError(true);
    jsCompilerOptions->strict(true);
    jsCompilerOptions->target(ts::ScriptTarget::ES5);
    jsCompilerOptions->module(ts::ModuleKind::CommonJS);

    js_ref<ts::Program> jsProgram = ts()->createProgram(
        tc::explicit_cast<js_ref<ReadonlyArray<js_ref<String>>>>(
            tc::make_iterator_range(argv + 1, argv + argc)
        ),
        jsCompilerOptions
    );
    js_ref<ts::EmitResult> jsEmitresult = jsProgram->emit();

    tc::for_each(
        tc::concat(ts()->getPreEmitDiagnostics(jsProgram), jsEmitresult->diagnostics()),
        [](js_ref<ts::Diagnostic> jsDiagnostic) {
            if (jsDiagnostic->file()) {
                js_ref<ts::LineAndCharacter> jsLineAndCharacter = jsDiagnostic->file().value()->getLineAndCharacterOfPosition(jsDiagnostic->start().value());
                js_ref<String> jsMessage = ts()->flattenDiagnosticMessageText(jsDiagnostic->messageText(), tc::explicit_cast<js_ref<String>>("\n"));
                printf("%s (%d,%d): %s\n",
                    std::string((*jsDiagnostic->file())->fileName()).c_str(),
                    jsLineAndCharacter->line() + 1,
                    jsLineAndCharacter->character() + 1,
                    std::string(jsMessage).c_str()
                );
            } else {
                printf("%s\n", std::string(ts()->flattenDiagnosticMessageText(jsDiagnostic->messageText(), tc::explicit_cast<js_ref<String>>("\n"))).c_str());
            }
        }
    );

    int iExitCode = jsEmitresult->emitSkipped() ? 1 : 0;
    printf("Process exiting with code '%d'.\n", iExitCode);
    return iExitCode;
}
