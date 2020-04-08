#include "precompiled.h"
#include "typescript.d.bootstrap.h"
#include "mangle.h"
#include "walk_type.h"

using tc::js::js_string;
using tc::js::js_optional;
using tc::js::js_unknown;
using tc::js::globals::console;
using tc::js::globals::ts;
using tc::js::globals::Array;
using tc::js::globals::ReadonlyArray;

std::string RetrieveSymbolFromCpp(ts::Symbol jsymSymbol) noexcept {
	std::string strSymbolName = tc::explicit_cast<std::string>(jsymSymbol->getName());
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
	if (strSourceName.front() == '"' && strSourceName.back() == '"') {
		strSourceName = strSourceName.substr(1, strSourceName.length() - 2);
	}
	std::string strResult = tc::explicit_cast<std::string>(tc::transform(
		strSourceName,
		[&strSourceName](char c) noexcept {
			if (c == '-') return '_';
			if (c == '_') return c;
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

struct SJsMethod {
	ts::Symbol m_jsymMethod;
	ts::Declaration m_jdeclMethod;
	ts::SignatureDeclaration m_jtsSignatureDeclaration;
	ts::Signature m_jtsSignature;
	js_optional<ReadonlyArray<js_unknown>> m_joptarrunkTypeParameter;
	std::string m_strCppifiedParameters;

	SJsMethod(ts::TypeChecker const jtsTypeChecker, ts::Symbol const jsymMethod, ts::Declaration const jdeclMethod) noexcept
		: m_jsymMethod(jsymMethod)
		, m_jdeclMethod(jdeclMethod)
		, m_jtsSignatureDeclaration([&]() noexcept -> ts::SignatureDeclaration {
			if (auto const jotsMethodSignature = ts()->isMethodSignature(jdeclMethod)) { // In interfaces.
				return *jotsMethodSignature;
			}
			if (auto const jotsMethodDeclaration = ts()->isMethodDeclaration(jdeclMethod)) { // In classes.
				return *jotsMethodDeclaration;
			}
			if (auto const jotsConstructorDeclaration = ts()->isConstructorDeclaration(jdeclMethod)) {
				return *jotsConstructorDeclaration;
			}
			_ASSERTFALSE;
		}())
		, m_jtsSignature(*jtsTypeChecker->getSignatureFromDeclaration(m_jtsSignatureDeclaration))
		, m_joptarrunkTypeParameter(m_jtsSignature->getTypeParameters())
		, m_strCppifiedParameters(tc::explicit_cast<std::string>(tc::join_separated(
			tc::transform(
				m_jtsSignature->getParameters(),
				[&jtsTypeChecker, &jdeclMethod](ts::Symbol const jsymParameter) noexcept {
					return tc::concat(
						MangleType(jtsTypeChecker, jtsTypeChecker->getTypeOfSymbolAtLocation(jsymParameter, jdeclMethod)),
						" ",
						CppifyName(jsymParameter)
					);
				}
			),
			", "
		)))
	{
		_ASSERTEQUAL(ts()->getCombinedModifierFlags(jdeclMethod), ts::ModifierFlags::None);
	}
};

struct SJsProperty {
	ts::Symbol m_jsymProperty;
	std::string m_strJsName;
	std::string m_strCppifiedName;
	ts::Declaration m_jdeclProperty;
	ts::Type m_jtypeProperty;
	std::string m_strMangledPropertyType;
	bool m_bReadonly;

	SJsProperty(ts::TypeChecker const jtsTypeChecker, ts::Symbol const jsymProperty) noexcept
		: m_jsymProperty(jsymProperty)
		, m_strJsName(tc::explicit_cast<std::string>(jsymProperty->getName()))
		, m_strCppifiedName(CppifyName(jsymProperty))
		, m_jdeclProperty([&]() noexcept {
			_ASSERTEQUAL(jsymProperty->declarations()->length(), 1);
			return jsymProperty->declarations()[0];
		}())
		, m_jtypeProperty(jtsTypeChecker->getTypeOfSymbolAtLocation(jsymProperty, m_jdeclProperty))
		, m_strMangledPropertyType(MangleType(jtsTypeChecker, m_jtypeProperty))
		, m_bReadonly(ts()->getCombinedModifierFlags(m_jdeclProperty) & ts::ModifierFlags::Readonly)
	{
		ts::ModifierFlags const nModifierFlags = ts()->getCombinedModifierFlags(m_jdeclProperty);
		_ASSERT(ts::ModifierFlags::None == nModifierFlags || ts::ModifierFlags::Readonly == nModifierFlags);
	}
};

struct SJsClass {
	ts::Symbol m_jsymClass;
	std::vector<ts::Symbol> m_vecjsymExportOfModule;
	std::vector<ts::Symbol> m_vecjsymExportType;
	std::vector<ts::Symbol> m_vecjsymMember;
	std::vector<SJsMethod> m_vecjsmethodMethod;
	std::vector<SJsProperty> m_vecjspropertyProperty;
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
		, m_vecjsymMember([&]() noexcept {
			if (jsymClass->members()) {
				return tc::explicit_cast<std::vector<ts::Symbol>>(*jsymClass->members());
			} else {
				return std::vector<ts::Symbol>();
			}
		}())
		, m_vecjsmethodMethod(tc::make_vector(tc::join(tc::transform(
			tc::filter(m_vecjsymMember, [](ts::Symbol const jsymMember) noexcept {
				return ts::SymbolFlags::Method == jsymMember->getFlags() || ts::SymbolFlags::Constructor == jsymMember->getFlags();
			}),
			[&jtsTypeChecker](ts::Symbol const jsymMethod) noexcept {
				return tc::transform(
					jsymMethod->declarations(),
					[&jtsTypeChecker, jsymMethod](ts::Declaration const jdeclMethod) noexcept {
						return SJsMethod(jtsTypeChecker, jsymMethod, jdeclMethod);
					}
				);
			}
		))))
		, m_vecjspropertyProperty(tc::make_vector(tc::transform(
			tc::filter(m_vecjsymMember, [](ts::Symbol const jsymMember) noexcept {
				return ts::SymbolFlags::Property == jsymMember->getFlags();
			}),
			[&](ts::Symbol const jsymProperty) noexcept {
				return SJsProperty(jtsTypeChecker, jsymProperty);
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
	tc::for_each(jtsProgram->getSourceFiles(),
		[&](ts::SourceFile const& jtsSourceFile) noexcept {
			if (!tc::find_unique<tc::return_bool>(rngstrFileNames, tc::explicit_cast<std::string>(jtsSourceFile->fileName()))) {
				return;
			}
			auto const josymSourceFile = jtsTypeChecker->getSymbolAtLocation(jtsSourceFile);
			if (!josymSourceFile) {
				tc::append(std::cerr, "Module not found for ", tc::explicit_cast<std::string>(jtsSourceFile->fileName()), "\n");
				return;
			}
			tc::cont_emplace_back(vecjsymExportedModule, *josymSourceFile);
		}
	);

	tc::for_each(
		vecjsymExportedModule,
		[&](ts::Symbol const& jsymSourceFile) noexcept {
			tc::append(std::cerr, "Module name is ", tc::explicit_cast<std::string>(jsymSourceFile->getName()), "\n");
			WalkType(jtsTypeChecker, 0, jsymSourceFile);
		}
	);

	tc::append(std::cerr, "\n========== GENERATED CODE ==========\n");

	{
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
					"		};\n",
					tc::join(tc::transform(
						jsclassClass.m_vecjspropertyProperty,
						[](SJsProperty const &jspropertyProperty) noexcept {
							return tc::concat(
								"		auto ", jspropertyProperty.m_strCppifiedName, "() noexcept;\n",
								jspropertyProperty.m_bReadonly ?
									"" :
									tc::explicit_cast<std::string>(tc::concat(
										"		void ", jspropertyProperty.m_strCppifiedName, "(", jspropertyProperty.m_strMangledPropertyType, " v) noexcept;\n"
									))
							);
						}
					)),
					tc::join(tc::transform(
						jsclassClass.m_vecjsmethodMethod,
						[](SJsMethod const& jsmethodMethod) noexcept {
							if (ts::SymbolFlags::Method == jsmethodMethod.m_jsymMethod->getFlags()) {
								return tc::explicit_cast<std::string>(tc::concat(
									"		auto ", CppifyName(jsmethodMethod.m_jsymMethod), "(", jsmethodMethod.m_strCppifiedParameters, ") noexcept;\n"
								));
							} else if (ts::SymbolFlags::Constructor == jsmethodMethod.m_jsymMethod->getFlags()) {
								return tc::explicit_cast<std::string>(tc::concat(
									"		static auto _construct(", jsmethodMethod.m_strCppifiedParameters, ") noexcept;\n"
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
				return tc::explicit_cast<std::string>(tc::concat(
					tc::join(tc::transform(
						jsclassClass.m_vecjspropertyProperty,
						[&strClassNamespace](SJsProperty const &jspropertyProperty) noexcept {
							return tc::concat(
								"	auto ", strClassNamespace, jspropertyProperty.m_strCppifiedName, "() noexcept ",
								"{ return _getProperty<", jspropertyProperty.m_strMangledPropertyType, ">(\"", jspropertyProperty.m_strJsName, "\"); }\n",
								jspropertyProperty.m_bReadonly ?
									"" :
									tc::explicit_cast<std::string>(tc::concat(
										"	void ", strClassNamespace, jspropertyProperty.m_strCppifiedName, "(", jspropertyProperty.m_strMangledPropertyType, " v) noexcept ",
										"{ _setProperty(\"", jspropertyProperty.m_strJsName, "\", v); }\n"
									))
							);
						}
					)),
					tc::join(tc::transform(
						jsclassClass.m_vecjsmethodMethod,
						[&jtsTypeChecker, &jsclassClass, &strClassNamespace](SJsMethod const& jsmethodMethod) noexcept {
							auto const rngstrArguments = tc::transform(
								jsmethodMethod.m_jtsSignature->getParameters(),
								[](ts::Symbol const jsymParameter) noexcept {
									return CppifyName(jsymParameter);
								}
							);
							if (ts::SymbolFlags::Method == jsmethodMethod.m_jsymMethod->getFlags()) {
								auto const rngchCallArguments = tc::join_separated(
									tc::concat(
										tc::single(tc::concat("\"", tc::explicit_cast<std::string>(jsmethodMethod.m_jsymMethod->getName()), "\"")),
										rngstrArguments
									),
									", "
								);
								return tc::explicit_cast<std::string>(tc::concat(
									"	auto ", strClassNamespace, CppifyName(jsmethodMethod.m_jsymMethod), "(", jsmethodMethod.m_strCppifiedParameters, ") noexcept {\n",
									"		return _call<", MangleType(jtsTypeChecker, jsmethodMethod.m_jtsSignature->getReturnType()), ">(", rngchCallArguments, ");\n",
									"	}\n"
								));
							} else if (ts::SymbolFlags::Constructor == jsmethodMethod.m_jsymMethod->getFlags()) {
								auto const rngchSelfType = MangleType(jtsTypeChecker, jtsTypeChecker->getDeclaredTypeOfSymbol(jsclassClass.m_jsymClass));
								auto const rngchCallArguments = tc::join_separated(rngstrArguments, ", ");
								return tc::explicit_cast<std::string>(tc::concat(
									"	auto ", strClassNamespace, "_construct(", jsmethodMethod.m_strCppifiedParameters, ") noexcept {\n",
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
			"} // namespace tc::js\n"
		);
	}
	return 0;
}
