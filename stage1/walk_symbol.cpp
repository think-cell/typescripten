#include "precompiled.h"
#include "typescript.d.bootstrap.h"
#include "walk_symbol.h"

using tc::js::ts;

std::vector<ts::Symbol> g_vecjsymEnum, g_vecjsymClass;

bool IsEnumInCpp(ts::Symbol const jsymType) noexcept {
	return
		ts::SymbolFlags::RegularEnum == jsymType->getFlags() ||
		ts::SymbolFlags::ConstEnum == jsymType->getFlags();
}

bool IsClassInCpp(ts::Symbol const jsymType) noexcept {
	return
		ts::SymbolFlags::Class == jsymType->getFlags() ||
		ts::SymbolFlags::Interface == jsymType->getFlags() ||
		(ts::SymbolFlags::Interface | ts::SymbolFlags::FunctionScopedVariable) == jsymType->getFlags() ||
		ts::SymbolFlags::ValueModule == jsymType->getFlags() ||
		(ts::SymbolFlags::ValueModule | ts::SymbolFlags::Interface) == jsymType->getFlags() ||
		(ts::SymbolFlags::NamespaceModule | ts::SymbolFlags::Interface) == jsymType->getFlags() ||
		ts::SymbolFlags::NamespaceModule == jsymType->getFlags();
}

void WalkSymbol(ts::TypeChecker const& jtsTypeChecker, int const nOffset, ts::Symbol const jsymType) noexcept {
	tc::append(std::cerr,
		tc::repeat_n(' ', nOffset),
		"'", tc::explicit_cast<std::string>(jtsTypeChecker->getFullyQualifiedName(jsymType)), "', ",
		"flags=", tc::as_dec(static_cast<int>(jsymType->getFlags())),
		"\n"
	);
	// jtsTypeChecker->getPropertiesOfType(jtsTypeChecker->getDeclaredTypeOfSymbol(jsymType): all properties, including derived and methods.
	// jsymType->members(): defined properties/methods, not derived.
	// jsymType()->globalExports: unknown.
	// jsymType()->exports: nested static types/methods/properties
	// jtsTypeChecker->getExportsOfModule(jsymType): same as 'exports', but when called on a module with `export = Foo`, returns members of `Foo`, not `Foo` itself.

	if (IsEnumInCpp(jsymType)) {
		tc::cont_emplace_back(g_vecjsymEnum, jsymType);
	} else if (IsClassInCpp(jsymType)) {
		tc::cont_emplace_back(g_vecjsymClass, jsymType);
	}

	tc::append(std::cerr, tc::repeat_n(' ', nOffset + 2), "members\n");
	if (tc::js::ts_ext::Symbol(jsymType)->members()) {
		tc::for_each(*tc::js::ts_ext::Symbol(jsymType)->members(), [&](ts::Symbol const jsymChild) noexcept { WalkSymbol(jtsTypeChecker, nOffset + 4, jsymChild); });
	}

	if (jsymType->getFlags() & ts::SymbolFlags::Module) {
		tc::append(std::cerr, tc::repeat_n(' ', nOffset + 2), "exportsOfModule\n");
		tc::for_each(jtsTypeChecker->getExportsOfModule(jsymType),
			[&](ts::Symbol const jsymChild) noexcept { WalkSymbol(jtsTypeChecker, nOffset + 4, jsymChild); }
		);
	} else if (tc::js::ts_ext::Symbol(jsymType)->exports()) {
		tc::append(std::cerr, tc::repeat_n(' ', nOffset + 2), "exports\n");
		tc::for_each(*tc::js::ts_ext::Symbol(jsymType)->exports(),
			[&](ts::Symbol const jsymChild) noexcept { WalkSymbol(jtsTypeChecker, nOffset + 4, jsymChild); }
		);
	}

	tc::append(std::cerr, tc::repeat_n(' ', nOffset + 2), "call signatures\n");
	tc::for_each(jtsTypeChecker->getSignaturesOfType(jtsTypeChecker->getDeclaredTypeOfSymbol(jsymType), ts::SignatureKind::Call),
		[&](ts::Signature const jtsSignature) noexcept {
			tc::append(std::cerr,
				tc::repeat_n(' ', nOffset + 4),
				tc::explicit_cast<std::string>(jtsTypeChecker->signatureToString(jtsSignature)),
				"\n"
			);
		}
	);

	tc::append(std::cerr, tc::repeat_n(' ', nOffset + 2), "constructors\n");
	tc::for_each(jtsTypeChecker->getSignaturesOfType(jtsTypeChecker->getDeclaredTypeOfSymbol(jsymType), ts::SignatureKind::Construct),
		[&](ts::Signature const jtsSignature) noexcept {
			tc::append(std::cerr,
				tc::repeat_n(' ', nOffset + 4),
				tc::explicit_cast<std::string>(jtsTypeChecker->signatureToString(jtsSignature)),
				"\n"
			);
		}
	);

	if (auto jointerfacetype = tc::js::ts_ext::isClassOrInterface(jtsTypeChecker->getDeclaredTypeOfSymbol(jsymType))) {
		tc::append(std::cerr, tc::repeat_n(' ', nOffset + 2), "base types\n");
		tc::for_each(jtsTypeChecker->getBaseTypes(*jointerfacetype),
			[&](tc::js::ts_ext::BaseType const jtsBaseType) noexcept {
				tc::append(std::cerr,
					tc::repeat_n(' ', nOffset + 4),
					tc::explicit_cast<std::string>(jtsTypeChecker->typeToString(jtsBaseType)),
					"\n"
				);
			}
		);
	}
}

std::vector<ts::Symbol> ListSourceFileTopLevel(ts::TypeChecker const& jtsTypeChecker, ts::SourceFile const jtsSourceFile) noexcept {
	std::vector<ts::Symbol> vecjsymTopLevel;
	ts::forEachChild(jtsSourceFile, tc::jst::js_lambda_wrap([&](ts::Node jnodeChild) noexcept -> tc::jst::js_unknown {
		if (auto const jotsFunctionDeclaration = tc::js::ts_ext::isFunctionDeclaration(jnodeChild)) {
			tc::cont_emplace_back(vecjsymTopLevel, jtsTypeChecker->getSymbolAtLocation(*tc::js::ts_ext::name(*jotsFunctionDeclaration)));
		} else if (auto const jotsVariableStatement = tc::js::ts_ext::isVariableStatement(jnodeChild)) {
			tc::for_each(tc::js::ts_ext::MakeReadOnlyArray<ts::VariableDeclaration>((*jotsVariableStatement)->declarationList()->declarations()), [&](ts::VariableDeclaration const jtsVariableDeclaration) {
				tc::cont_emplace_back(vecjsymTopLevel, jtsTypeChecker->getSymbolAtLocation(jtsVariableDeclaration->name()));
			});
		} else if (auto const jotsClassDeclaration = tc::js::ts_ext::isClassDeclaration(jnodeChild)) {
			tc::cont_emplace_back(vecjsymTopLevel, jtsTypeChecker->getSymbolAtLocation(*tc::js::ts_ext::name(*jotsClassDeclaration)));
		} else if (auto const jotsInterfaceDeclaration = tc::js::ts_ext::isInterfaceDeclaration(jnodeChild)) {
			tc::cont_emplace_back(vecjsymTopLevel, jtsTypeChecker->getSymbolAtLocation(*tc::js::ts_ext::name(*jotsInterfaceDeclaration)));
		} else /* TODO: TypeAliasDeclaration */ if (auto const jotsEnumDeclaration = tc::js::ts_ext::isEnumDeclaration(jnodeChild)) {
			tc::cont_emplace_back(vecjsymTopLevel, jtsTypeChecker->getSymbolAtLocation(*tc::js::ts_ext::name(*jotsEnumDeclaration)));
		} else if (auto const jotsModuleDeclaration = tc::js::ts_ext::isModuleDeclaration(jnodeChild)) {
			tc::cont_emplace_back(vecjsymTopLevel, jtsTypeChecker->getSymbolAtLocation((*jotsModuleDeclaration)->name()));
		} else if (jnodeChild->kind() == ts::SyntaxKind::EndOfFileToken) {
			// Do nothing
		} else {
			tc::append(std::cerr, "Unknown source file-level child kind: ", tc::as_dec(static_cast<int>(jnodeChild->kind())), "\n");
		}
		return tc::jst::js_undefined();
	}));
	return vecjsymTopLevel;
}
