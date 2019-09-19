#include <string>
#include <iostream>
#include <vector>
#include <utility>
#include "explicit_cast.h"
#include "range_defines.h"
#include "range.h"
#include "join_adaptor.h"
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

std::vector<ts::Symbol> g_vjsymEnums, g_vjsymClasses;

bool isEnumInCpp(ts::Symbol jSymbol) {
    return
        jSymbol->getFlags() == static_cast<int>(ts::SymbolFlags::RegularEnum) ||
        jSymbol->getFlags() == static_cast<int>(ts::SymbolFlags::ConstEnum);
}

bool isClassInCpp(ts::Symbol jSymbol) {
    return jSymbol->getFlags() & (
        static_cast<int>(ts::SymbolFlags::Class) |
        static_cast<int>(ts::SymbolFlags::Interface) |
        static_cast<int>(ts::SymbolFlags::ValueModule) |
        static_cast<int>(ts::SymbolFlags::NamespaceModule)
        );
}

void walkType(ts::TypeChecker& jsTypeChecker, int offset, ts::Symbol jSymbol) {
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

    if (isEnumInCpp(jSymbol)) {
        g_vjsymEnums.push_back(jSymbol);
    } else if (isClassInCpp(jSymbol)) {
       g_vjsymClasses.push_back(jSymbol);
    }

    tc::append(std::cout, tc::repeat_n(' ', offset + 2), "members\n");
    if (jSymbol->members()) {
        tc::for_each(*jSymbol->members(), [&](ts::Symbol jChildSymbol) { walkType(jsTypeChecker, offset + 4, jChildSymbol); });
    }

    tc::append(std::cout, tc::repeat_n(' ', offset + 2), "exportsOfModule\n");
    if (jSymbol->exports()) {
        tc::for_each(jsTypeChecker->getExportsOfModule(jSymbol),
            [&](ts::Symbol jChildSymbol) { walkType(jsTypeChecker, offset + 4, jChildSymbol); }
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

    auto mangleSymbolName = [&](ts::Symbol jSymbol) {
        std::string sMangled = "_js_j";
        tc::for_each(std::string(jsTypeChecker->getFullyQualifiedName(jSymbol)), [&](char c) {
            switch (c) {
            case '_': sMangled += "_u"; break;
            case ',': sMangled += "_c"; break;
            case '.': sMangled += "_d"; break;
            case '-': sMangled += "_m"; break;
            case '"': sMangled += "_q"; break;
            default: sMangled += c; break;
            }
        });
        return sMangled;
    };

    std::vector<ts::Symbol> vjsym_exportedModules;
    tc::for_each(jsProgram->getSourceFiles(),
        [&](ts::SourceFile const& jsSourceFile) {
            if (!tc::find_unique<tc::return_bool>(rngFileNames, std::string(jsSourceFile->fileName()))) {
                return;
            }
            auto jsymSourceFileSymbol = jsTypeChecker->getSymbolAtLocation(jsSourceFile);
            if (!jsymSourceFileSymbol) {
                tc::append(std::cout, "Module not found for ", std::string(jsSourceFile->fileName()), "\n");
                return;
            }
            vjsym_exportedModules.push_back(*jsymSourceFileSymbol);
        }
    );

    tc::for_each(
        vjsym_exportedModules,
        [&](ts::Symbol const& jsymSourceFileSymbol) {
            tc::append(std::cout, "Module name is ", std::string(jsymSourceFileSymbol->getName()), "\n");
            walkType(jsTypeChecker, 0, jsymSourceFileSymbol);
        }
    );

    tc::append(std::cout, "\n========== GENERATED CODE ==========\n");

    {
        // Generating enum definitions.
        tc::append(std::cout,
            tc::join(tc::transform(g_vjsymEnums, [&](ts::Symbol jsymEnum) {
                _ASSERT(jsymEnum->exports());
                return tc::concat(
                    "enum class ", mangleSymbolName(jsymEnum), " {\n",
                    tc::join(
                        tc::transform(*jsymEnum->exports(), [&](ts::Symbol jsymOption) {
                            _ASSERTEQUAL(jsymOption->getFlags(), static_cast<int>(ts::SymbolFlags::EnumMember));
                            auto jaDeclarations = jsymOption->declarations();
                            _ASSERTEQUAL(jaDeclarations->length(), 1);
                            auto jDeclaration = ts()->isEnumMember(jaDeclarations[0]);
                            _ASSERT(jDeclaration);
                            auto juOptionValue = jsTypeChecker->getConstantValue(*jDeclaration);
                            _ASSERT(juOptionValue.getEmval().isNumber()); // Computed values of enums are unsupported.
                            return tc::concat(
                                "    ", std::string(jsymOption->getName()), " = ",
                                tc::as_dec(tc::explicit_cast<int>(double(juOptionValue))),
                                ",\n"
                            );
                        })
                    ),
                    "};\n"
                );
            })),
            tc::join(tc::transform(g_vjsymClasses, [&](ts::Symbol jsymClass) {
               return tc::concat("struct ", mangleSymbolName(jsymClass), ";\n");
            })),
            tc::join(tc::transform(g_vjsymClasses, [&](ts::Symbol jsymClass) {
               Array<ts::Symbol> jasymExports(std::initializer_list<ts::Symbol>{});
               if (jsymClass->exports()) {
                   jasymExports = jsTypeChecker->getExportsOfModule(jsymClass);
               }
               // TODO: force eager evaluation to keep jasymExports in scope.
               return tc::explicit_cast<std::string>(tc::concat(
                   "struct ", mangleSymbolName(jsymClass), " {\n",
                   tc::join(
                       tc::transform(
                           tc::filter(jasymExports, [&](ts::Symbol jExportSymbol) {
                               return isEnumInCpp(jExportSymbol) || isClassInCpp(jExportSymbol);
                           }),
                           [&](ts::Symbol jExportSymbol) {
                               return tc::concat(
                                   "    using ",
                                   std::string(jExportSymbol->getName()),
                                   " = js_ref<",
                                   mangleSymbolName(jExportSymbol),
                                   ">;\n"
                               );
                           }
                       )
                   ),
                   "};\n"
               ));
            }))
        );
    }
    return 0;
}
