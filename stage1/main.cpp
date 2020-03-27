#include "precompiled.h"
#include "typescript.d.bootstrap.h"
#include "mangle.h"
#include "walk_type.h"

using tc::js::js_string;
using tc::js::js_unknown;
using tc::js::globals::console;
using tc::js::globals::ts;
using tc::js::globals::Array;
using tc::js::globals::ReadonlyArray;

int main(int argc, char* argv[]) {
	_ASSERT(2 <= argc);

	ts::CompilerOptions const jtsCompilerOptions;
	jtsCompilerOptions->strict(true);
	jtsCompilerOptions->target(ts::ScriptTarget::ES5);
	jtsCompilerOptions->module(ts::ModuleKind::CommonJS);

	auto const rngstrFileNames = tc::make_iterator_range(argv + 1, argv + argc);
	ts::Program const jtsProgram = ts()->createProgram(ReadonlyArray<js_string>(rngstrFileNames), jtsCompilerOptions);

	ts::TypeChecker const jtsTypeChecker = jtsProgram->getTypeChecker();

	{
		auto const jtsReadOnlyArrayDiagnostics = ts()->getPreEmitDiagnostics(jtsProgram);
		if (jtsReadOnlyArrayDiagnostics->length()) {
			console()->log(ts()->formatDiagnosticsWithColorAndContext(jtsReadOnlyArrayDiagnostics, ts()->createCompilerHost(jtsCompilerOptions)));
			return 1;
		}
	}

	std::vector<ts::Symbol> vecjsymExportedModule;
	tc::for_each(jtsProgram->getSourceFiles(),
		[&](ts::SourceFile const& jtsSourceFile) noexcept {
			if (!tc::find_unique<tc::return_bool>(rngstrFileNames, tc::explicit_cast<std::string>(jtsSourceFile->fileName()))) {
				return;
			}
			auto const josymSourceFile = jtsTypeChecker->getSymbolAtLocation(jtsSourceFile);
			if (!josymSourceFile) {
				tc::append(std::cout, "Module not found for ", tc::explicit_cast<std::string>(jtsSourceFile->fileName()), "\n");
				return;
			}
			tc::cont_emplace_back(vecjsymExportedModule, *josymSourceFile);
		}
	);

	tc::for_each(
		vecjsymExportedModule,
		[&](ts::Symbol const& jsymSourceFile) noexcept {
			tc::append(std::cout, "Module name is ", tc::explicit_cast<std::string>(jsymSourceFile->getName()), "\n");
			WalkType(jtsTypeChecker, 0, jsymSourceFile);
		}
	);

	tc::append(std::cout, "\n========== GENERATED CODE ==========\n");

	{
		tc::append(std::cout,
			tc::join(tc::transform(g_vecjsymEnum, [&jtsTypeChecker](ts::Symbol const jsymEnum) noexcept {
				return tc::concat(
					"enum class ", MangleSymbolName(jtsTypeChecker, jsymEnum), " {\n",
					tc::join(
						tc::transform(*jsymEnum->exports(), [&jtsTypeChecker](ts::Symbol const jsymOption) noexcept {
							_ASSERTEQUAL(jsymOption->getFlags(), ts::SymbolFlags::EnumMember);
							auto const jarrDeclaration = jsymOption->declarations();
							_ASSERTEQUAL(jarrDeclaration->length(), 1);
							auto const jtsEnumMember = *ts()->isEnumMember(jarrDeclaration[0]);
							_ASSERTEQUAL(ts()->getCombinedModifierFlags(jtsEnumMember), ts::ModifierFlags::None);
							auto const junionOptionValue = jtsTypeChecker->getConstantValue(jtsEnumMember);
							if (!junionOptionValue.getEmval().isNumber()) {
								// Uncomputed value.
								return tc::explicit_cast<std::string>(tc::concat(
									"	/*", tc::explicit_cast<std::string>(jsymOption->getName()), " = ??? */\n"
								));
							} else {
								return tc::explicit_cast<std::string>(tc::concat(
									"	", tc::explicit_cast<std::string>(jsymOption->getName()), " = ",
									tc::as_dec(tc::explicit_cast<int>(double(junionOptionValue))),
									",\n"
								));
							}
						})
					),
					"};\n"
				);
			})),
			tc::join(tc::transform(g_vecjsymClass, [&jtsTypeChecker](ts::Symbol const jsymClass) noexcept {
				return tc::concat("struct ", MangleSymbolName(jtsTypeChecker, jsymClass), ";\n");
			})),
			tc::join(tc::transform(g_vecjsymClass, [&jtsTypeChecker](ts::Symbol const jsymClass) noexcept {
				auto const jarrsymExport = [&]() noexcept {
					if (jsymClass->exports()) {
						return jtsTypeChecker->getExportsOfModule(jsymClass);
					} else {
						return Array<ts::Symbol>(tc::make_empty_range<ts::Symbol>());
					}
				}();
				auto const vecjsymMember = [&]() noexcept {
					if (jsymClass->members()) {
						return tc::explicit_cast<std::vector<ts::Symbol>>(*jsymClass->members());
					} else {
						return std::vector<ts::Symbol>();
					}
				}();

				std::vector<ts::Symbol> vecjsymBaseClass;
				if (auto jointerfacetypeClass = jtsTypeChecker->getDeclaredTypeOfSymbol(jsymClass)->isClassOrInterface()) {
					tc::for_each(jtsTypeChecker->getBaseTypes(*jointerfacetypeClass),
						[&](ts::BaseType const jtsBaseType) noexcept {
							if (auto const jointerfacetypeBase = tc::reluctant_implicit_cast<ts::Type>(jtsBaseType)->isClassOrInterface()) {
								tc::cont_emplace_back(vecjsymBaseClass, *(*jointerfacetypeBase)->getSymbol());
							}
						}
					);
				} else {
					// Do nothing: e.g. namespaces.
				}

				return tc::explicit_cast<std::string>(tc::concat(
					"struct ", MangleSymbolName(jtsTypeChecker, jsymClass),
					" : ",
					tc_conditional_range(
						tc::empty(vecjsymBaseClass),
						tc::explicit_cast<std::string>("virtual IObject"),
						tc::explicit_cast<std::string>(tc::join_separated(
							tc::transform(vecjsymBaseClass,
								[&jtsTypeChecker](ts::Symbol const jsymBaseClass) noexcept {
									return tc::concat("virtual ", MangleSymbolName(jtsTypeChecker, jsymBaseClass));
								}
							),
							", "
						))
					),
					" {\n",
					tc::join(tc::transform(
						tc::filter(jarrsymExport, [](ts::Symbol const jsymExport) noexcept {
							return IsEnumInCpp(jsymExport);
						}),
						[&jtsTypeChecker](ts::Symbol const jsymExport) noexcept {
							return tc::concat(
								"	using ",
								tc::explicit_cast<std::string>(jsymExport->getName()),
								" = ",
								MangleSymbolName(jtsTypeChecker, jsymExport),
								";\n"
							);
						}
					)),
					tc::join(tc::transform(
						tc::filter(jarrsymExport, [](ts::Symbol const jsymExport) noexcept {
							return IsClassInCpp(jsymExport);
						}),
						[&jtsTypeChecker](ts::Symbol const jsymExport) noexcept {
							return tc::concat(
								"	using ",
								tc::explicit_cast<std::string>(jsymExport->getName()),
								" = js_ref<",
								MangleSymbolName(jtsTypeChecker, jsymExport),
								">;\n"
							);
						}
					)),
					tc::join(tc::transform(
						tc::filter(vecjsymMember, [](ts::Symbol const jsymMember) noexcept {
							return ts::SymbolFlags::Property == jsymMember->getFlags();
						}),
						[&jtsTypeChecker](ts::Symbol const jsymProperty) noexcept {
							_ASSERTEQUAL(jsymProperty->declarations()->length(), 1);
							ts::Declaration const jdeclProperty = jsymProperty->declarations()[0];
							ts::ModifierFlags const nModifierFlags = ts()->getCombinedModifierFlags(jdeclProperty);
							_ASSERT(nModifierFlags == ts::ModifierFlags::None || nModifierFlags == ts::ModifierFlags::Readonly);
							return tc::concat(
								"	auto ",
								tc::explicit_cast<std::string>(jsymProperty->getName()),
								"() noexcept { return _getProperty<",
								MangleType(jtsTypeChecker, jtsTypeChecker->getTypeOfSymbolAtLocation(jsymProperty, jdeclProperty)),
								">(\"",
								tc::explicit_cast<std::string>(jsymProperty->getName()),
								"\"); }\n",
								(ts()->getCombinedModifierFlags(jdeclProperty) & ts::ModifierFlags::Readonly) ?
									"" :
									tc::explicit_cast<std::string>(tc::concat(
										"	void ",
										tc::explicit_cast<std::string>(jsymProperty->getName()),
										"(",
										MangleType(jtsTypeChecker, jtsTypeChecker->getTypeOfSymbolAtLocation(jsymProperty, jdeclProperty)),
										" v) noexcept { _setProperty(\"",
										tc::explicit_cast<std::string>(jsymProperty->getName()),
										"\", v); }\n"
									))
							);
						}
					)),
					tc::join(tc::transform(
						tc::filter(vecjsymMember, [](ts::Symbol const jsymMember) noexcept {
							return ts::SymbolFlags::Method == jsymMember->getFlags();
						}),
						[&jtsTypeChecker](ts::Symbol const jsymMethod) noexcept {
							return tc::join(tc::transform(
								jsymMethod->declarations(),
								[&jtsTypeChecker, jsymMethod](ts::Declaration const jdeclMethod) noexcept {
									_ASSERTEQUAL(ts()->getCombinedModifierFlags(jdeclMethod), ts::ModifierFlags::None);
									auto jtsSignatureDeclaration = [&]() -> ts::SignatureDeclaration noexcept {
										if (auto const jotsMethodSignature = ts()->isMethodSignature(jdeclMethod)) {
											_ASSERT(!ts()->isMethodDeclaration(jdeclMethod));
											return *jotsMethodSignature;
										}
										if (auto const jotsMethodDeclaration = ts()->isMethodDeclaration(jdeclMethod)) {
											return *jotsMethodDeclaration;
										}
										_ASSERTFALSE;
									}();

									auto const jtsSignature = *jtsTypeChecker->getSignatureFromDeclaration(jtsSignatureDeclaration);
									if (auto const jrarrunkTypeParameter = jtsSignature->getTypeParameters()) {
										if (!tc::empty(*jrarrunkTypeParameter)) {
											return tc::explicit_cast<std::string>(tc::concat(
												"	/* ",
												tc::explicit_cast<std::string>(jtsTypeChecker->signatureToString(jtsSignature)),
												" */\n"
											));
										}
									}
									return tc::explicit_cast<std::string>(tc::concat(
										"	auto ",
										tc::explicit_cast<std::string>(jsymMethod->getName()),
										"(",
										tc::join_separated(
											tc::transform(
												jtsSignature->getParameters(),
												[&jtsTypeChecker, jdeclMethod](ts::Symbol const jsymParameter) noexcept {
													return tc::concat(
														MangleType(jtsTypeChecker, jtsTypeChecker->getTypeOfSymbolAtLocation(jsymParameter, jdeclMethod)),
														" ",
														tc::explicit_cast<std::string>(jsymParameter->getName())
													);
												}
											),
											", "
										),
										") noexcept {\n",
										"		return _call<", MangleType(jtsTypeChecker, jtsSignature->getReturnType()), ">",
											"(\"", tc::explicit_cast<std::string>(jsymMethod->getName()), "\"",
												tc::join(tc::transform(
													jtsSignature->getParameters(),
													[](ts::Symbol const jsymParameter) noexcept {
														return tc::concat(", ", tc::explicit_cast<std::string>(jsymParameter->getName()));
													}
												)),
											");\n",
										"	}\n"
									));
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
