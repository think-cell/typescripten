#include <stdio.h>
#include <string>
#include "range_defines.h"
#include "range.h"
#include "js_bootstrap.h"
#include "typescript.d.bootstrap.h"

using tc::js::js_string;
using tc::js::globals::ts;
using tc::js::globals::Array;
using tc::js::globals::ReadonlyArray;

int main(int argc, char* argv[]) {
    _ASSERT(2 <= argc);

    ts::CompilerOptions jsCompilerOptions;
    jsCompilerOptions->strict(true);
    jsCompilerOptions->target(ts::ScriptTarget::ES5);
    jsCompilerOptions->module(ts::ModuleKind::CommonJS);

    auto rngFileNames = tc::make_iterator_range(argv + 1, argv + argc);
    ts::Program jsProgram = ts()->createProgram(ReadonlyArray<js_string>(rngFileNames), jsCompilerOptions);

    ts::TypeChecker jsTypeChecker = jsProgram->getTypeChecker();

    _ASSERTEQUAL(0, ts()->getPreEmitDiagnostics(jsProgram)->length());

    tc::for_each(
        tc::filter(
            jsProgram->getSourceFiles(),
            [&](ts::SourceFile const& jsSourceFile) {
                return tc::find_unique<tc::return_bool>(rngFileNames, std::string(jsSourceFile->fileName()));
            }
        ),
        [&](ts::SourceFile const& jsSourceFile) {
            auto jsChildren = jsSourceFile->getChildren();
            tc::for_each(jsTypeChecker->getSymbolsInScope(jsChildren[0], ts::SymbolFlags::ModuleMember), [&](ts::Symbol symbol) {
                printf("name is %s\n", std::string(symbol->name()).c_str());
            });

            _ASSERTEQUAL(jsChildren->length(), 2);
            _ASSERTEQUAL(jsChildren[0]->kind(), ts::SyntaxKind::SyntaxList);
            _ASSERTEQUAL(jsChildren[1]->kind(), ts::SyntaxKind::EndOfFileToken);
            tc::for_each(jsChildren[0]->getChildren(),
                [&](ts::Node jnodeTopLevel) {
                    if (auto optjvarstmtTopLevel = ts()->isVariableStatement(jnodeTopLevel)) {
                        printf("variable statement\n");
                    } else if (auto optjifacedeclTopLevel = ts()->isInterfaceDeclaration(jnodeTopLevel)) {
                        printf("interface declaration\n");
                    } else if (auto optjfuncdeclTopLevel = ts()->isFunctionDeclaration(jnodeTopLevel)) {
                        printf("function declaration\n");
                    } else if (auto optjtypealiasTopLevel = ts()->isTypeAliasDeclaration(jnodeTopLevel)) {
                        printf("type alias declaration\n");
                    } else if (auto optjmoddeclTopLevel = ts()->isModuleDeclaration(jnodeTopLevel)) {
                        printf("module declaration '%s'\n", std::string((*optjmoddeclTopLevel)->name()->text()).c_str());
                    } else {
                        printf("!!! unknown top level node: %d\n", jnodeTopLevel->kind());
                    }
                }
            );
        }
    );
    return 0;
}
