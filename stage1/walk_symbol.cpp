#include "precompiled.h"
#include "typescript.d.bootstrap.h"
#include "walk_symbol.h"

using tc::js::ts;

std::vector<ts::Symbol> g_vecjsymEnum, g_vecjsymClass, g_vecjsymTypeAlias;

ECppType CppType(ts::TypeChecker jtsTypeChecker, ts::Symbol jsymType) noexcept {
	// TODO: Symbol::getFlags returns a bitmask and we must treat it so
	// Callers of CppType must treat return value as a bit mask too
	if(ts::SymbolFlags::RegularEnum == jsymType->getFlags() || ts::SymbolFlags::ConstEnum == jsymType->getFlags()) {
		return ecpptypeENUM;
	}
	if(ts::SymbolFlags::Class == jsymType->getFlags() ||
	ts::SymbolFlags::Interface == jsymType->getFlags() ||
	(ts::SymbolFlags::Interface | ts::SymbolFlags::FunctionScopedVariable) == jsymType->getFlags() ||
	ts::SymbolFlags::ValueModule == jsymType->getFlags() ||
	(ts::SymbolFlags::ValueModule | ts::SymbolFlags::Interface) == jsymType->getFlags() ||
	(ts::SymbolFlags::NamespaceModule | ts::SymbolFlags::Interface) == jsymType->getFlags() ||
	ts::SymbolFlags::NamespaceModule == jsymType->getFlags()) {
		return ecpptypeCLASS;
	}

	if(ts::SymbolFlags::TypeAlias==jsymType->getFlags()) {
		// Emit type aliases as using declarations if the type alias only references types, not e.g. literals
		// A type alias of a single or a union of literals should be transformed into tag structs in C++
		auto IsObject = [](auto jtypeInternal) noexcept {
			switch(jtypeInternal->flags()) {
			case ts::TypeFlags::Any:
			case ts::TypeFlags::Unknown:
			case ts::TypeFlags::String:
			case ts::TypeFlags::Number:
			case ts::TypeFlags::Boolean:
			case ts::TypeFlags::Enum:
			case ts::TypeFlags::BigInt:
			case ts::TypeFlags::Undefined:
			case ts::TypeFlags::Object:
				return true;
			default:
				return false;
			}
		};

		auto const jtype = jtsTypeChecker->getTypeFromTypeNode(ts::TypeAliasDeclaration(jsymType->declarations()[0])->type());
		auto const ojuniontype = tc::js::ts_ext::isUnion(jtype);
		if((ojuniontype && tc::all_of((*ojuniontype)->types(), IsObject))
		|| IsObject(jtype)) {
			return ecpptypeTYPEALIAS;
		}
	}
	return ecpptypeIGNORE;
}

void WalkSymbol(ts::TypeChecker jtsTypeChecker, int const nOffset, ts::Symbol const jsymType) noexcept {
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
	auto const ecpptype = CppType(jtsTypeChecker, jsymType);
	if(ecpptypeENUM&ecpptype) {
		tc::cont_emplace_back(g_vecjsymEnum, jsymType);
	}
	if(ecpptypeCLASS&ecpptype) {
		tc::cont_emplace_back(g_vecjsymClass, jsymType);
	}
	if(ecpptypeTYPEALIAS&ecpptype) {
		tc::cont_emplace_back(g_vecjsymTypeAlias, jsymType); 
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
			[&](tc::js::ts::BaseType const jtsBaseType) noexcept {
				tc::append(std::cerr,
					tc::repeat_n(' ', nOffset + 4),
					tc::explicit_cast<std::string>(jtsTypeChecker->typeToString(jtsBaseType)),
					"\n"
				);
			}
		);
	}
}

