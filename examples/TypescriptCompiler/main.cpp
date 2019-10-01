#include <stdio.h>
#include <string>
#include "range_defines.h"
#include "range.h"
#include "typescript.d.bootstrap.h"

using tc::js::js_string;
using tc::js::globals::ts;
using tc::js::globals::Array;
using tc::js::globals::ReadonlyArray;

int main(int argc, char* argv[]) {
	_ASSERT(2 <= argc);

	ts::CompilerOptions jsCompilerOptions;
	jsCompilerOptions->noEmitOnError(true);
	jsCompilerOptions->strict(true);
	jsCompilerOptions->target(ts::ScriptTarget::ES5);
	jsCompilerOptions->module(ts::ModuleKind::CommonJS);

	ts::Program jsProgram = ts()->createProgram(
		ReadonlyArray<js_string>(
			tc::make_iterator_range(argv + 1, argv + argc)
		),
		jsCompilerOptions
	);
	ts::EmitResult jsEmitresult = jsProgram->emit();

	tc::for_each(
		tc::concat(ts()->getPreEmitDiagnostics(jsProgram), jsEmitresult->diagnostics()),
		[](ts::Diagnostic jsDiagnostic) {
			if (jsDiagnostic->file()) {
				ts::LineAndCharacter jsLineAndCharacter = (*jsDiagnostic->file())->getLineAndCharacterOfPosition(*jsDiagnostic->start());
				js_string jsMessage = ts()->flattenDiagnosticMessageText(jsDiagnostic->messageText(), js_string("\n"));
				printf("%s (%d,%d): %s\n",
					std::string((*jsDiagnostic->file())->fileName()).c_str(),
					jsLineAndCharacter->line() + 1,
					jsLineAndCharacter->character() + 1,
					std::string(jsMessage).c_str()
				);
			} else {
				printf("%s\n", std::string(ts()->flattenDiagnosticMessageText(jsDiagnostic->messageText(), js_string("\n"))).c_str());
			}
		}
	);

	int iExitCode = jsEmitresult->emitSkipped() ? 1 : 0;
	printf("Process exiting with code '%d'.\n", iExitCode);
	return iExitCode;
}
