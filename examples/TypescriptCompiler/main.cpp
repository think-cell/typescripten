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

    js_ref<ts::CompilerOptions> options(val::object());
    options->noEmitOnError(true);
    options->strict(true);
    options->target(ts::ScriptTarget::ES5);
    options->module(ts::ModuleKind::CommonJS);

    js_ref<Array<js_ref<String>>> fileNames(val::array());
    for (int i = 1; i < argc; i++) {
        fileNames->push(js_ref<String>(val(std::string(argv[i]))));
    }
    js_ref<ts::Program> program = ts()->createProgram(js_ref<ReadonlyArray<js_ref<String>>>(fileNames.get()), options);
    js_ref<ts::EmitResult> emitResult = program->emit();

    tc::for_each(
        tc::concat(ts()->getPreEmitDiagnostics(program), emitResult->diagnostics()),
        [](js_ref<ts::Diagnostic> diagnostic) {
            if (diagnostic->file()) {
                js_ref<ts::LineAndCharacter> lineAndCharacter = diagnostic->file().value()->getLineAndCharacterOfPosition(diagnostic->start().value());
                js_ref<String> message = ts()->flattenDiagnosticMessageText(diagnostic->messageText(), js_ref<String>(val("\n")));
                printf("%s (%d,%d): %s\n",
                    std::string((*diagnostic->file())->fileName()).c_str(),
                    lineAndCharacter->line() + 1,
                    lineAndCharacter->character() + 1,
                    std::string(message).c_str()
                );
            } else {
                printf("%s\n", std::string(ts()->flattenDiagnosticMessageText(diagnostic->messageText(), js_ref<String>(val("\n")))).c_str());
            }
        }
    );

    int exitCode = emitResult->emitSkipped() ? 1 : 0;
    printf("Process exiting with code '%d'.\n", exitCode);
    return exitCode;
}
