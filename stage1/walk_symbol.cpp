#include "precompiled.h"
#include "typescript.d.bootstrap.h"

#include "walk_symbol.h"
#include "mangle.h"

using tc::js::ts;
extern std::optional<tc::js::ts::TypeChecker> g_ojtsTypeChecker;

void PrintSymbolTree(int const nOffset, ts::Symbol const jsymType) noexcept {
	tc::append(std::cerr,
		tc::repeat_n(' ', nOffset),
		"'", FullyQualifiedName(jsymType), "', ",
		"flags=", tc::as_dec(static_cast<int>(jsymType->getFlags())),
		"\n"
	);
	// (*g_ojtsTypeChecker)->getPropertiesOfType((*g_ojtsTypeChecker)->getDeclaredTypeOfSymbol(jsymType): all properties, including derived and methods.
	// jsymType->members(): defined properties/methods, not derived.
	// jsymType()->globalExports: unknown.
	// jsymType()->exports: nested static types/methods/properties
	// (*g_ojtsTypeChecker)->getExportsOfModule(jsymType): same as 'exports', but when called on a module with `export = Foo`, returns members of `Foo`, not `Foo` itself.
	tc::append(std::cerr, tc::repeat_n(' ', nOffset + 2), "members\n");
	if (tc::js::ts_ext::Symbol(jsymType)->members()) {
		tc::for_each(*tc::js::ts_ext::Symbol(jsymType)->members(), [&](ts::Symbol const jsymChild) noexcept { PrintSymbolTree(nOffset + 4, jsymChild); });
	}

	if(static_cast<bool>(jsymType->getFlags() & ts::SymbolFlags::Module)) {
		tc::append(std::cerr, tc::repeat_n(' ', nOffset + 2), "exportsOfModule\n");
		tc::for_each((*g_ojtsTypeChecker)->getExportsOfModule(jsymType),
			[&](ts::Symbol const jsymChild) noexcept { PrintSymbolTree(nOffset + 4, jsymChild); }
		);
	} else if (tc::js::ts_ext::Symbol(jsymType)->exports()) {
		tc::append(std::cerr, tc::repeat_n(' ', nOffset + 2), "exports\n");
		tc::for_each(*tc::js::ts_ext::Symbol(jsymType)->exports(),
			[&](ts::Symbol const jsymChild) noexcept { PrintSymbolTree(nOffset + 4, jsymChild); }
		);
	}

	tc::append(std::cerr, tc::repeat_n(' ', nOffset + 2), "call signatures\n");
	tc::for_each((*g_ojtsTypeChecker)->getSignaturesOfType((*g_ojtsTypeChecker)->getDeclaredTypeOfSymbol(jsymType), ts::SignatureKind::Call),
		[&](ts::Signature const jtsSignature) noexcept {
			tc::append(std::cerr,
				tc::repeat_n(' ', nOffset + 4),
				tc::explicit_cast<std::string>((*g_ojtsTypeChecker)->signatureToString(jtsSignature)),
				"\n"
			);
		}
	);

	tc::append(std::cerr, tc::repeat_n(' ', nOffset + 2), "constructors\n");
	tc::for_each((*g_ojtsTypeChecker)->getSignaturesOfType((*g_ojtsTypeChecker)->getDeclaredTypeOfSymbol(jsymType), ts::SignatureKind::Construct),
		[&](ts::Signature const jtsSignature) noexcept {
			tc::append(std::cerr,
				tc::repeat_n(' ', nOffset + 4),
				tc::explicit_cast<std::string>((*g_ojtsTypeChecker)->signatureToString(jtsSignature)),
				"\n"
			);
		}
	);

	if (auto jointerfacetype = tc::js::ts_ext::isClassOrInterface((*g_ojtsTypeChecker)->getDeclaredTypeOfSymbol(jsymType))) {
		tc::append(std::cerr, tc::repeat_n(' ', nOffset + 2), "base types\n");
		tc::for_each((*g_ojtsTypeChecker)->getBaseTypes(*jointerfacetype),
			[&](tc::js::ts::BaseType const jtsBaseType) noexcept {
				tc::append(std::cerr,
					tc::repeat_n(' ', nOffset + 4),
					tc::explicit_cast<std::string>((*g_ojtsTypeChecker)->typeToString(jtsBaseType)),
					"\n"
				);
			}
		);
	}
}

