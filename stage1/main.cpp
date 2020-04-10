#include "precompiled.h"
#include "typescript.d.bootstrap.h"
#include "mangle.h"
#include "walk_symbol.h"

using tc::js::js_string;
using tc::js::js_optional;
using tc::js::js_unknown;
using tc::js::globals::console;
using tc::js::globals::ts;
using tc::js::globals::Array;
using tc::js::globals::ReadonlyArray;

std::string RetrieveSymbolFromCpp(ts::Symbol jsymSymbol) noexcept {
	std::string strSymbolName = tc::explicit_cast<std::string>(jsymSymbol->getName());
	if ('"' == strSymbolName.front() && '"' == strSymbolName.back()) {
		strSymbolName = strSymbolName.substr(1, strSymbolName.length() - 2);
	}
	if (!jsymSymbol->parent()) {
		return tc::explicit_cast<std::string>(tc::concat("emscripten::val::global(\"", strSymbolName, "\")"));
	} else {
		return tc::explicit_cast<std::string>(tc::concat(
			RetrieveSymbolFromCpp(*jsymSymbol->parent()),
			"[\"", strSymbolName, "\"]"
		));
	}
}

std::string CppifyName(ts::Symbol jsymSymbol) noexcept {
	std::string strSourceName = tc::explicit_cast<std::string>(jsymSymbol->getName());
	if ('"' == strSourceName.front() && '"' == strSourceName.back()) {
		strSourceName = strSourceName.substr(1, strSourceName.length() - 2);
	}
	std::string strResult = tc::explicit_cast<std::string>(tc::transform(
		strSourceName,
		[&strSourceName](char c) noexcept {
			if ('-' == c) return '_';
			if ('_' == c) return c;
			if ('a' <= c && c <= 'z') return c;
			if ('A' <= c && c <= 'Z') return c;
			if ('0' <= c && c <= '9') return c;
			tc::append(std::cerr, "Cannot convert JS name to C++ name: '", strSourceName, "'\n");
			_ASSERTFALSE;
		}
	));
	_ASSERT(!strResult.empty());
	return strResult;
}

struct SJsFunctionLike {
	ts::Symbol m_jsymFunctionLike;
	ts::Declaration m_jdeclFunctionLike;
	ts::SignatureDeclaration m_jtsSignatureDeclaration;
	ts::Signature m_jtsSignature;
	js_optional<ReadonlyArray<js_unknown>> m_joptarrunkTypeParameter;
	std::string m_strCppifiedParameters;

	SJsFunctionLike(ts::TypeChecker const jtsTypeChecker, ts::Symbol const jsymFunctionLike, ts::Declaration const jdeclFunctionLike) noexcept
		: m_jsymFunctionLike(jsymFunctionLike)
		, m_jdeclFunctionLike(jdeclFunctionLike)
		, m_jtsSignatureDeclaration([&]() noexcept -> ts::SignatureDeclaration {
			if (auto const jotsMethodSignature = ts()->isMethodSignature(jdeclFunctionLike)) { // In interfaces.
				return *jotsMethodSignature;
			}
			if (auto const jotsMethodDeclaration = ts()->isMethodDeclaration(jdeclFunctionLike)) { // In classes.
				return *jotsMethodDeclaration;
			}
			if (auto const jotsConstructorDeclaration = ts()->isConstructorDeclaration(jdeclFunctionLike)) {
				return *jotsConstructorDeclaration;
			}
			if (auto const jotsFunctionDeclaration = ts()->isFunctionDeclaration(jdeclFunctionLike)) {
				return *jotsFunctionDeclaration;
			}
			_ASSERTFALSE;
		}())
		, m_jtsSignature(*jtsTypeChecker->getSignatureFromDeclaration(m_jtsSignatureDeclaration))
		, m_joptarrunkTypeParameter(m_jtsSignature->getTypeParameters())
		, m_strCppifiedParameters(tc::explicit_cast<std::string>(tc::join_separated(
			tc::transform(
				m_jtsSignature->getParameters(),
				[&jtsTypeChecker, &jdeclFunctionLike](ts::Symbol const jsymParameter) noexcept {
					return tc::concat(
						MangleType(jtsTypeChecker, jtsTypeChecker->getTypeOfSymbolAtLocation(jsymParameter, jdeclFunctionLike)),
						" ",
						CppifyName(jsymParameter)
					);
				}
			),
			", "
		)))
	{
		if (ts::ModifierFlags::None != ts()->getCombinedModifierFlags(jdeclFunctionLike) &&
			ts::ModifierFlags::Export != ts()->getCombinedModifierFlags(jdeclFunctionLike) &&
			ts::ModifierFlags::Ambient != ts()->getCombinedModifierFlags(jdeclFunctionLike)) {
			tc::append(std::cerr,
				"Unknown getCombinedModifierFlags for jdeclFunctionLike ",
				tc::explicit_cast<std::string>(m_jsymFunctionLike->getName()),
				": ",
				tc::as_dec(static_cast<int>(ts()->getCombinedModifierFlags(jdeclFunctionLike))),
				"\n"
			);
			_ASSERTFALSE;
		}
	}
};

struct SJsVariableLike {
	ts::Symbol m_jsymName;
	std::string m_strJsName;
	std::string m_strCppifiedName;
	ts::Declaration m_jdeclVariableLike;
	ts::Type m_jtypeDeclared;
	std::string m_strMangledType;
	bool m_bReadonly;

	SJsVariableLike(ts::TypeChecker const jtsTypeChecker, ts::Symbol const jsymName) noexcept
		: m_jsymName(jsymName)
		, m_strJsName(tc::explicit_cast<std::string>(jsymName->getName()))
		, m_strCppifiedName(CppifyName(jsymName))
		, m_jdeclVariableLike([&]() noexcept {
			_ASSERTEQUAL(jsymName->declarations()->length(), 1);
			return jsymName->declarations()[0];
		}())
		, m_jtypeDeclared(jtsTypeChecker->getTypeOfSymbolAtLocation(jsymName, m_jdeclVariableLike))
		, m_strMangledType(MangleType(jtsTypeChecker, m_jtypeDeclared))
		, m_bReadonly(ts()->getCombinedModifierFlags(m_jdeclVariableLike) & ts::ModifierFlags::Readonly)
	{
		ts::ModifierFlags const nModifierFlags = ts()->getCombinedModifierFlags(m_jdeclVariableLike);
		if (ts::ModifierFlags::None != nModifierFlags &&
			ts::ModifierFlags::Export != nModifierFlags &&
			ts::ModifierFlags::Readonly != nModifierFlags &&
			ts::ModifierFlags::Ambient != ts()->getCombinedModifierFlags(m_jdeclVariableLike)) {
			tc::append(std::cerr,
				"Unknown getCombinedModifierFlags for jdeclVariableLike ",
				m_strJsName,
				": ",
				tc::as_dec(static_cast<int>(nModifierFlags)),
				"\n"
			);
			_ASSERTFALSE;
		}
	}
};

struct SJsClass {
	ts::Symbol m_jsymClass;
	std::vector<ts::Symbol> m_vecjsymExportOfModule;
	std::vector<ts::Symbol> m_vecjsymExportType;
	std::vector<SJsFunctionLike> m_vecjsfunctionlikeExportFunction;
	std::vector<SJsVariableLike> m_vecjsvariablelikeExportVariable;
	std::vector<ts::Symbol> m_vecjsymMember;
	std::vector<SJsFunctionLike> m_vecjsfunctionlikeMethod;
	std::vector<SJsVariableLike> m_vecjsvariablelikeProperty;
	std::vector<ts::Symbol> m_vecjsymBaseClass;

	SJsClass(ts::TypeChecker const jtsTypeChecker, ts::Symbol const jsymClass) noexcept
		: m_jsymClass(jsymClass)
		, m_vecjsymExportOfModule(
			jsymClass->getFlags() & ts::SymbolFlags::Module
			? tc::make_vector(jtsTypeChecker->getExportsOfModule(jsymClass))
			: tc::make_vector(tc::make_empty_range<ts::Symbol>())
		)
		, m_vecjsymExportType(tc::make_vector(tc::filter(
			m_vecjsymExportOfModule,
			[](ts::Symbol const jsymExport) noexcept {
				return IsEnumInCpp(jsymExport) || IsClassInCpp(jsymExport);
			}
		)))
		, m_vecjsfunctionlikeExportFunction(tc::make_vector(tc::join(tc::transform(
			tc::filter(
				m_vecjsymExportOfModule,
				[](ts::Symbol const jsymExport) noexcept {
					return ts::SymbolFlags::Function == jsymExport->getFlags();
				}
			),
			[&jtsTypeChecker](ts::Symbol const jsymFunction) noexcept {
				return tc::transform(
					jsymFunction->declarations(),
					[&jtsTypeChecker, jsymFunction](ts::Declaration const jdeclFunction) noexcept {
						return SJsFunctionLike(jtsTypeChecker, jsymFunction, jdeclFunction);
					}
				);
			}
		))))
		, m_vecjsvariablelikeExportVariable(tc::make_vector(tc::transform(
			tc::filter(
				m_vecjsymExportOfModule,
				[](ts::Symbol const jsymExport) noexcept {
					return
						ts::SymbolFlags::FunctionScopedVariable == jsymExport->getFlags() ||
						ts::SymbolFlags::BlockScopedVariable == jsymExport->getFlags();
				}
			),
			[&jtsTypeChecker](ts::Symbol const jsymVariable) noexcept {
				return SJsVariableLike(jtsTypeChecker, jsymVariable);
			}
		)))
		, m_vecjsymMember([&]() noexcept {
			if (jsymClass->members()) {
				return tc::explicit_cast<std::vector<ts::Symbol>>(*jsymClass->members());
			} else {
				return std::vector<ts::Symbol>();
			}
		}())
		, m_vecjsfunctionlikeMethod(tc::make_vector(tc::join(tc::transform(
			tc::filter(m_vecjsymMember, [](ts::Symbol const jsymMember) noexcept {
				return ts::SymbolFlags::Method == jsymMember->getFlags() || ts::SymbolFlags::Constructor == jsymMember->getFlags();
			}),
			[&jtsTypeChecker](ts::Symbol const jsymMethod) noexcept {
				return tc::transform(
					jsymMethod->declarations(),
					[&jtsTypeChecker, jsymMethod](ts::Declaration const jdeclMethod) noexcept {
						return SJsFunctionLike(jtsTypeChecker, jsymMethod, jdeclMethod);
					}
				);
			}
		))))
		, m_vecjsvariablelikeProperty(tc::make_vector(tc::transform(
			tc::filter(m_vecjsymMember, [](ts::Symbol const jsymMember) noexcept {
				return ts::SymbolFlags::Property == jsymMember->getFlags();
			}),
			[&](ts::Symbol const jsymProperty) noexcept {
				return SJsVariableLike(jtsTypeChecker, jsymProperty);
			}
		)))
		, m_vecjsymBaseClass()
	{
		if (auto jointerfacetypeClass = jtsTypeChecker->getDeclaredTypeOfSymbol(jsymClass)->isClassOrInterface()) {
			tc::for_each(jtsTypeChecker->getBaseTypes(*jointerfacetypeClass),
				[&](ts::BaseType const jtsBaseType) noexcept {
					if (auto const jointerfacetypeBase = tc::reluctant_implicit_cast<ts::Type>(jtsBaseType)->isClassOrInterface()) {
						tc::cont_emplace_back(m_vecjsymBaseClass, *(*jointerfacetypeBase)->getSymbol());
					}
				}
			);
		} else {
			// Do nothing: e.g. namespaces.
		}
	}
};

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
	std::vector<ts::Symbol> vecjsymExportedSymbol;
	tc::for_each(jtsProgram->getSourceFiles(),
		[&](ts::SourceFile const& jtsSourceFile) noexcept {
			if (!tc::find_unique<tc::return_bool>(rngstrFileNames, tc::explicit_cast<std::string>(jtsSourceFile->fileName()))) {
				return;
			}
			auto const josymSourceFile = jtsTypeChecker->getSymbolAtLocation(jtsSourceFile);
			if (!josymSourceFile) {
				tc::append(std::cerr, "Module not found for ", tc::explicit_cast<std::string>(jtsSourceFile->fileName()), ", treating at as a global library\n");
				tc::append(vecjsymExportedSymbol, ListSourceFileTopLevel(jtsTypeChecker, jtsSourceFile));
				return;
			}
			tc::cont_emplace_back(vecjsymExportedModule, *josymSourceFile);
		}
	);

	tc::for_each(
		vecjsymExportedModule,
		[&](ts::Symbol const& jsymSourceFile) noexcept {
			tc::append(std::cerr, "Module name is ", tc::explicit_cast<std::string>(jsymSourceFile->getName()), "\n");
			WalkSymbol(jtsTypeChecker, 0, jsymSourceFile);
		}
	);

	tc::for_each(
		vecjsymExportedSymbol,
		[&](ts::Symbol const& jsymSymbol) noexcept {
			tc::append(std::cerr, "Global exported symbol is ", tc::explicit_cast<std::string>(jsymSymbol->getName()), "\n");
			WalkSymbol(jtsTypeChecker, 0, jsymSymbol);
		}
	);

	tc::append(std::cerr, "\n========== GENERATED CODE ==========\n");

	{
		tc::for_each(g_vecjsymEnum, [&jtsTypeChecker](ts::Symbol const jsymEnum) noexcept {
			g_usstrAllowedMangledTypes.insert(MangleSymbolName(jtsTypeChecker, jsymEnum));
		});
		tc::for_each(g_vecjsymClass, [&jtsTypeChecker](ts::Symbol const jsymClass) noexcept {
			g_usstrAllowedMangledTypes.insert(MangleSymbolName(jtsTypeChecker, jsymClass));
		});

		auto vecjsclassClass = tc::make_vector(tc::transform(g_vecjsymClass, [&jtsTypeChecker](ts::Symbol const jsymClass) noexcept {
			return SJsClass(jtsTypeChecker, jsymClass);
		}));

		tc::append(std::cout,
			"namespace tc::js {\n",
			tc::join(tc::transform(g_vecjsymEnum, [&jtsTypeChecker](ts::Symbol const jsymEnum) noexcept {
				// Enums are declared outside of the _jsall class because we have to mark them as IsJsIntegralEnum
				// before using in js interop.
				return tc::concat(
					"enum class _enum", MangleSymbolName(jtsTypeChecker, jsymEnum), " {\n",
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
									"	", CppifyName(jsymOption), " = ",
									tc::as_dec(tc::explicit_cast<int>(double(junionOptionValue))),
									",\n"
								));
							}
						})
					),
					"};\n",
					"template<> struct IsJsIntegralEnum<_enum", MangleSymbolName(jtsTypeChecker, jsymEnum), "> : std::true_type {};\n"
				);
			})),
			"namespace _jsall {\n",
			tc::join(tc::transform(g_vecjsymEnum, [&jtsTypeChecker](ts::Symbol const jsymEnum) noexcept {
				return tc::concat(
					"	using ", MangleSymbolName(jtsTypeChecker, jsymEnum), " = _enum", MangleSymbolName(jtsTypeChecker, jsymEnum), ";\n"
				);
			})),
			tc::join(tc::transform(g_vecjsymClass, [&jtsTypeChecker](ts::Symbol const jsymClass) noexcept {
				return tc::concat(
					"	struct _impl", MangleSymbolName(jtsTypeChecker, jsymClass), ";\n",
					"	using ", MangleSymbolName(jtsTypeChecker, jsymClass), " = js_ref<_impl", MangleSymbolName(jtsTypeChecker, jsymClass), ">;\n"
				);
			})),
			tc::join(tc::transform(vecjsclassClass, [&jtsTypeChecker](SJsClass const& jsclassClass) noexcept {
				return tc::explicit_cast<std::string>(tc::concat(
					"	struct _impl", MangleSymbolName(jtsTypeChecker, jsclassClass.m_jsymClass),
					" : ",
					tc_conditional_range(
						tc::empty(jsclassClass.m_vecjsymBaseClass),
						tc::explicit_cast<std::string>("virtual IObject"),
						tc::explicit_cast<std::string>(tc::join_separated(
							tc::transform(jsclassClass.m_vecjsymBaseClass,
								[&jtsTypeChecker](ts::Symbol const jsymBaseClass) noexcept {
									return tc::concat("virtual _impl", MangleSymbolName(jtsTypeChecker, jsymBaseClass));
								}
							),
							", "
						))
					),
					" {\n",
					"		struct _js_ref_definitions {\n",
					tc::join(tc::transform(
						jsclassClass.m_vecjsymExportType,
						[&jtsTypeChecker](ts::Symbol const jsymExport) noexcept {
							return tc::concat(
								"			using ",
								CppifyName(jsymExport),
								" = ",
								MangleSymbolName(jtsTypeChecker, jsymExport),
								";\n"
							);
						}
					)),
					tc::join(tc::transform(
						jsclassClass.m_vecjsfunctionlikeExportFunction,
						[](SJsFunctionLike const& jsfunctionlikeFunction) noexcept {
							return tc::explicit_cast<std::string>(tc::concat(
								"			static auto ", CppifyName(jsfunctionlikeFunction.m_jsymFunctionLike), "(", jsfunctionlikeFunction.m_strCppifiedParameters, ") noexcept;\n"
							));
						}
					)),
					tc::join(tc::transform(
						jsclassClass.m_vecjsvariablelikeExportVariable,
						[](SJsVariableLike const &jsvariablelikeVariable) noexcept {
							return tc::concat(
								"			static auto ", jsvariablelikeVariable.m_strCppifiedName, "() noexcept;\n",
								jsvariablelikeVariable.m_bReadonly ?
									"" :
									tc::explicit_cast<std::string>(tc::concat(
										"			static void ", jsvariablelikeVariable.m_strCppifiedName, "(", jsvariablelikeVariable.m_strMangledType, " v) noexcept;\n"
									))
							);
						}
					)),
					"		};\n",
					tc::join(tc::transform(
						jsclassClass.m_vecjsvariablelikeProperty,
						[](SJsVariableLike const &jsvariablelikeProperty) noexcept {
							return tc::concat(
								"		auto ", jsvariablelikeProperty.m_strCppifiedName, "() noexcept;\n",
								jsvariablelikeProperty.m_bReadonly ?
									"" :
									tc::explicit_cast<std::string>(tc::concat(
										"		void ", jsvariablelikeProperty.m_strCppifiedName, "(", jsvariablelikeProperty.m_strMangledType, " v) noexcept;\n"
									))
							);
						}
					)),
					tc::join(tc::transform(
						jsclassClass.m_vecjsfunctionlikeMethod,
						[](SJsFunctionLike const& jsfunctionlikeMethod) noexcept {
							if (ts::SymbolFlags::Method == jsfunctionlikeMethod.m_jsymFunctionLike->getFlags()) {
								return tc::explicit_cast<std::string>(tc::concat(
									"		auto ", CppifyName(jsfunctionlikeMethod.m_jsymFunctionLike), "(", jsfunctionlikeMethod.m_strCppifiedParameters, ") noexcept;\n"
								));
							} else if (ts::SymbolFlags::Constructor == jsfunctionlikeMethod.m_jsymFunctionLike->getFlags()) {
								return tc::explicit_cast<std::string>(tc::concat(
									"		static auto _construct(", jsfunctionlikeMethod.m_strCppifiedParameters, ") noexcept;\n"
								));
							} else {
								_ASSERTFALSE;
							}
						}
					)),
					"	};\n"
				));
			})),
			tc::join(tc::transform(vecjsclassClass, [&jtsTypeChecker](SJsClass const& jsclassClass) noexcept {
				auto strClassNamespace = tc::explicit_cast<std::string>(tc::concat(
					"_impl", MangleSymbolName(jtsTypeChecker, jsclassClass.m_jsymClass), "::"
				));
				auto strClassInstanceRetrieve = RetrieveSymbolFromCpp(jsclassClass.m_jsymClass);
				return tc::explicit_cast<std::string>(tc::concat(
					tc::join(tc::transform(
						jsclassClass.m_vecjsfunctionlikeExportFunction,
						[&jtsTypeChecker, &strClassNamespace](SJsFunctionLike const& jsfunctionlikeFunction) noexcept {
							auto const rngchCallArguments = tc::join_separated(
								tc::transform(
									jsfunctionlikeFunction.m_jtsSignature->getParameters(),
									[](ts::Symbol const jsymParameter) noexcept {
										return CppifyName(jsymParameter);
									}
								),
								", "
							);
							auto const rngchFunctionCall = tc::concat(
								RetrieveSymbolFromCpp(jsfunctionlikeFunction.m_jsymFunctionLike), "(", rngchCallArguments, ")"
							);
							return tc::explicit_cast<std::string>(tc::concat(
								"	inline auto ", strClassNamespace, "_js_ref_definitions::",
									CppifyName(jsfunctionlikeFunction.m_jsymFunctionLike), "(", jsfunctionlikeFunction.m_strCppifiedParameters, ") noexcept {\n",
								ts::TypeFlags::Void == jsfunctionlikeFunction.m_jtsSignature->getReturnType()->flags()
									? tc::explicit_cast<std::string>(tc::concat("		", rngchFunctionCall, ";\n"))
									: tc::explicit_cast<std::string>(tc::concat(
										"		return ", rngchFunctionCall, ".template as<", MangleType(jtsTypeChecker, jsfunctionlikeFunction.m_jtsSignature->getReturnType()), ">();\n"
									)),
								"	}\n"
							));
						}
					)),
					tc::join(tc::transform(
						jsclassClass.m_vecjsvariablelikeExportVariable,
						[&strClassNamespace, &strClassInstanceRetrieve](SJsVariableLike const &jsvariablelikeVariable) noexcept {
							return tc::concat(
								"	inline auto ", strClassNamespace, "_js_ref_definitions::", jsvariablelikeVariable.m_strCppifiedName, "() noexcept ",
								"{ return ", strClassInstanceRetrieve, "[\"", jsvariablelikeVariable.m_strJsName, "\"].template as<", jsvariablelikeVariable.m_strMangledType, ">(); }\n",
								jsvariablelikeVariable.m_bReadonly ?
									"" :
									tc::explicit_cast<std::string>(tc::concat(
										"	inline void ", strClassNamespace, "_js_ref_definitions::", jsvariablelikeVariable.m_strCppifiedName, "(", jsvariablelikeVariable.m_strMangledType, " v) noexcept ",
										"{ ", strClassInstanceRetrieve, ".set(\"", jsvariablelikeVariable.m_strJsName, "\", v); }\n"
									))
							);
						}
					)),
					tc::join(tc::transform(
						jsclassClass.m_vecjsvariablelikeProperty,
						[&strClassNamespace](SJsVariableLike const &jsvariablelikeProperty) noexcept {
							return tc::concat(
								"	inline auto ", strClassNamespace, jsvariablelikeProperty.m_strCppifiedName, "() noexcept ",
								"{ return _getProperty<", jsvariablelikeProperty.m_strMangledType, ">(\"", jsvariablelikeProperty.m_strJsName, "\"); }\n",
								jsvariablelikeProperty.m_bReadonly ?
									"" :
									tc::explicit_cast<std::string>(tc::concat(
										"	inline void ", strClassNamespace, jsvariablelikeProperty.m_strCppifiedName, "(", jsvariablelikeProperty.m_strMangledType, " v) noexcept ",
										"{ _setProperty(\"", jsvariablelikeProperty.m_strJsName, "\", v); }\n"
									))
							);
						}
					)),
					tc::join(tc::transform(
						jsclassClass.m_vecjsfunctionlikeMethod,
						[&jtsTypeChecker, &jsclassClass, &strClassNamespace](SJsFunctionLike const& jsfunctionlikeMethod) noexcept {
							auto const rngstrArguments = tc::transform(
								jsfunctionlikeMethod.m_jtsSignature->getParameters(),
								[](ts::Symbol const jsymParameter) noexcept {
									return CppifyName(jsymParameter);
								}
							);
							if (ts::SymbolFlags::Method == jsfunctionlikeMethod.m_jsymFunctionLike->getFlags()) {
								auto const rngchCallArguments = tc::join_separated(
									tc::concat(
										tc::single(tc::concat("\"", tc::explicit_cast<std::string>(jsfunctionlikeMethod.m_jsymFunctionLike->getName()), "\"")),
										rngstrArguments
									),
									", "
								);
								return tc::explicit_cast<std::string>(tc::concat(
									"	inline auto ", strClassNamespace, CppifyName(jsfunctionlikeMethod.m_jsymFunctionLike), "(", jsfunctionlikeMethod.m_strCppifiedParameters, ") noexcept {\n",
									"		return _call<", MangleType(jtsTypeChecker, jsfunctionlikeMethod.m_jtsSignature->getReturnType()), ">(", rngchCallArguments, ");\n",
									"	}\n"
								));
							} else if (ts::SymbolFlags::Constructor == jsfunctionlikeMethod.m_jsymFunctionLike->getFlags()) {
								auto const rngchSelfType = MangleType(jtsTypeChecker, jtsTypeChecker->getDeclaredTypeOfSymbol(jsclassClass.m_jsymClass));
								auto const rngchCallArguments = tc::join_separated(rngstrArguments, ", ");
								return tc::explicit_cast<std::string>(tc::concat(
									"	inline auto ", strClassNamespace, "_construct(", jsfunctionlikeMethod.m_strCppifiedParameters, ") noexcept {\n",
									"		return ", rngchSelfType, "(", RetrieveSymbolFromCpp(jsclassClass.m_jsymClass), ".new_(", rngchCallArguments, "));\n",
									"	}\n"
								));
							} else {
								_ASSERTFALSE;
							}
						}
					))
				));
			})),
			"}; // namespace _jsall\n",
			tc::join(tc::transform(
				vecjsymExportedModule,
				[&jtsTypeChecker](ts::Symbol const& jsymSourceFile) noexcept {
					_ASSERT(IsClassInCpp(jsymSourceFile));
					return tc::explicit_cast<std::string>(tc::concat(
						"using ", CppifyName(jsymSourceFile), " = _jsall::", MangleSymbolName(jtsTypeChecker, jsymSourceFile), ";\n"
					));
				}
			)),
			"namespace globals {\n",
			tc::join(tc::transform(
				vecjsymExportedSymbol,
				[&jtsTypeChecker](ts::Symbol const& jsymExported) noexcept {
					if (IsClassInCpp(jsymExported) || IsEnumInCpp(jsymExported)) {
						return tc::explicit_cast<std::string>(tc::concat(
							"	using ", CppifyName(jsymExported), " = _jsall::", MangleSymbolName(jtsTypeChecker, jsymExported), ";\n"
						));
					} else if (ts::SymbolFlags::Function == jsymExported->getFlags()) {
						return tc::explicit_cast<std::string>(tc::join(tc::transform(
							jsymExported->declarations(),
							[&jtsTypeChecker, &jsymExported](ts::Declaration const jdeclFunction) {
								SJsFunctionLike jsfunctionlikeFunction(jtsTypeChecker, jsymExported, jdeclFunction);
								// TODO: deduplicate following code into SJsFunctionLike.
								auto const rngchCallArguments = tc::join_separated(
									tc::transform(
										jsfunctionlikeFunction.m_jtsSignature->getParameters(),
										[](ts::Symbol const jsymParameter) noexcept {
											return CppifyName(jsymParameter);
										}
									),
									", "
								);
								auto const rngchFunctionCall = tc::concat(
									RetrieveSymbolFromCpp(jsfunctionlikeFunction.m_jsymFunctionLike), "(", rngchCallArguments, ")"
								);
								return tc::explicit_cast<std::string>(tc::concat(
									"	inline auto ",
										CppifyName(jsfunctionlikeFunction.m_jsymFunctionLike), "(", jsfunctionlikeFunction.m_strCppifiedParameters, ") noexcept {\n",
									"	using namespace _jsall;\n",
									ts::TypeFlags::Void == jsfunctionlikeFunction.m_jtsSignature->getReturnType()->flags()
										? tc::explicit_cast<std::string>(tc::concat("		", rngchFunctionCall, ";\n"))
										: tc::explicit_cast<std::string>(tc::concat(
											"		return ", rngchFunctionCall, ".template as<", MangleType(jtsTypeChecker, jsfunctionlikeFunction.m_jtsSignature->getReturnType()), ">();\n"
										)),
									"	}\n"
								));
							}
						)));
					} else if (ts::SymbolFlags::FunctionScopedVariable == jsymExported->getFlags() ||
						ts::SymbolFlags::BlockScopedVariable == jsymExported->getFlags()) {
						SJsVariableLike jsvariablelikeVariable(jtsTypeChecker, jsymExported);
						// TODO: deduplicate following code into SJsVariableLike.
						return tc::explicit_cast<std::string>(tc::concat(
							"	inline auto ", jsvariablelikeVariable.m_strCppifiedName, "() noexcept ",
							"{ using namespace _jsall; return emscripten::val::global(\"", jsvariablelikeVariable.m_strJsName, "\").template as<", jsvariablelikeVariable.m_strMangledType, ">(); }\n",
							jsvariablelikeVariable.m_bReadonly ?
								"" :
								tc::explicit_cast<std::string>(tc::concat(
									"	inline void ", jsvariablelikeVariable.m_strCppifiedName, "(", jsvariablelikeVariable.m_strMangledType, " v) noexcept ",
									"{ using namespace _jsall; emscripten::val::global().set(\"", jsvariablelikeVariable.m_strJsName, "\", v); }\n"
								))
						));
					} else {
						return tc::explicit_cast<std::string>(tc::concat(
							"	/* ", tc::explicit_cast<std::string>(jtsTypeChecker->getFullyQualifiedName(jsymExported)), " flags=", tc::as_dec(static_cast<int>(jsymExported->getFlags())), "*/\n"
						));
					}
				}
			)),
			"} // namespace globals\n",
			"} // namespace tc::js\n"
		);
	}
	return 0;
}
