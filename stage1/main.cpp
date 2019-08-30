#include <string>
#include <iostream>
#include <vector>
#include "explicit_cast.h"
#include "range_defines.h"
#include "range.h"
#include "js_bootstrap.h"
#include "typescript.d.bootstrap.h"

using tc::js::js_string;
using tc::js::js_unknown;
using tc::js::globals::console;
using tc::js::globals::ts;
using tc::js::globals::Array;
using tc::js::globals::ReadonlyArray;

namespace tc {
template<>
struct appender_type<std::ostream> {
   using sink_value_type = char;

   appender_type(std::ostream& os) : m_os(os) {}

   void operator()(char c) const& noexcept {
       m_os.put(c);
   }

private:
   std::ostream& m_os;
};
}; // namespace tc

void printTree(ts::TypeChecker& jsTypeChecker, int offset, ts::Symbol jSymbol) {
    std::string s;
    tc::append(std::cout,
        tc::repeat_n(' ', offset),
        "'", std::string(jsTypeChecker->getFullyQualifiedName(jSymbol)), "', ",
        "flags=", tc::as_dec(jSymbol->getFlags()),
        "\n"
    );
    // jsTypeChecker->getPropertiesOfType(jsTypeChecker->getDeclaredTypeOfSymbol(jSymbol): all properties, including derived and methods.
    // jSymbol->members(): defined properties/methods, not derived.
    // jsSymbol()->globalExports: unknown.
    // jsSymbol()->exports: nested static types/methods/properties
    // jsTypeChecker->getExportsOfModule(jSymbol): same as 'exports', but when called on a module with `export = Foo`, returns members of `Foo`, not `Foo` itself.

    tc::append(std::cout, tc::repeat_n(' ', offset + 2), "members\n");
    if (jSymbol->members()) {
        tc::for_each(*jSymbol->members(), [&](ts::Symbol jChildSymbol) { printTree(jsTypeChecker, offset + 4, jChildSymbol); });
    }

    tc::append(std::cout, tc::repeat_n(' ', offset + 2), "exports\n");
    if (jSymbol->exports()) {
        tc::for_each(jsTypeChecker->getExportsOfModule(jSymbol),
            [&](ts::Symbol jChildSymbol) { printTree(jsTypeChecker, offset + 4, jChildSymbol); }
        );
    }

    tc::append(std::cout, tc::repeat_n(' ', offset + 2), "call signatures\n");
    tc::for_each(jsTypeChecker->getSignaturesOfType(jsTypeChecker->getDeclaredTypeOfSymbol(jSymbol), ts::SignatureKind::Call),
        [&](ts::Signature jSignature) {
            tc::append(std::cout,
                tc::repeat_n(' ', offset + 4),
                std::string(jsTypeChecker->signatureToString(jSignature)),
                "\n"
            );
        }
    );

    tc::append(std::cout, tc::repeat_n(' ', offset + 2), "constructors\n");
    tc::for_each(jsTypeChecker->getSignaturesOfType(jsTypeChecker->getDeclaredTypeOfSymbol(jSymbol), ts::SignatureKind::Construct),
        [&](ts::Signature jSignature) {
            tc::append(std::cout,
                tc::repeat_n(' ', offset + 4),
                std::string(jsTypeChecker->signatureToString(jSignature)),
                "\n"
            );
        }
    );
}

int main(int argc, char* argv[]) {
    _ASSERT(2 <= argc);

    ts::CompilerOptions jsCompilerOptions;
    jsCompilerOptions->strict(true);
    jsCompilerOptions->target(ts::ScriptTarget::ES5);
    jsCompilerOptions->module(ts::ModuleKind::CommonJS);

    auto rngFileNames = tc::make_iterator_range(argv + 1, argv + argc);
    ts::Program jsProgram = ts()->createProgram(ReadonlyArray<js_string>(rngFileNames), jsCompilerOptions);

    ts::TypeChecker jsTypeChecker = jsProgram->getTypeChecker();

    {
        auto jDiagnostics = ts()->getPreEmitDiagnostics(jsProgram);
        if (jDiagnostics->length()) {
            console()->log(ts()->formatDiagnosticsWithColorAndContext(jDiagnostics, ts()->createCompilerHost(jsCompilerOptions)));
            return 1;
        }
    }

    tc::for_each(
        tc::filter(
            jsProgram->getSourceFiles(),
            [&](ts::SourceFile const& jsSourceFile) {
                return tc::find_unique<tc::return_bool>(rngFileNames, std::string(jsSourceFile->fileName()));
            }
        ),
        [&](ts::SourceFile const& jsSourceFile) {
            auto jsymSourceFileSymbol = jsTypeChecker->getSymbolAtLocation(jsSourceFile);
            if (!jsymSourceFileSymbol) {
                tc::append(std::cout, "Module not found for ", std::string(jsSourceFile->fileName()), "\n");
                return;
            }
            tc::append(std::cout, "Module name is ", std::string((*jsymSourceFileSymbol)->getName()), "\n");
            printTree(jsTypeChecker, 0, *jsymSourceFileSymbol);
            tc::append(std::cout, "syntax children:\n");

            auto jsChildren = jsSourceFile->getChildren();

            _ASSERTEQUAL(jsChildren->length(), 2);
            _ASSERTEQUAL(jsChildren[0]->kind(), ts::SyntaxKind::SyntaxList);
            _ASSERTEQUAL(jsChildren[1]->kind(), ts::SyntaxKind::EndOfFileToken);
            tc::for_each(jsChildren[0]->getChildren(),
                [&](ts::Node jnodeTopLevel) {
                    if (auto optjvarstmtTopLevel = ts()->isVariableStatement(jnodeTopLevel)) {
                        tc::append(std::cout, "variable statement\n");
                    } else if (auto optjifacedeclTopLevel = ts()->isInterfaceDeclaration(jnodeTopLevel)) {
                        tc::append(std::cout, "interface declaration\n");
                    } else if (auto optjfuncdeclTopLevel = ts()->isFunctionDeclaration(jnodeTopLevel)) {
                        tc::append(std::cout, "function declaration\n");
                    } else if (auto optjtypealiasTopLevel = ts()->isTypeAliasDeclaration(jnodeTopLevel)) {
                        tc::append(std::cout, "type alias declaration\n");
                    } else if (auto optjmoddeclTopLevel = ts()->isModuleDeclaration(jnodeTopLevel)) {
                        tc::append(std::cout, "module declaration '", std::string((*optjmoddeclTopLevel)->name()->text()), "'\n");
                    } else {
//                        tc::append(std::cout, "!!! unknown top level node: ", tc::as_dec(static_cast<int>(jnodeTopLevel->kind())), "\n");
                    }
                }
            );
        }
    );
    return 0;
}
