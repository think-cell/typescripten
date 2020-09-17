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

std::optional<ts::TypeChecker> g_ojtsTypeChecker;
bool g_bGlobalScopeConstructionComplete = true;

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
	void SortDeclarationOrder(SetJsXXX& setjs, Func ForEachChild) noexcept {
		std::unordered_set<std::string> setstrSeen;
		auto itjsSorted = setjs.begin();

		auto dfs = [&](typename SetJsXXX::iterator itjs, auto const& dfs) {
			if(!setstrSeen.insert(itjs->m_strMangledName).second) {
				return;
			}
			ForEachChild(itjs, [&](auto const& itjs) {
				dfs(setjs.template project<0>(tc::base_cast< decltype(setjs.template get<1>().begin()) >(itjs)), dfs);
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
	(*g_ojtsTypeChecker) = jtsProgram->getTypeChecker();

	{
		auto const jtsReadOnlyArrayDiagnostics = ts::getPreEmitDiagnostics(jtsProgram);
		if (jtsReadOnlyArrayDiagnostics->length()) {
			console::log(ts::formatDiagnosticsWithColorAndContext(jtsReadOnlyArrayDiagnostics, ts::FormatDiagnosticsHost(ts::createCompilerHost(jtsCompilerOptions).getEmval())));
			return 1;
		}
	}

	// Iterate over source files, enumerate global symbols and then walk the tree of child symbols
	SJsScope scopeGlobal(
		[&]() noexcept {
			std::vector<ts::Symbol> vecjsymExportedSymbol;
			tc::for_each(jtsProgram->getSourceFiles(),
				[&](ts::SourceFile const& jtsSourceFile) noexcept {
					if (!tc::find_unique<tc::return_bool>(rngstrFileNames, tc::explicit_cast<std::string>(jtsSourceFile->fileName()))) {
						return;
					}
					auto const josymSourceFile = (*g_ojtsTypeChecker)->getSymbolAtLocation(jtsSourceFile);
					if (!josymSourceFile) {
						tc::append(std::cerr, "Module not found for ", tc::explicit_cast<std::string>(jtsSourceFile->fileName()), ", treating at as a global library\n");

						{
							ts::forEachChild(jtsSourceFile, tc::jst::js_lambda_wrap([&](ts::Node jnodeChild) noexcept -> tc::jst::js_unknown {
								if (auto const jotsFunctionDeclaration = tc::js::ts_ext::isFunctionDeclaration(jnodeChild)) {
									tc::cont_emplace_back(vecjsymExportedSymbol, (*g_ojtsTypeChecker)->getSymbolAtLocation(*(*jotsFunctionDeclaration)->name()));
								} else if (auto const jotsVariableStatement = tc::js::ts_ext::isVariableStatement(jnodeChild)) {
									tc::for_each(tc::js::ts_ext::MakeReadOnlyArray<ts::VariableDeclaration>((*jotsVariableStatement)->declarationList()->declarations()), [&](ts::VariableDeclaration const jtsVariableDeclaration) {
										tc::cont_emplace_back(vecjsymExportedSymbol, (*g_ojtsTypeChecker)->getSymbolAtLocation(jtsVariableDeclaration->name()));
									});
								} else if (auto const jotsClassDeclaration = tc::js::ts_ext::isClassDeclaration(jnodeChild)) {
									tc::cont_emplace_back(vecjsymExportedSymbol, (*g_ojtsTypeChecker)->getSymbolAtLocation(*(*jotsClassDeclaration)->name()));
								} else if (auto const jotsInterfaceDeclaration = tc::js::ts_ext::isInterfaceDeclaration(jnodeChild)) {
									tc::cont_emplace_back(vecjsymExportedSymbol, (*g_ojtsTypeChecker)->getSymbolAtLocation((*jotsInterfaceDeclaration)->name()));
								} else if (auto const jotsEnumDeclaration = tc::js::ts_ext::isEnumDeclaration(jnodeChild)) {
									tc::cont_emplace_back(vecjsymExportedSymbol, (*g_ojtsTypeChecker)->getSymbolAtLocation((*jotsEnumDeclaration)->name()));
								} else if (auto const jotsModuleDeclaration = tc::js::ts_ext::isModuleDeclaration(jnodeChild)) {
									tc::cont_emplace_back(vecjsymExportedSymbol, (*g_ojtsTypeChecker)->getSymbolAtLocation((*jotsModuleDeclaration)->name()));
								} else if(auto const jotsTypeAliasDeclaration = tc::js::ts_ext::isTypeAliasDeclaration(jnodeChild)) {
									tc::cont_emplace_back(vecjsymExportedSymbol, (*g_ojtsTypeChecker)->getSymbolAtLocation((*jotsTypeAliasDeclaration)->name()));
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
				return FullyQualifiedName(jsym);
			}));

			tc::for_each(
				vecjsymExportedSymbol,
				[&](ts::Symbol const& jsymSourceFile) noexcept {
					tc::append(std::cerr, "Global symbol/module name is ", tc::explicit_cast<std::string>(jsymSourceFile->getName()), "\n");
					PrintSymbolTree(0, jsymSourceFile);
				}
			);
			
			return vecjsymExportedSymbol;
		}()
	);

	tc::for_each(g_setjsclass, [](auto const& jsclass) noexcept { 
		const_cast<SJsClass&>(jsclass).ResolveBaseClasses();
	});

	// Can call MangleType once construction of the global scope is complete 
	g_bGlobalScopeConstructionComplete = true;

	tc::append(std::cerr, "\n========== GENERATED CODE ==========\n");

	{

		tc::for_each(g_setjsclass, [&](SJsClass const& jsclass) noexcept {
			MergeWithSameCppSignatureInplace(const_cast<SJsClass&>(jsclass).m_vecjsfunctionlikeMethod);
			MergeWithSameCppSignatureInplace(const_cast<SJsClass&>(jsclass).m_vecjsfunctionlikeExportFunction);
		});
		MergeWithSameCppSignatureInplace(scopeGlobal.m_vecjsfunctionlikeExportFunction);

		// Find correct declaration order by going depth-first over class hierarchy
		// Typescript has no requirements on declaration order of type aliases. This is valid typescript:
		// type FooBar2 = FooBar;
		// type FooBar = number | string;
		// In C++, FooBar must be declared before FooBar2.
		// For each type alias declaration, we iterate depth-first over all type aliases referenced on the right-hand side
		// and emit them first.

		// Sort the sequenced indices of g_setjsclass/g_setjstypealias
		SortDeclarationOrder(
			g_setjsclass, 
			[](SetJsClass::iterator itjsclass, auto ForEachChild) noexcept {
				tc::for_each(itjsclass->m_vecpjsclassBase, [&](SJsClass const* pjsclass) noexcept {
					ForEachChild(tc::cont_find<tc::return_element>(
						g_setjsclass.template get<1>(),
						pjsclass->m_strMangledName
					));
				});
			}
		);
		SortDeclarationOrder(
			g_setjstypealias, 
			[&](SetJsTypeAlias::iterator itjstypealias, auto ForEachChild) noexcept {
				ForEachChildTypeNode(itjstypealias->m_jtypenode, [&](ts::TypeNode jtypenode) noexcept {
					if(ts::SyntaxKind::TypeReference==jtypenode->kind()) {
						if(auto const ojsym = (*g_ojtsTypeChecker)->getTypeFromTypeNode(jtypenode)->aliasSymbol()) {
							if(auto const itjstypealias = tc::cont_find<tc::return_element_or_null>(
								g_setjstypealias.template get<1>(),
								FullyQualifiedName(*ojsym)
							)) { // stop dfs when the child class is not part of our source files
								ForEachChild(itjstypealias);
							}
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
			tc::join(tc::transform(g_setjsclass, [](SJsClass const& jsclass) noexcept {
				return tc::concat(
					"	struct _impl", jsclass.m_strMangledName, ";\n",
					"	using ", jsclass.m_strMangledName, " = js_ref<_impl", jsclass.m_strMangledName, ">;\n"
				);
			})),
			tc::join(tc::transform(g_setjstypealias, [&](SJsTypeAlias const& jtypealias) noexcept {
				return tc::explicit_cast<std::string>(
					tc::concat(
						"	using ", jtypealias.m_strMangledName, " = ", jtypealias.MangleType().m_strWithComments,
						";\n"
					)
				);
			})),
			tc::join(tc::transform(g_setjsclass, [](SJsClass const& jsclass) noexcept {
				return tc::explicit_cast<std::string>(tc::concat(
					"	struct _impl", jsclass.m_strMangledName,
					" : ",
					tc_conditional_range(
						tc::empty(jsclass.m_vecpjsclassBase),
						"virtual IObject",
						tc::join_separated(
							tc::transform(jsclass.m_vecpjsclassBase,
								[](SJsClass const* pjsclass) noexcept {
									return tc::concat("virtual _impl", pjsclass->m_strMangledName);
								}
							),
							", "
						)
					),
					tc::join(tc::transform(
						jsclass.m_vecjsymBaseUnknown,
						[](ts::Symbol jsym) noexcept {
							return tc::concat("/* : ", FullyQualifiedName(jsym), " */");
						}
					)),
					" {\n",
					"		struct _tcjs_definitions {\n",
					tc::join(tc::transform(
						jsclass.m_vecvarpExportType,
						[](auto const& varp) noexcept {
							return tc::visit(varp, [](auto const* pjs) noexcept {
								return tc::concat(
									"			using ",
									CppifyName(pjs->m_jsym),
									" = ",
									pjs->m_strMangledName,
									";\n"
								);
							});
						}
					)),
					tc::join(tc::transform(
						jsclass.m_vecjsfunctionlikeExportFunction,
						[](SJsFunctionLike const& jsfunctionlikeFunction) noexcept {
							return tc::concat(
								"			static auto ", jsfunctionlikeFunction.m_strCppifiedName, "(", jsfunctionlikeFunction.CppifiedParametersWithCommentsDecl(), ") noexcept;\n"
							);
						}
					)),
					tc::join(tc::transform(
						jsclass.m_vecjsvariablelikeExportVariable,
						[](SJsVariableLike const& jsvariablelikeVariable) noexcept {
							return tc::concat(
								"			static auto ", jsvariablelikeVariable.m_strCppifiedName, "() noexcept;\n",
								tc_conditional_range(
									jsvariablelikeVariable.m_bReadonly,
									"",
									tc::concat(
										"			static void ", jsvariablelikeVariable.m_strCppifiedName, "(", jsvariablelikeVariable.MangleType().m_strWithComments, " v) noexcept;\n"
									)
								)
							);
						}
					)),
					"		};\n",
					tc::join(tc::transform(
						jsclass.m_vecjsvariablelikeProperty,
						[](SJsVariableLike const& jsvariablelikeProperty) noexcept {
							return tc::concat(
								"		auto ", jsvariablelikeProperty.m_strCppifiedName, "() noexcept;\n",
								tc_conditional_range(
									jsvariablelikeProperty.m_bReadonly,
									"",
									tc::concat(
										"		void ", jsvariablelikeProperty.m_strCppifiedName, "(", jsvariablelikeProperty.MangleType().m_strWithComments, " v) noexcept;\n"
									)
								)
							);
						}
					)),
					tc::join(tc::transform(
						jsclass.m_vecjsfunctionlikeMethod,
						[](SJsFunctionLike const& jsfunctionlikeMethod) noexcept {
							if (ts::SymbolFlags::Method == jsfunctionlikeMethod.m_jsym->getFlags()) {
								return tc::explicit_cast<std::string>(tc::concat(
									"		auto ", jsfunctionlikeMethod.m_strCppifiedName, "(", jsfunctionlikeMethod.CppifiedParametersWithCommentsDecl(), ") noexcept;\n"
								));
							} else if (ts::SymbolFlags::Constructor == jsfunctionlikeMethod.m_jsym->getFlags()) {
								return tc::explicit_cast<std::string>(tc::concat(
									"		static auto _tcjs_construct(", jsfunctionlikeMethod.CppifiedParametersWithCommentsDecl(), ") noexcept;\n"
								));
							} else {
								_ASSERTFALSE;
							}
						}
					)),
					tc_conditional_range(
						jsclass.m_bHasImplicitDefaultConstructor,
						"		static auto _tcjs_construct() noexcept;\n"
					),
					"	};\n"
				));
			})),
			tc::join(tc::transform(g_setjsclass, [](SJsClass const& jsclass) noexcept {
				auto strClassNamespace = tc::explicit_cast<std::string>(tc::concat(
					"_impl", jsclass.m_strMangledName, "::"
				));
				auto strClassInstanceRetrieve = RetrieveSymbolFromCpp(jsclass.m_jsym);
				return tc::explicit_cast<std::string>(tc::concat(
					tc::join(tc::transform(
						jsclass.m_vecjsfunctionlikeExportFunction,
						[&strClassNamespace](SJsFunctionLike const& jsfunctionlikeFunction) noexcept {
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
								RetrieveSymbolFromCpp(jsfunctionlikeFunction.m_jsym), "(", rngchCallArguments, ")"
							);
							return tc::explicit_cast<std::string>(tc::concat(
								"	inline auto ", strClassNamespace, "_tcjs_definitions::",
									jsfunctionlikeFunction.m_strCppifiedName, "(", jsfunctionlikeFunction.CppifiedParametersWithCommentsDef(), ") noexcept {\n",
								tc_conditional_range(
									ts::TypeFlags::Void == jsfunctionlikeFunction.m_jtsSignature->getReturnType()->flags(),
									tc::concat("		", rngchFunctionCall, ";\n"),
									tc::concat(
										"		return ", rngchFunctionCall, ".template as<", MangleType(jsfunctionlikeFunction.m_jtsSignature->getReturnType()).m_strWithComments, ">();\n"
									)
								),
								"	}\n"
							));
						}
					)),
					tc::join(tc::transform(
						jsclass.m_vecjsvariablelikeExportVariable,
						[&strClassNamespace, &strClassInstanceRetrieve](SJsVariableLike const& jsvariablelikeVariable) noexcept {
							return tc::concat(
								"	inline auto ", strClassNamespace, "_tcjs_definitions::", jsvariablelikeVariable.m_strCppifiedName, "() noexcept ",
								"{ return ", strClassInstanceRetrieve, "[\"", jsvariablelikeVariable.m_strJsName, "\"].template as<", jsvariablelikeVariable.MangleType().m_strWithComments, ">(); }\n",
								tc_conditional_range(
									jsvariablelikeVariable.m_bReadonly,
									"",
									tc::concat(
										"	inline void ", strClassNamespace, "_tcjs_definitions::", jsvariablelikeVariable.m_strCppifiedName, "(", jsvariablelikeVariable.MangleType().m_strWithComments, " v) noexcept ",
										"{ ", strClassInstanceRetrieve, ".set(\"", jsvariablelikeVariable.m_strJsName, "\", v); }\n"
									)
								)
							);
						}
					)),
					tc::join(tc::transform(
						jsclass.m_vecjsvariablelikeProperty,
						[&strClassNamespace](SJsVariableLike const& jsvariablelikeProperty) noexcept {
							return tc::concat(
								"	inline auto ", strClassNamespace, jsvariablelikeProperty.m_strCppifiedName, "() noexcept ",
								"{ return _getProperty<", jsvariablelikeProperty.MangleType().m_strWithComments, ">(\"", jsvariablelikeProperty.m_strJsName, "\"); }\n",
								tc_conditional_range(
									jsvariablelikeProperty.m_bReadonly,
									"",
									tc::concat(
										"	inline void ", strClassNamespace, jsvariablelikeProperty.m_strCppifiedName, "(", jsvariablelikeProperty.MangleType().m_strWithComments, " v) noexcept ",
										"{ _setProperty(\"", jsvariablelikeProperty.m_strJsName, "\", v); }\n"
									)
								)
							);
						}
					)),
					tc::join(tc::transform(
						jsclass.m_vecjsfunctionlikeMethod,
						[&jsclass, &strClassNamespace](SJsFunctionLike const& jsfunctionlikeMethod) noexcept {
							auto const rngstrArguments = tc::transform(
								jsfunctionlikeMethod.m_jtsSignature->getParameters(),
								[](ts::Symbol const jsymParameter) noexcept {
									return CppifyName(jsymParameter);
								}
							);
							if (ts::SymbolFlags::Method == jsfunctionlikeMethod.m_jsym->getFlags()) {
								auto const rngchCallArguments = tc::join_separated(
									tc::concat(
										tc::single(tc::concat("\"", tc::explicit_cast<std::string>(jsfunctionlikeMethod.m_jsym->getName()), "\"")),
										rngstrArguments
									),
									", "
								);
								return tc::explicit_cast<std::string>(tc::concat(
									"	inline auto ", strClassNamespace, jsfunctionlikeMethod.m_strCppifiedName, "(", jsfunctionlikeMethod.CppifiedParametersWithCommentsDef(), ") noexcept {\n",
									"		return _call<", MangleType(jsfunctionlikeMethod.m_jtsSignature->getReturnType()).m_strWithComments, ">(", rngchCallArguments, ");\n",
									"	}\n"
								));
							} else if (ts::SymbolFlags::Constructor == jsfunctionlikeMethod.m_jsym->getFlags()) {
								auto const rngchCallArguments = tc::join_separated(rngstrArguments, ", ");
								return tc::explicit_cast<std::string>(tc::concat(
									"	inline auto ", strClassNamespace, "_tcjs_construct(", jsfunctionlikeMethod.CppifiedParametersWithCommentsDef(), ") noexcept {\n",
									"		return ", jsclass.m_strMangledName, "(", RetrieveSymbolFromCpp(jsclass.m_jsym), ".new_(", rngchCallArguments, "));\n",
									"	}\n"
								));
							} else {
								_ASSERTFALSE;
							}
						}
					)),
					tc_conditional_range(
						jsclass.m_bHasImplicitDefaultConstructor,
						tc::concat(
							"	inline auto ", strClassNamespace, "_tcjs_construct() noexcept {\n",
							"		return ", jsclass.m_strMangledName, "(", RetrieveSymbolFromCpp(jsclass.m_jsym), ".new_());\n",
							"	}\n"
						)
					)
				));
			}))
		);

		tc::append(std::cout,
			tc::join(tc::transform(
				scopeGlobal.m_vecjsfunctionlikeExportFunction,
				[](SJsFunctionLike& jsfunctionlikeFunction) {
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
						RetrieveSymbolFromCpp(jsfunctionlikeFunction.m_jsym), "(", rngchCallArguments, ")"
					);
					return tc::explicit_cast<std::string>(tc::concat(
						"	inline auto ",
							jsfunctionlikeFunction.m_strCppifiedName, "(", jsfunctionlikeFunction.CppifiedParametersWithCommentsDef(), ") noexcept {\n",
						tc_conditional_range(
							ts::TypeFlags::Void == jsfunctionlikeFunction.m_jtsSignature->getReturnType()->flags(),
							tc::concat("		", rngchFunctionCall, ";\n"),
							tc::concat(
								"		return ", rngchFunctionCall, ".template as<", MangleType(jsfunctionlikeFunction.m_jtsSignature->getReturnType()).m_strWithComments, ">();\n"
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
						"{ return emscripten::val::global(\"", jsvariablelikeVariable.m_strJsName, "\").template as<", jsvariablelikeVariable.MangleType().m_strWithComments, ">(); }\n",
						tc_conditional_range(
							jsvariablelikeVariable.m_bReadonly,
							"",
							tc::concat(
								"	inline void ", jsvariablelikeVariable.m_strCppifiedName, "(", jsvariablelikeVariable.MangleType().m_strWithComments, " v) noexcept ",
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
				scopeGlobal.m_vecvarpExportType,
				[](auto const& varp) noexcept {
					return tc::visit(varp, [](auto const* pjs) noexcept {
						return tc::explicit_cast<std::string>(tc::concat(
							"	using ", CppifyName(pjs->m_jsym), " = js_defs::", pjs->m_strMangledName, ";\n"
						));
					});
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
