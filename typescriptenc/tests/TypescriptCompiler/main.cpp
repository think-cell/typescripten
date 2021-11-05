#include "../../precompiled.h"

using tc::jst::create_js_object;
using tc::js::string;
using tc::js::ts;
using tc::js::Array;
using tc::js::ReadonlyArray;

int main(int argc, char* argv[]) {
	_ASSERT(2 <= argc);

	ts::CompilerOptions const jsCompilerOptions(create_js_object);
	jsCompilerOptions->noEmitOnError(true);
	jsCompilerOptions->strict(true);
	jsCompilerOptions->target(ts::ScriptTarget::ES5);
	jsCompilerOptions->module(ts::ModuleKind::CommonJS);

	ts::Program const jsProgram = ts::createProgram(
		ReadonlyArray<string>(
			create_js_object,
			tc::make_iterator_range(argv + 1, argv + argc)
		),
		jsCompilerOptions
	);
	ts::EmitResult const jsEmitresult = jsProgram->emit();

	tc::for_each(
		tc::concat(ts::getPreEmitDiagnostics(jsProgram), jsEmitresult->diagnostics()),
		[](ts::Diagnostic const jsDiagnostic) noexcept {
			if (jsDiagnostic->file()) {
				ts::LineAndCharacter const jsLineAndCharacter = (*jsDiagnostic->file())->getLineAndCharacterOfPosition(*jsDiagnostic->start());
				string const jsMessage = ts::flattenDiagnosticMessageText(jsDiagnostic->messageText(), string("\n"));
				printf("%s (%d,%d): %s\n",
					tc::explicit_cast<std::string>((*jsDiagnostic->file())->fileName()).c_str(),
					tc::explicit_cast<int>(jsLineAndCharacter->line()) + 1,
					tc::explicit_cast<int>(jsLineAndCharacter->character()) + 1,
					tc::explicit_cast<std::string>(jsMessage).c_str()
				);
			} else {
				printf("%s\n", tc::explicit_cast<std::string>(ts::flattenDiagnosticMessageText(jsDiagnostic->messageText(), string("\n"))).c_str());
			}
		}
	);

	int iExitCode = jsEmitresult->emitSkipped() ? 1 : 0;
	printf("Process exiting with code '%d'.\n", iExitCode);
	return iExitCode;
}
