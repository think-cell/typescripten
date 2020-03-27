#include "precompiled.h"
#include "typescript.d.bootstrap.h"

using tc::js::globals::ts;

std::vector<ts::Symbol> g_vecjsymEnum, g_vecjsymClass;

bool IsEnumInCpp(ts::Symbol const jsymType) noexcept {
	// TODO: more assertions: "I've seen these flags, I think they are unimportant, explicitly ignoring".
	return
		ts::SymbolFlags::RegularEnum == jsymType->getFlags() ||
		ts::SymbolFlags::ConstEnum == jsymType->getFlags();
}

bool IsClassInCpp(ts::Symbol const jsymType) noexcept {
	// TODO: more assertions: "I've seen these flags, I think they are unimportant, explicitly ignoring".
	return jsymType->getFlags() & (
		ts::SymbolFlags::Class |
		ts::SymbolFlags::Interface |
		ts::SymbolFlags::ValueModule |
		ts::SymbolFlags::NamespaceModule
		);
}

void WalkType(ts::TypeChecker const& jtsTypeChecker, int const nOffset, ts::Symbol const jsymType) noexcept {
	tc::append(std::cout,
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

	tc::append(std::cout, tc::repeat_n(' ', nOffset + 2), "members\n");
	if (jsymType->members()) {
		tc::for_each(*jsymType->members(), [&](ts::Symbol const jsymChild) noexcept { WalkType(jtsTypeChecker, nOffset + 4, jsymChild); });
	}

	tc::append(std::cout, tc::repeat_n(' ', nOffset + 2), "exportsOfModule\n");
	if (jsymType->exports()) {
		tc::for_each(jtsTypeChecker->getExportsOfModule(jsymType),
			[&](ts::Symbol const jsymChild) noexcept { WalkType(jtsTypeChecker, nOffset + 4, jsymChild); }
		);
	}

	tc::append(std::cout, tc::repeat_n(' ', nOffset + 2), "call signatures\n");
	tc::for_each(jtsTypeChecker->getSignaturesOfType(jtsTypeChecker->getDeclaredTypeOfSymbol(jsymType), ts::SignatureKind::Call),
		[&](ts::Signature const jtsSignature) noexcept {
			tc::append(std::cout,
				tc::repeat_n(' ', nOffset + 4),
				tc::explicit_cast<std::string>(jtsTypeChecker->signatureToString(jtsSignature)),
				"\n"
			);
		}
	);

	tc::append(std::cout, tc::repeat_n(' ', nOffset + 2), "constructors\n");
	tc::for_each(jtsTypeChecker->getSignaturesOfType(jtsTypeChecker->getDeclaredTypeOfSymbol(jsymType), ts::SignatureKind::Construct),
		[&](ts::Signature const jtsSignature) noexcept {
			tc::append(std::cout,
				tc::repeat_n(' ', nOffset + 4),
				tc::explicit_cast<std::string>(jtsTypeChecker->signatureToString(jtsSignature)),
				"\n"
			);
		}
	);

	if (auto jointerfacetype = jtsTypeChecker->getDeclaredTypeOfSymbol(jsymType)->isClassOrInterface()) {
		tc::append(std::cout, tc::repeat_n(' ', nOffset + 2), "base types\n");
		tc::for_each(jtsTypeChecker->getBaseTypes(*jointerfacetype),
			[&](ts::BaseType const jtsBaseType) noexcept {
				tc::append(std::cout,
					tc::repeat_n(' ', nOffset + 4),
					tc::explicit_cast<std::string>(jtsTypeChecker->typeToString(jtsBaseType)),
					"\n"
				);
			}
		);
	}
}
