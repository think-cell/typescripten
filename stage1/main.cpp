#include "precompiled.h"
#include "typescript.d.bootstrap.h"
#include "mangle.h"
#include "walk_symbol.h"
#include "jstypes.h"

using tc::jst::create_js_object;
using tc::jst::js_string;
using tc::jst::js_optional;
using tc::jst::js_unknown;
using tc::js::console;
using tc::js::ts;
using tc::js::Array;
using tc::js::ReadonlyArray;

namespace {
	std::string RetrieveSymbolFromCpp(ts::Symbol jsymSymbol) noexcept {
		std::string strSymbolName = tc::explicit_cast<std::string>(jsymSymbol->getName());
		if ('"' == strSymbolName.front() && '"' == strSymbolName.back()) {
			strSymbolName = strSymbolName.substr(1, strSymbolName.length() - 2);
		}
		return tc::explicit_cast<std::string>(
			tc_conditional_range(
				!tc::js::ts_ext::Symbol(jsymSymbol)->parent(),
				tc::concat("emscripten::val::global(\"", strSymbolName, "\")"),
				tc::concat(
					RetrieveSymbolFromCpp(*tc::js::ts_ext::Symbol(jsymSymbol)->parent()),
					"[\"", strSymbolName, "\"]"
				)
			)
		);
	}


	template<typename SetJsXXX, typename Func>
	void SortDeclarationOrder(ts::TypeChecker jtsTypeChecker, SetJsXXX& setjs, Func ForEachChildSymbol) noexcept {
		std::unordered_set<std::string> setstrSeen;
		auto itjsSorted = setjs.begin();

		auto dfs = [&](typename SetJsXXX::iterator itjs, auto const& dfs) {
			if(!setstrSeen.insert(itjs->m_strMangledName).second) {
				return;
			}
			ForEachChildSymbol(itjs, [&](ts::Symbol const& jsym) {
				if(auto const itjsSymbol = tc::cont_find<tc::return_element_or_null>(
					setjs.template get<1>(),
					MangleSymbolName(jtsTypeChecker, jsym)
				)) { // stop dfs when the child class is not part of our source files
					dfs(setjs.template project<0>(tc::base_cast< decltype(setjs.template get<1>().begin()) >(itjsSymbol)), dfs);
				}
			});
			setjs.relocate(itjsSorted, itjs);
		};

		while(itjsSorted!=setjs.end()) {
			dfs(itjsSorted, dfs);
			++itjsSorted;
		}
	}
}

SetJsEnum g_setjsenum;
SetJsClass g_setjsclass;
SetJsTypeAlias g_setjstypealias;

int main(int argc, char* argv[]) {
	_ASSERT(2 <= argc);

	tc::js::ts_ext::CompilerOptions const jtsCompilerOptions(create_js_object);
	jtsCompilerOptions->strict(true);
	jtsCompilerOptions->target(ts::ScriptTarget::ES5);
	jtsCompilerOptions->module(ts::ModuleKind::CommonJS);

	auto const rngstrFileNames = tc::make_iterator_range(argv + 1, argv + argc);
	tc::for_each(rngstrFileNames, [](tc::ptr_range<char> rngch) noexcept {
		tc::for_each(rngch, [](auto& ch) noexcept { 
			if('\\'==ch) ch = '/'; // typescript createProgram does not support backslashes 
		});
	});
	ts::Program const jtsProgram = ts::createProgram(ReadonlyArray<js_string>(create_js_object, rngstrFileNames), jtsCompilerOptions);

	ts::TypeChecker const jtsTypeChecker = jtsProgram->getTypeChecker();

	{
		auto const jtsReadOnlyArrayDiagnostics = ts::getPreEmitDiagnostics(jtsProgram);
		if (jtsReadOnlyArrayDiagnostics->length()) {
			console::log(ts::formatDiagnosticsWithColorAndContext(jtsReadOnlyArrayDiagnostics, ts::FormatDiagnosticsHost(ts::createCompilerHost(jtsCompilerOptions).getEmval())));
			return 1;
		}
	}

	// Iterate over source files, enumerate global symbols and then walk the tree of child symbols
	SJsScope scopeGlobal(
		jtsTypeChecker,
		[&]() noexcept {
			std::vector<ts::Symbol> vecjsymExportedSymbol;
			tc::for_each(jtsProgram->getSourceFiles(),
				[&](ts::SourceFile const& jtsSourceFile) noexcept {
					if (!tc::find_unique<tc::return_bool>(rngstrFileNames, tc::explicit_cast<std::string>(jtsSourceFile->fileName()))) {
						return;
					}
					auto const josymSourceFile = jtsTypeChecker->getSymbolAtLocation(jtsSourceFile);
					if (!josymSourceFile) {
						tc::append(std::cerr, "Module not found for ", tc::explicit_cast<std::string>(jtsSourceFile->fileName()), ", treating at as a global library\n");

						{
							ts::forEachChild(jtsSourceFile, tc::jst::js_lambda_wrap([&](ts::Node jnodeChild) noexcept -> tc::jst::js_unknown {
								if (auto const jotsFunctionDeclaration = tc::js::ts_ext::isFunctionDeclaration(jnodeChild)) {
									tc::cont_emplace_back(vecjsymExportedSymbol, jtsTypeChecker->getSymbolAtLocation(*(*jotsFunctionDeclaration)->name()));
								} else if (auto const jotsVariableStatement = tc::js::ts_ext::isVariableStatement(jnodeChild)) {
									tc::for_each(tc::js::ts_ext::MakeReadOnlyArray<ts::VariableDeclaration>((*jotsVariableStatement)->declarationList()->declarations()), [&](ts::VariableDeclaration const jtsVariableDeclaration) {
										tc::cont_emplace_back(vecjsymExportedSymbol, jtsTypeChecker->getSymbolAtLocation(jtsVariableDeclaration->name()));
									});
								} else if (auto const jotsClassDeclaration = tc::js::ts_ext::isClassDeclaration(jnodeChild)) {
									tc::cont_emplace_back(vecjsymExportedSymbol, jtsTypeChecker->getSymbolAtLocation(*(*jotsClassDeclaration)->name()));
								} else if (auto const jotsInterfaceDeclaration = tc::js::ts_ext::isInterfaceDeclaration(jnodeChild)) {
									tc::cont_emplace_back(vecjsymExportedSymbol, jtsTypeChecker->getSymbolAtLocation((*jotsInterfaceDeclaration)->name()));
								} else if (auto const jotsEnumDeclaration = tc::js::ts_ext::isEnumDeclaration(jnodeChild)) {
									tc::cont_emplace_back(vecjsymExportedSymbol, jtsTypeChecker->getSymbolAtLocation((*jotsEnumDeclaration)->name()));
								} else if (auto const jotsModuleDeclaration = tc::js::ts_ext::isModuleDeclaration(jnodeChild)) {
									tc::cont_emplace_back(vecjsymExportedSymbol, jtsTypeChecker->getSymbolAtLocation((*jotsModuleDeclaration)->name()));
								} else if(auto const jotsTypeAliasDeclaration = tc::js::ts_ext::isTypeAliasDeclaration(jnodeChild)) {
									tc::cont_emplace_back(vecjsymExportedSymbol, jtsTypeChecker->getSymbolAtLocation((*jotsTypeAliasDeclaration)->name()));
								} else if (jnodeChild->kind() == ts::SyntaxKind::EndOfFileToken) {
									// Do nothing
								} else {
									tc::append(std::cerr, "Unknown source file-level child kind: ", tc::as_dec(static_cast<int>(jnodeChild->kind())), "\n");
								}
								return tc::jst::js_undefined();
							}));

							// An interface declaration and a variable statement may introduce the same symbol, e.g., 
							// interface EventTarget {
							// }

							// var EventTarget: {
							// 	prototype: EventTarget;
							// 	new(): EventTarget;
							// };
							//
							// These define the same C++ class

							// Overloaded functions also define the same symbol with two declarations:

							// function test(a: number);
							// function test(a: string);

							// For more combinations see tests/duplicate_symbols and https://www.typescriptlang.org/docs/handbook/declaration-files/deep-dive.html#advanced-combinations
							return;
						}
					}
					tc::cont_emplace_back(vecjsymExportedSymbol, *josymSourceFile);
				}
			);

			// Make list of exported symbols unique
			tc::sort_unique_inplace(vecjsymExportedSymbol, tc::projected(tc::fn_less(), [&](ts::Symbol jsym) noexcept {
				return static_cast<std::string>(jtsTypeChecker->getFullyQualifiedName(jsym));
			}));

			tc::for_each(
				vecjsymExportedSymbol,
				[&](ts::Symbol const& jsymSourceFile) noexcept {
					tc::append(std::cerr, "Global symbol/module name is ", tc::explicit_cast<std::string>(jsymSourceFile->getName()), "\n");
					PrintSymbolTree(jtsTypeChecker, 0, jsymSourceFile);
				}
			);
			
			return vecjsymExportedSymbol;
		}()
	);
	scopeGlobal.WalkSymbols(jtsTypeChecker);

	tc::append(std::cerr, "\n========== GENERATED CODE ==========\n");

	{
		// TODO: Do this in constructors or in SJsScope::Initialize?
		tc::for_each(g_setjsclass, [&](SJsClass const& jsclass) noexcept {
			g_usstrAllowedMangledTypes.insert(jsclass.m_strMangledName);
		});

		tc::for_each(g_setjsenum, [&](SJsEnum const& jsenum) noexcept {
			g_usstrAllowedMangledTypes.insert(jsenum.m_strMangledName);
		});

		tc::for_each(g_setjstypealias, [&](SJsTypeAlias const& jstypealias) noexcept {
			g_usstrAllowedMangledTypes.insert(jstypealias.m_strMangledName);
		});

		// Find correct declaration order by going depth-first over class hierarchy
		// Typescript has no requirements on declaration order of type aliases. This is valid typescript:
		// type FooBar2 = FooBar;
		// type FooBar = number | string;
		// In C++, FooBar must be declared before FooBar2.
		// For each type alias declaration, we iterate depth-first over all type aliases referenced on the right-hand side
		// and emit them first.

		// Sort the sequenced indices of g_setjsclass/g_setjstypealias
		SortDeclarationOrder(
			jtsTypeChecker,
			g_setjsclass, 
			[](SetJsClass::iterator itjsclass, auto ForEachChildSymbol) noexcept {
				tc::for_each(itjsclass->m_vecjsymBaseClass, ForEachChildSymbol);
			}
		);
		SortDeclarationOrder(
			jtsTypeChecker,
			g_setjstypealias, 
			[&](SetJsTypeAlias::iterator itjstypealias, auto ForEachChildSymbol) noexcept {
				ForEachChildTypeNode(itjstypealias->m_jtypenode, [&](ts::TypeNode jtypenode) noexcept {
					if(ts::SyntaxKind::TypeReference==jtypenode->kind()) {
						if(auto const ojsym = jtsTypeChecker->getTypeFromTypeNode(jtypenode)->aliasSymbol()) {
							ForEachChildSymbol(*ojsym);
						}
						return erecurseSKIP;
					} else {
						return erecurseCONTINUE;
					}
				});
			}
		);

		tc::append(std::cout,
			"namespace tc::js_defs {\n",
			tc::join(tc::transform(g_setjsenum, [](SJsEnum const& jsenumEnum) noexcept {
				// We have to mark enums as IsJsIntegralEnum before using in js interop.
				return tc::concat(
					"enum class _enum", jsenumEnum.m_strMangledName, " {\n",
					tc::join(
						tc::transform(jsenumEnum.m_vecjsenumoption, [&jsenumEnum](SJsEnumOption const& jsenumoption) noexcept {
							return tc_conditional_range(
								jsenumoption.m_ovardblstrValue,
								tc::concat(
									"	", jsenumoption.m_strCppifiedName,
									tc_conditional_range(
										jsenumEnum.m_bIsIntegral,
										tc::concat(" = ", tc::as_dec(tc::explicit_cast<int>(std::get<double>(*jsenumoption.m_ovardblstrValue))))
									),
									",\n"
								),
								tc::concat(
									"	/*", jsenumoption.m_strJsName, " = ??? */\n"
								)
							);
						})
					),
					"};\n"
				);
			})),
			"} // namespace tc::js_defs\n",
			"namespace tc::jst {\n",
			tc::join(tc::transform(g_setjsenum, [](SJsEnum const& jsenumEnum) noexcept {
				// Enums are declared outside of the _jsall class because we have to mark them as IsJsIntegralEnum
				// before using in js interop.
				return tc_conditional_range(
					jsenumEnum.m_bIsIntegral,
					tc::concat(
						"template<> struct IsJsIntegralEnum<js_defs::_enum", jsenumEnum.m_strMangledName, "> : std::true_type {};\n"
					),
					tc::concat(
						"template<> struct IsJsHeterogeneousEnum<js_defs::_enum", jsenumEnum.m_strMangledName, "> : std::true_type {\n",
						"	static inline auto const& Values() {\n",
						"		using E = js_defs::_enum", jsenumEnum.m_strMangledName, ";\n",
						"		static tc::unordered_map<E, jst::js_unknown> vals{\n",
						tc::join_separated(
							tc::transform(
								tc::filter(jsenumEnum.m_vecjsenumoption, TC_MEMBER(.m_ovardblstrValue)),
								[](SJsEnumOption const& jsenumoption) noexcept {
									_ASSERT(jsenumoption.m_ovardblstrValue);
									return tc::concat(
										"			{E::", jsenumoption.m_strCppifiedName, ", ",
										tc::visit(*jsenumoption.m_ovardblstrValue,
											[](double dblValue) {
												// TODO: std::to_string because of floating-point numbers. May be not enough precision.
												return tc::explicit_cast<std::string>(tc::concat("js_unknown(", std::to_string(dblValue), ")"));
											},
											[](std::string const& strValue) {
												return tc::explicit_cast<std::string>(tc::concat("js_string(\"", strValue, "\")"));
											}
										),
										"}"
									);
								}
							),
							",\n"
						),
						"\n",
						"		};\n",
						"		return vals;\n",
						"	}\n",
						"};\n"
					)
				);
			})),
			"} // namespace tc::jst\n",
			"namespace tc::js_defs {\n",
			"	using namespace jst; // no ADL\n",
			tc::join(tc::transform(g_setjsenum, [](SJsEnum const& jsenumEnum) noexcept {
				return tc::concat(
					"	using ", jsenumEnum.m_strMangledName, " = _enum", jsenumEnum.m_strMangledName, ";\n"
				);
			})),
			tc::join(tc::transform(g_setjsclass, [](SJsClass const& jsclassClass) noexcept {
				return tc::concat(
					"	struct _impl", jsclassClass.m_strMangledName, ";\n",
					"	using ", jsclassClass.m_strMangledName, " = js_ref<_impl", jsclassClass.m_strMangledName, ">;\n"
				);
			})),
			tc::join(tc::transform(g_setjstypealias, [&](SJsTypeAlias const& jtypealias) noexcept {
				return tc::explicit_cast<std::string>(
					tc::concat(
						"	using ", jtypealias.m_strMangledName, " = ", jtypealias.m_mt.m_strWithComments,
						";\n"
					)
				);
			})),
			tc::join(tc::transform(g_setjsclass, [&jtsTypeChecker](SJsClass const& jsclassClass) noexcept {
				std::vector<std::string> vecstrBaseClass, vecstrUnknownBaseClass;
				 tc::for_each(
					jsclassClass.m_vecjsymBaseClass,
					[&](ts::Symbol const jsymBaseClass) noexcept {
						auto strMangledName = MangleSymbolName(jtsTypeChecker, jsymBaseClass);
						if (g_usstrAllowedMangledTypes.count(strMangledName)) {
							tc::cont_emplace_back(vecstrBaseClass, strMangledName);
						} else {
							tc::cont_emplace_back(vecstrUnknownBaseClass, strMangledName);
						}
					}
				);
				return tc::explicit_cast<std::string>(tc::concat(
					"	struct _impl", jsclassClass.m_strMangledName,
					" : ",
					tc_conditional_range(
						tc::empty(vecstrBaseClass),
						"virtual IObject",
						tc::join_separated(
							tc::transform(vecstrBaseClass,
								[](std::string const& strMangledName) noexcept {
									return tc::concat("virtual _impl", strMangledName);
								}
							),
							", "
						)
					),
					tc::join(tc::transform(
						vecstrUnknownBaseClass,
						[](std::string const& strMangledName) noexcept {
							return tc::concat("/* : ", strMangledName, " */");
						}
					)),
					" {\n",
					"		struct _tcjs_definitions {\n",
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
							return tc::concat(
								"			static auto ", jsfunctionlikeFunction.m_strCppifiedName, "(", jsfunctionlikeFunction.m_strCppifiedParametersWithCommentsDecl, ") noexcept;\n"
							);
						}
					)),
					tc::join(tc::transform(
						jsclassClass.m_vecjsvariablelikeExportVariable,
						[](SJsVariableLike const& jsvariablelikeVariable) noexcept {
							return tc::concat(
								"			static auto ", jsvariablelikeVariable.m_strCppifiedName, "() noexcept;\n",
								tc_conditional_range(
									jsvariablelikeVariable.m_bReadonly,
									"",
									tc::concat(
										"			static void ", jsvariablelikeVariable.m_strCppifiedName, "(", jsvariablelikeVariable.m_mtType.m_strWithComments, " v) noexcept;\n"
									)
								)
							);
						}
					)),
					"		};\n",
					tc::join(tc::transform(
						jsclassClass.m_vecjsvariablelikeProperty,
						[](SJsVariableLike const& jsvariablelikeProperty) noexcept {
							return tc::concat(
								"		auto ", jsvariablelikeProperty.m_strCppifiedName, "() noexcept;\n",
								tc_conditional_range(
									jsvariablelikeProperty.m_bReadonly,
									"",
									tc::concat(
										"		void ", jsvariablelikeProperty.m_strCppifiedName, "(", jsvariablelikeProperty.m_mtType.m_strWithComments, " v) noexcept;\n"
									)
								)
							);
						}
					)),
					tc::join(tc::transform(
						jsclassClass.m_vecjsfunctionlikeMethod,
						[](SJsFunctionLike const& jsfunctionlikeMethod) noexcept {
							if (ts::SymbolFlags::Method == jsfunctionlikeMethod.m_jsymFunctionLike->getFlags()) {
								return tc::explicit_cast<std::string>(tc::concat(
									"		auto ", jsfunctionlikeMethod.m_strCppifiedName, "(", jsfunctionlikeMethod.m_strCppifiedParametersWithCommentsDecl, ") noexcept;\n"
								));
							} else if (ts::SymbolFlags::Constructor == jsfunctionlikeMethod.m_jsymFunctionLike->getFlags()) {
								return tc::explicit_cast<std::string>(tc::concat(
									"		static auto _tcjs_construct(", jsfunctionlikeMethod.m_strCppifiedParametersWithCommentsDecl, ") noexcept;\n"
								));
							} else {
								_ASSERTFALSE;
							}
						}
					)),
					tc_conditional_range(
						jsclassClass.m_bHasImplicitDefaultConstructor,
						"		static auto _tcjs_construct() noexcept;\n"
					),
					"	};\n"
				));
			})),
			tc::join(tc::transform(g_setjsclass, [&jtsTypeChecker](SJsClass const& jsclassClass) noexcept {
				auto strClassNamespace = tc::explicit_cast<std::string>(tc::concat(
					"_impl", jsclassClass.m_strMangledName, "::"
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
								"	inline auto ", strClassNamespace, "_tcjs_definitions::",
									jsfunctionlikeFunction.m_strCppifiedName, "(", jsfunctionlikeFunction.m_strCppifiedParametersWithCommentsDef, ") noexcept {\n",
								tc_conditional_range(
									ts::TypeFlags::Void == jsfunctionlikeFunction.m_jtsSignature->getReturnType()->flags(),
									tc::concat("		", rngchFunctionCall, ";\n"),
									tc::concat(
										"		return ", rngchFunctionCall, ".template as<", MangleType(jtsTypeChecker, jsfunctionlikeFunction.m_jtsSignature->getReturnType()).m_strWithComments, ">();\n"
									)
								),
								"	}\n"
							));
						}
					)),
					tc::join(tc::transform(
						jsclassClass.m_vecjsvariablelikeExportVariable,
						[&strClassNamespace, &strClassInstanceRetrieve](SJsVariableLike const& jsvariablelikeVariable) noexcept {
							return tc::concat(
								"	inline auto ", strClassNamespace, "_tcjs_definitions::", jsvariablelikeVariable.m_strCppifiedName, "() noexcept ",
								"{ return ", strClassInstanceRetrieve, "[\"", jsvariablelikeVariable.m_strJsName, "\"].template as<", jsvariablelikeVariable.m_mtType.m_strWithComments, ">(); }\n",
								tc_conditional_range(
									jsvariablelikeVariable.m_bReadonly,
									"",
									tc::concat(
										"	inline void ", strClassNamespace, "_tcjs_definitions::", jsvariablelikeVariable.m_strCppifiedName, "(", jsvariablelikeVariable.m_mtType.m_strWithComments, " v) noexcept ",
										"{ ", strClassInstanceRetrieve, ".set(\"", jsvariablelikeVariable.m_strJsName, "\", v); }\n"
									)
								)
							);
						}
					)),
					tc::join(tc::transform(
						jsclassClass.m_vecjsvariablelikeProperty,
						[&strClassNamespace](SJsVariableLike const& jsvariablelikeProperty) noexcept {
							return tc::concat(
								"	inline auto ", strClassNamespace, jsvariablelikeProperty.m_strCppifiedName, "() noexcept ",
								"{ return _getProperty<", jsvariablelikeProperty.m_mtType.m_strWithComments, ">(\"", jsvariablelikeProperty.m_strJsName, "\"); }\n",
								tc_conditional_range(
									jsvariablelikeProperty.m_bReadonly,
									"",
									tc::concat(
										"	inline void ", strClassNamespace, jsvariablelikeProperty.m_strCppifiedName, "(", jsvariablelikeProperty.m_mtType.m_strWithComments, " v) noexcept ",
										"{ _setProperty(\"", jsvariablelikeProperty.m_strJsName, "\", v); }\n"
									)
								)
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
									"	inline auto ", strClassNamespace, jsfunctionlikeMethod.m_strCppifiedName, "(", jsfunctionlikeMethod.m_strCppifiedParametersWithCommentsDef, ") noexcept {\n",
									"		return _call<", MangleType(jtsTypeChecker, jsfunctionlikeMethod.m_jtsSignature->getReturnType()).m_strWithComments, ">(", rngchCallArguments, ");\n",
									"	}\n"
								));
							} else if (ts::SymbolFlags::Constructor == jsfunctionlikeMethod.m_jsymFunctionLike->getFlags()) {
								auto const rngchCallArguments = tc::join_separated(rngstrArguments, ", ");
								return tc::explicit_cast<std::string>(tc::concat(
									"	inline auto ", strClassNamespace, "_tcjs_construct(", jsfunctionlikeMethod.m_strCppifiedParametersWithCommentsDef, ") noexcept {\n",
									"		return ", jsclassClass.m_strMangledName, "(", RetrieveSymbolFromCpp(jsclassClass.m_jsymClass), ".new_(", rngchCallArguments, "));\n",
									"	}\n"
								));
							} else {
								_ASSERTFALSE;
							}
						}
					)),
					tc_conditional_range(
						jsclassClass.m_bHasImplicitDefaultConstructor,
						tc::concat(
							"	inline auto ", strClassNamespace, "_tcjs_construct() noexcept {\n",
							"		return ", jsclassClass.m_strMangledName, "(", RetrieveSymbolFromCpp(jsclassClass.m_jsymClass), ".new_());\n",
							"	}\n"
						)
					)
				));
			}))
		);

		tc::append(std::cout,
			tc::join(tc::transform(
				scopeGlobal.m_vecjsfunctionlikeExportFunction,
				[&jtsTypeChecker](SJsFunctionLike& jsfunctionlikeFunction) {
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
							jsfunctionlikeFunction.m_strCppifiedName, "(", jsfunctionlikeFunction.m_strCppifiedParametersWithCommentsDef, ") noexcept {\n",
						tc_conditional_range(
							ts::TypeFlags::Void == jsfunctionlikeFunction.m_jtsSignature->getReturnType()->flags(),
							tc::concat("		", rngchFunctionCall, ";\n"),
							tc::concat(
								"		return ", rngchFunctionCall, ".template as<", MangleType(jtsTypeChecker, jsfunctionlikeFunction.m_jtsSignature->getReturnType()).m_strWithComments, ">();\n"
							)
						),
						"	}\n"
					));
				}
			)),
			tc::join(tc::transform(
				scopeGlobal.m_vecjsvariablelikeExportVariable,
				[&](SJsVariableLike const& jsvariablelikeVariable) noexcept {
					// TODO: deduplicate following code into SJsVariableLike.
					return tc::explicit_cast<std::string>(tc::concat(
						"	inline auto ", jsvariablelikeVariable.m_strCppifiedName, "() noexcept ",
						"{ return emscripten::val::global(\"", jsvariablelikeVariable.m_strJsName, "\").template as<", jsvariablelikeVariable.m_mtType.m_strWithComments, ">(); }\n",
						tc_conditional_range(
							jsvariablelikeVariable.m_bReadonly,
							"",
							tc::concat(
								"	inline void ", jsvariablelikeVariable.m_strCppifiedName, "(", jsvariablelikeVariable.m_mtType.m_strWithComments, " v) noexcept ",
								"{ emscripten::val::global().set(\"", jsvariablelikeVariable.m_strJsName, "\", v); }\n"
							)
						)
					));
				}
			)),
			"}; // namespace tc::js_defs\n",
			"namespace tc::js {\n"
		);

		tc::append(std::cout,
			tc::join(tc::transform(
				scopeGlobal.m_vecjsymExportType,
				[&jtsTypeChecker](ts::Symbol const& jsymType) noexcept {
					return tc::explicit_cast<std::string>(tc::concat(
						"	using ", CppifyName(jsymType), " = js_defs::", MangleSymbolName(jtsTypeChecker, jsymType), ";\n"
					));
				}
			)),
			tc::join(tc::transform(
				scopeGlobal.m_vecjsfunctionlikeExportFunction,
				[](SJsFunctionLike& jsfunctionlikeFunction) {
					return tc::concat("	using js_defs::", jsfunctionlikeFunction.m_strCppifiedName, ";\n");
				}
			)),
			tc::join(tc::transform(
				scopeGlobal.m_vecjsvariablelikeExportVariable,
				[](SJsVariableLike const& jsvariablelikeVariable) noexcept {
					return tc::concat("	using js_defs::", jsvariablelikeVariable.m_strCppifiedName, ";\n");
				}
			)),
			"} // namespace tc::js\n"
		);
	}
	return 0;
}
