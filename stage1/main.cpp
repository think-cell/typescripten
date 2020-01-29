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

std::vector<ts::Symbol> g_vecjsymEnum, g_vecjsymClass;

bool IsEnumInCpp(ts::Symbol jsymType) {
    // TODO: more assertions: "I've seen these flags, I think they are unimportant, explicitly ignoring".
	return
		static_cast<int>(ts::SymbolFlags::RegularEnum) == jsymType->getFlags() ||
		static_cast<int>(ts::SymbolFlags::ConstEnum) == jsymType->getFlags();
}

bool IsClassInCpp(ts::Symbol jsymType) {
    // TODO: more assertions: "I've seen these flags, I think they are unimportant, explicitly ignoring".
	return jsymType->getFlags() & (
		static_cast<int>(ts::SymbolFlags::Class) |
		static_cast<int>(ts::SymbolFlags::Interface) |
		static_cast<int>(ts::SymbolFlags::ValueModule) |
		static_cast<int>(ts::SymbolFlags::NamespaceModule)
		);
}

void WalkType(ts::TypeChecker& jtsTypeChecker, int nOffset, ts::Symbol jsymType) {
	tc::append(std::cout,
		tc::repeat_n(' ', nOffset),
		"'", std::string(jtsTypeChecker->getFullyQualifiedName(jsymType)), "', ",
		"flags=", tc::as_dec(jsymType->getFlags()),
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
		tc::for_each(*jsymType->members(), [&](ts::Symbol jsymChild) { WalkType(jtsTypeChecker, nOffset + 4, jsymChild); });
	}

	tc::append(std::cout, tc::repeat_n(' ', nOffset + 2), "exportsOfModule\n");
	if (jsymType->exports()) {
		tc::for_each(jtsTypeChecker->getExportsOfModule(jsymType),
			[&](ts::Symbol jsymChild) { WalkType(jtsTypeChecker, nOffset + 4, jsymChild); }
		);
	}

	tc::append(std::cout, tc::repeat_n(' ', nOffset + 2), "call signatures\n");
	tc::for_each(jtsTypeChecker->getSignaturesOfType(jtsTypeChecker->getDeclaredTypeOfSymbol(jsymType), ts::SignatureKind::Call),
		[&](ts::Signature jtsSignature) {
			tc::append(std::cout,
				tc::repeat_n(' ', nOffset + 4),
				std::string(jtsTypeChecker->signatureToString(jtsSignature)),
				"\n"
			);
		}
	);

	tc::append(std::cout, tc::repeat_n(' ', nOffset + 2), "constructors\n");
	tc::for_each(jtsTypeChecker->getSignaturesOfType(jtsTypeChecker->getDeclaredTypeOfSymbol(jsymType), ts::SignatureKind::Construct),
		[&](ts::Signature jtsSignature) {
			tc::append(std::cout,
				tc::repeat_n(' ', nOffset + 4),
				std::string(jtsTypeChecker->signatureToString(jtsSignature)),
				"\n"
			);
		}
	);

	if (auto jotsInterfaceType = jtsTypeChecker->getDeclaredTypeOfSymbol(jsymType)->isClassOrInterface()) {
		tc::append(std::cout, tc::repeat_n(' ', nOffset + 2), "base types\n");
		tc::for_each(jtsTypeChecker->getBaseTypes(*jotsInterfaceType),
			[&](ts::BaseType jtsBaseType) {
				tc::append(std::cout,
					tc::repeat_n(' ', nOffset + 4),
					std::string(jtsTypeChecker->typeToString(jtsBaseType)),
					"\n"
				);
			}
		);
	}
}

std::string MangleSymbolName(ts::TypeChecker jtsTypeChecker, ts::Symbol jsymType) {
	std::string strMangled = "_js_j";
	tc::for_each(std::string(jtsTypeChecker->getFullyQualifiedName(jsymType)), [&](char c) {
		switch (c) {
		case '_': tc::append(strMangled, "_u"); break;
		case ',': tc::append(strMangled, "_c"); break;
		case '.': tc::append(strMangled, "_d"); break;
		case '-': tc::append(strMangled, "_m"); break;
		case '"': tc::append(strMangled, "_q"); break;
		default: tc::cont_emplace_back(strMangled, c); break;
		}
	});
	return strMangled;
}

std::string MangleType(ts::TypeChecker jtsTypeChecker, ts::Type jtypeRoot) {
    // TODO: more assertions: "I've seen these flags, I think they are unimportant, explicitly ignoring".
	// See checker.ts:typeToTypeNodeHelper
	if (jtypeRoot->flags() & static_cast<int>(ts::TypeFlags::Any) ||
		jtypeRoot->flags() & static_cast<int>(ts::TypeFlags::Unknown)
		) {
		return "js_unknown";
	}
	if (jtypeRoot->flags() & static_cast<int>(ts::TypeFlags::String)) {
		return "js_string";
	}
	if (jtypeRoot->flags() & static_cast<int>(ts::TypeFlags::Number)) {
		return "double";
	}
	if (jtypeRoot->flags() & static_cast<int>(ts::TypeFlags::Boolean)) {
		return "bool";
	}
	if (jtypeRoot->flags() & static_cast<int>(ts::TypeFlags::Void)) {
		return "void";
	}
	if (jtypeRoot->flags() & static_cast<int>(ts::TypeFlags::Undefined)) {
		return "js_undefined";
	}
	if (jtypeRoot->flags() & static_cast<int>(ts::TypeFlags::Null)) {
		return "js_null";
	}
	if (auto jotsUnionType = jtypeRoot->isUnion()) {
		_ASSERT(1 < (*jotsUnionType)->types()->length());
		return tc::explicit_cast<std::string>(tc::concat(
			"js_union<",
			tc::join_separated(
				tc::transform((*jotsUnionType)->types(), [&](ts::Type jtypeUnionOption) {
					return MangleType(jtsTypeChecker, jtypeUnionOption);
				}),
				", "
			),
			">"
		));
	}
	if (auto jotsInterfaceType = jtypeRoot->isClassOrInterface()) {
		_ASSERTEQUAL(static_cast<int>(ts::TypeFlags::Object), (*jotsInterfaceType)->flags());
		_ASSERT(!(*jotsInterfaceType)->typeParameters());
		_ASSERT(!(*jotsInterfaceType)->outerTypeParameters());
		_ASSERT(!(*jotsInterfaceType)->localTypeParameters());
		_ASSERT(!(*jotsInterfaceType)->thisType());
		auto josymInterface = (*jotsInterfaceType)->getSymbol();
		_ASSERT(josymInterface);
		return tc::explicit_cast<std::string>(tc::concat(
			"js_ref<", MangleSymbolName(jtsTypeChecker, *josymInterface), ">"
		));
	}
	return tc::explicit_cast<std::string>(tc::concat(
		"js_unknown /*flags=",
		tc::as_dec(jtypeRoot->flags()),
		": ",
		std::string(jtsTypeChecker->typeToString(jtypeRoot)),
		"*/")
	);
};

int main(int argc, char* argv[]) {
	_ASSERT(2 <= argc);

	ts::CompilerOptions jtsCompilerOptions;
	jtsCompilerOptions->strict(true);
	jtsCompilerOptions->target(ts::ScriptTarget::ES5);
	jtsCompilerOptions->module(ts::ModuleKind::CommonJS);

	auto rngstrFileNames = tc::make_iterator_range(argv + 1, argv + argc);
	ts::Program jtsProgram = ts()->createProgram(ReadonlyArray<js_string>(rngstrFileNames), jtsCompilerOptions);

	ts::TypeChecker jtsTypeChecker = jtsProgram->getTypeChecker();

	{
		auto jtsReadOnlyArrayDiagnostics = ts()->getPreEmitDiagnostics(jtsProgram);
		if (jtsReadOnlyArrayDiagnostics->length()) {
			console()->log(ts()->formatDiagnosticsWithColorAndContext(jtsReadOnlyArrayDiagnostics, ts()->createCompilerHost(jtsCompilerOptions)));
			return 1;
		}
	}

	std::vector<ts::Symbol> vecjsymExportedModule;
	tc::for_each(jtsProgram->getSourceFiles(),
		[&](ts::SourceFile const& jtsSourceFile) {
			if (!tc::find_unique<tc::return_bool>(rngstrFileNames, std::string(jtsSourceFile->fileName()))) {
				return;
			}
			auto josymSourceFile = jtsTypeChecker->getSymbolAtLocation(jtsSourceFile);
			if (!josymSourceFile) {
				tc::append(std::cout, "Module not found for ", std::string(jtsSourceFile->fileName()), "\n");
				return;
			}
			tc::cont_emplace_back(vecjsymExportedModule, *josymSourceFile);
		}
	);

	tc::for_each(
		vecjsymExportedModule,
		[&](ts::Symbol const& jsymSourceFile) {
			tc::append(std::cout, "Module name is ", std::string(jsymSourceFile->getName()), "\n");
			WalkType(jtsTypeChecker, 0, jsymSourceFile);
		}
	);

	tc::append(std::cout, "\n========== GENERATED CODE ==========\n");

	{
		tc::append(std::cout,
			tc::join(tc::transform(g_vecjsymEnum, [&](ts::Symbol jsymEnum) {
				_ASSERT(jsymEnum->exports());
				return tc::concat(
					"enum class ", MangleSymbolName(jtsTypeChecker, jsymEnum), " {\n",
					tc::join(
						tc::transform(*jsymEnum->exports(), [&](ts::Symbol jsymOption) {
							_ASSERTEQUAL(jsymOption->getFlags(), static_cast<int>(ts::SymbolFlags::EnumMember));
							auto jarrDeclaration = jsymOption->declarations();
							_ASSERTEQUAL(jarrDeclaration->length(), 1);
							auto jotsEnumMember = ts()->isEnumMember(jarrDeclaration[0]);
							_ASSERT(jotsEnumMember);
							_ASSERTEQUAL(ts()->getCombinedModifierFlags(*jotsEnumMember), 0);
							auto junionOptionValue = jtsTypeChecker->getConstantValue(*jotsEnumMember);
							if (!junionOptionValue.getEmval().isNumber()) {
								// Uncomputed value.
								return tc::explicit_cast<std::string>(tc::concat(
									"	/*", std::string(jsymOption->getName()), " = ??? */\n"
								));
							} else {
								return tc::explicit_cast<std::string>(tc::concat(
									"	", std::string(jsymOption->getName()), " = ",
									tc::as_dec(tc::explicit_cast<int>(double(junionOptionValue))),
									",\n"
								));
							}
						})
					),
					"};\n"
				);
			})),
			tc::join(tc::transform(g_vecjsymClass, [&](ts::Symbol jsymClass) {
				return tc::concat("struct ", MangleSymbolName(jtsTypeChecker, jsymClass), ";\n");
			})),
			tc::join(tc::transform(g_vecjsymClass, [&](ts::Symbol jsymClass) {
				Array<ts::Symbol> jarrsymExport(std::initializer_list<ts::Symbol>{});
				if (jsymClass->exports()) {
					jarrsymExport = jtsTypeChecker->getExportsOfModule(jsymClass);
				}
				std::vector<ts::Symbol> vecjsymMember;
				if (jsymClass->members()) {
					vecjsymMember = tc::explicit_cast<std::vector<ts::Symbol>>(*jsymClass->members());
				}

				std::vector<ts::Symbol> vecjsymBaseClass;
				if (auto joptInterfaceType = jtsTypeChecker->getDeclaredTypeOfSymbol(jsymClass)->isClassOrInterface()) {
					tc::for_each(jtsTypeChecker->getBaseTypes(*joptInterfaceType),
						[&](ts::BaseType jtsBaseType) {
							if (auto jotsInterfaceType = tc::reluctant_implicit_cast<ts::Type>(jtsBaseType)->isClassOrInterface()) {
								auto josymInterface = (*jotsInterfaceType)->getSymbol();
								_ASSERT(josymInterface);
								tc::cont_emplace_back(vecjsymBaseClass, *josymInterface);
							}
						}
					);
				} else {
					// Do nothing: e.g. namespaces.
				}

				// TODO: force eager evaluation to keep arrays in scope.
				return tc::explicit_cast<std::string>(tc::concat(
					"struct ", MangleSymbolName(jtsTypeChecker, jsymClass),
					(vecjsymBaseClass.empty() ? "" :
						tc::explicit_cast<std::string>(tc::concat(
							" : ",
							tc::join_separated(
								tc::transform(vecjsymBaseClass,
									[&](ts::Symbol jsymBaseClass) {
										return tc::concat("virtual ", MangleSymbolName(jtsTypeChecker, jsymBaseClass));
									}
								),
								", "
							)
						))
					),
					" {\n",
					tc::join(tc::transform(
						tc::filter(jarrsymExport, [&](ts::Symbol jsymExport) {
							return IsEnumInCpp(jsymExport) || IsClassInCpp(jsymExport);
						}),
						[&](ts::Symbol jsymExport) {
							return tc::concat(
								"	using ",
								std::string(jsymExport->getName()),
								" = js_ref<",
								MangleSymbolName(jtsTypeChecker, jsymExport),
								">;\n"
							);
						}
					)),
					tc::join(tc::transform(
						tc::filter(vecjsymMember, [&](ts::Symbol jsymMember) {
							return static_cast<int>(ts::SymbolFlags::Property) == jsymMember->getFlags();
						}),
						[&](ts::Symbol jsymProperty) {
							_ASSERTEQUAL(jsymProperty->declarations()->length(), 1);
							ts::Declaration jdeclProperty = jsymProperty->declarations()[0];
							int nModifierFlags = ts()->getCombinedModifierFlags(jdeclProperty);
							_ASSERT(nModifierFlags == 0 || nModifierFlags == static_cast<int>(ts::ModifierFlags::Readonly));
							return tc::concat(
								"	auto ",
								std::string(jsymProperty->getName()),
								"() noexcept { return _getProperty<",
								MangleType(jtsTypeChecker, jtsTypeChecker->getTypeOfSymbolAtLocation(jsymProperty, jdeclProperty)),
								">(\"",
								std::string(jsymProperty->getName()),
								"\"); }\n",
								(ts()->getCombinedModifierFlags(jdeclProperty) & static_cast<int>(ts::ModifierFlags::Readonly)) ?
									"" :
									tc::explicit_cast<std::string>(tc::concat(
										"	void ",
										std::string(jsymProperty->getName()),
										"(",
										MangleType(jtsTypeChecker, jtsTypeChecker->getTypeOfSymbolAtLocation(jsymProperty, jdeclProperty)),
										" v) noexcept { _setProperty(\"",
										std::string(jsymProperty->getName()),
										"\", v); }\n"
									))
							);
						}
					)),
					tc::join(tc::transform(
						tc::filter(vecjsymMember, [&](ts::Symbol jsymMember) {
							return static_cast<int>(ts::SymbolFlags::Method) == jsymMember->getFlags();
						}),
						[&](ts::Symbol jsymMethod) {
							return tc::join(tc::transform(
								jsymMethod->declarations(),
								[&](ts::Declaration jdeclMethod) {
									_ASSERTEQUAL(ts()->getCombinedModifierFlags(jdeclMethod), 0);
									tc::js::js_optional<ts::SignatureDeclaration> jotsSignatureDeclaration;
									if (auto jotsMethodSignature = ts()->isMethodSignature(jdeclMethod)) {
										jotsSignatureDeclaration = *jotsMethodSignature;
									}
									if (auto jotsMethodDeclaration = ts()->isMethodDeclaration(jdeclMethod)) {
										jotsSignatureDeclaration = *jotsMethodDeclaration;
									}
									_ASSERT(jotsSignatureDeclaration);

									auto const jotsSignature = jtsTypeChecker->getSignatureFromDeclaration(*jotsSignatureDeclaration);
									_ASSERT(jotsSignature);

									auto const jtsSignature = *jotsSignature;
									if (auto jrarrunkTypeParameter = jtsSignature->getTypeParameters()) {
										_ASSERT(tc::empty(*jrarrunkTypeParameter));
									}
									return tc::concat(
										"	auto ",
										std::string(jsymMethod->getName()),
										"(",
										tc::join_separated(
											tc::transform(
												jtsSignature->getParameters(),
												[&](ts::Symbol const jsymParameter) {
													return tc::concat(
														MangleType(jtsTypeChecker, jtsTypeChecker->getTypeOfSymbolAtLocation(jsymParameter, jdeclMethod)),
														" ",
														std::string(jsymParameter->getName())
													);
												}
											),
											", "
										),
										") noexcept {\n",
										"		return _call<", MangleType(jtsTypeChecker, jtsSignature->getReturnType()), ">",
										"(\"", std::string(jsymMethod->getName()), "\"",
										tc::join(tc::transform(
											jtsSignature->getParameters(),
											[&](ts::Symbol jsymParameter) {
												return tc::concat(", ", std::string(jsymParameter->getName()));
											}
										)),
										");\n",
										"	}\n"
									);
								}
							));
						}
					)),
					"};\n"
				));
			}))
		);
	}
	return 0;
}
