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
	std::vector<typename SetJsXXX::value_type const*> SortDeclarationOrder(SetJsXXX& setjs, Func ForEachChild) noexcept {
		std::unordered_set<std::string> setstrSeen;
		std::vector<typename SetJsXXX::value_type const*> vecpjs;

		auto dfs = [&](auto const& js, auto const& dfs) {
			if(!setstrSeen.insert(js.m_strQualifiedName).second) {
				return;
			}
			ForEachChild(js, [&](auto const& jsChild) {
				dfs(jsChild, dfs);
			});
			tc::cont_emplace_back(vecpjs, std::addressof(js));
		};

		tc::for_each(setjs, [&](auto const& js) noexcept {
			dfs(js, dfs);
		});
		return vecpjs;
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
								if (auto const jotsFunctionDeclaration = ts::isFunctionDeclaration(jnodeChild)) {
									tc::cont_emplace_back(vecjsymExportedSymbol, (*g_ojtsTypeChecker)->getSymbolAtLocation(*(*jotsFunctionDeclaration)->name()));
								} else if (auto const jotsVariableStatement = ts::isVariableStatement(jnodeChild)) {
									tc::for_each(tc::js::ts_ext::MakeReadOnlyArray<ts::VariableDeclaration>((*jotsVariableStatement)->declarationList()->declarations()), [&](ts::VariableDeclaration const jtsVariableDeclaration) {
										tc::cont_emplace_back(vecjsymExportedSymbol, (*g_ojtsTypeChecker)->getSymbolAtLocation(jtsVariableDeclaration->name()));
									});
								} else if (auto const jotsClassDeclaration = ts::isClassDeclaration(jnodeChild)) {
									tc::cont_emplace_back(vecjsymExportedSymbol, (*g_ojtsTypeChecker)->getSymbolAtLocation(*(*jotsClassDeclaration)->name()));
								} else if (auto const jotsInterfaceDeclaration = ts::isInterfaceDeclaration(jnodeChild)) {
									tc::cont_emplace_back(vecjsymExportedSymbol, (*g_ojtsTypeChecker)->getSymbolAtLocation((*jotsInterfaceDeclaration)->name()));
								} else if (auto const jotsEnumDeclaration = ts::isEnumDeclaration(jnodeChild)) {
									tc::cont_emplace_back(vecjsymExportedSymbol, (*g_ojtsTypeChecker)->getSymbolAtLocation((*jotsEnumDeclaration)->name()));
								} else if (auto const jotsModuleDeclaration = ts::isModuleDeclaration(jnodeChild)) {
									tc::cont_emplace_back(vecjsymExportedSymbol, (*g_ojtsTypeChecker)->getSymbolAtLocation((*jotsModuleDeclaration)->name()));
								} else if(auto const jotsTypeAliasDeclaration = ts::isTypeAliasDeclaration(jnodeChild)) {
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

	tc::for_each(g_setjsclass, TC_MEMBER(.ResolveBaseClasses()));

	// Can call MangleType once construction of the global scope is complete 
	g_bGlobalScopeConstructionComplete = true;

	tc::append(std::cerr, "\n========== GENERATED CODE ==========\n");

	{

		tc::for_each(g_setjsclass, [&](SJsClass& jsclass) noexcept {
			MergeWithSameCppSignatureInplace(jsclass.m_vecjsfunctionlikeMethod);
			MergeWithSameCppSignatureInplace(jsclass.m_vecjsfunctionlikeExportFunction);
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
		std::vector<SJsClass const*> vecpjsclassSorted = SortDeclarationOrder(
			g_setjsclass, 
			[](SJsClass const& jsclass, auto ForEachChild) noexcept {
				tc::for_each(jsclass.m_vecpjsclassBase, [&](SJsClass const* pjsclass) noexcept {
					ForEachChild(*pjsclass);
				});
			}
		);
		std::vector<SJsTypeAlias const*> vecpjstypealiasSorted = SortDeclarationOrder(
			g_setjstypealias, 
			[&](SJsTypeAlias const& jstypealias, auto ForEachChild) noexcept {
				ForEachChildTypeNode(jstypealias.m_jtypenode, [&](ts::TypeNode jtypenode) noexcept {
					if(ts::SyntaxKind::TypeReference==jtypenode->kind()) {
						if(auto const ojsym = (*g_ojtsTypeChecker)->getTypeFromTypeNode(jtypenode)->aliasSymbol()) {
							if(auto const itjstypealias = tc::cont_find<tc::return_element_or_null>(
								g_setjstypealias,
								FullyQualifiedName(*ojsym)
							)) { // stop dfs when the child class is not part of our source files
								ForEachChild(*itjstypealias);
							}
						}
						return erecurseSKIP;
					} else {
						return erecurseCONTINUE;
					}
				});
			}
		);

		auto FunctionImpl = [](auto&& rngstrNamespace, auto const& CallExpression, SJsFunctionLike const& jsfunctionlike) noexcept {
			auto ojstypenode = ts::SignatureDeclarationBase(jsfunctionlike.m_jsignature->getDeclaration())->type();
			return tc::concat(
				"\tinline auto ", std::forward<decltype(rngstrNamespace)>(rngstrNamespace), jsfunctionlike.m_strCppifiedName, "(", jsfunctionlike.CppifiedParametersWithCommentsDef(), ") noexcept {\n",
				tc_conditional_range(
					ojstypenode && ts::SyntaxKind::TypePredicate==(*ojstypenode)->kind(),
					// Functions may be type guards https://www.typescriptlang.org/docs/handbook/advanced-types.html
					// The actual callable JS function printed by CallExpession() returns a bool. Usually, the typescript compiler uses the function return value to perform a cast for us.
					// Here we do this in C++
					tc::concat(
						"\t\tstd::optional<", MangleType((*g_ojtsTypeChecker)->getTypeFromTypeNode(ts::TypePredicateNode(*ojstypenode)->type())).m_strWithComments ,"> result;\n"
						"\t\tif(", CallExpression(), ") {\n"
							"\t\t\tresult.emplace(", 
								tc_conditional_range(
									ts::SyntaxKind::ThisType==ts::Node(ts::TypePredicateNode(*ojstypenode)->parameterName())->kind(),
									"_this<js_unknown>()",
									tc::concat(
										"js_unknown(",
										tc::find_first_if<tc::return_element>(
											jsfunctionlike.m_vecjsvariablelikeParameters,
											[&](auto const& jsvariable) noexcept {
												return jsvariable.m_strJsName==tc::explicit_cast<std::string>(ts::idText(ts::Identifier(ts::TypePredicateNode(*ojstypenode)->parameterName())));
											}
										)->m_strCppifiedName,
										")"
									)
								),
							");\n"
						"\t\t}\n"
						"\t\treturn result;\n"
					),
					// Normal function: Just insert function call and optional return
					tc::concat(
						"\t\t",
						tc_conditional_range(
							ts::TypeFlags::Void!=jsfunctionlike.m_jsignature->getReturnType()->flags(),
							"return"
						),
						" ", CallExpression(), ";\n"
					)
				),
				"\t}\n"
			);
		};

		auto ExportFunctionImpl = [&](auto&& rngstrNamespace, SJsFunctionLike const& jsfunctionlike) noexcept {
			return FunctionImpl(
					std::forward<decltype(rngstrNamespace)>(rngstrNamespace),
					[&]() noexcept {
						auto FunctionCall = [&]() noexcept {
							return tc::concat(
								RetrieveSymbolFromCpp(jsfunctionlike.m_jsym), "(", 
									tc::join_separated(
										tc::transform(jsfunctionlike.m_vecjsvariablelikeParameters, TC_MEMBER(.m_strCppifiedName)),
										", "
									), 
								")"
							);
						};
						return tc_conditional_range(
							ts::TypeFlags::Void == jsfunctionlike.m_jsignature->getReturnType()->flags(),
							tc::concat(FunctionCall()),
							tc::concat(
								FunctionCall(), ".template as<", MangleType(jsfunctionlike.m_jsignature->getReturnType()).m_strWithComments, ">()"
							)
						);
					},
					jsfunctionlike
			);
		};

		auto ClassExportTypeUsingDecl = [](auto const& js) noexcept {
			return tc::concat(
				"\tusing ",
				js.m_strCppifiedName,
				" = ",
				js.m_strMangledName,
				";\n"
			);
		};

		tc::append(std::cout,
			"namespace tc::js_defs {\n",
			tc::join(tc::transform(g_setjsenum, [](SJsEnum const& jsenumEnum) noexcept {
				// We have to mark enums as IsJsIntegralEnum before using in js interop.
				// We have to generate them as enum class even though this is semantically stronger than 
				// TypeScript enums actually are. Non-class C++ enum options must be globally unique though
				// and TypeScript enum option names are scoped names like C++ enum class options.
				return tc::concat(
					"	enum class ", jsenumEnum.m_strMangledName, " {\n",
					tc::join_separated(
						tc::transform(jsenumEnum.m_vecjsenumoption, [&jsenumEnum](SJsEnumOption const& jsenumoption) noexcept {
							return tc::concat(
								"		", jsenumoption.m_strCppifiedName,
								tc_conditional_range(
									jsenumEnum.m_bIsIntegral && jsenumoption.m_ovardblstrValue,
									tc::concat(" = ", tc::as_dec(tc::explicit_cast<int>(std::get<double>(*jsenumoption.m_ovardblstrValue))))
								)
							);
						}),
						",\n"
					),
					"\n	};\n"
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
						"	template<> struct IsJsIntegralEnum<js_defs::", jsenumEnum.m_strMangledName, "> : std::true_type {};\n"
					),
					tc::concat(
						"template<> struct IsJsHeterogeneousEnum<js_defs::", jsenumEnum.m_strMangledName, "> : std::true_type {\n",
						"	static inline auto const& Values() {\n",
						"		using E = js_defs::", jsenumEnum.m_strMangledName, ";\n",
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
			tc::join(tc::transform(vecpjsclassSorted, [](SJsClass const* pjsclass) noexcept {
				return tc::concat(
					"	struct _impl", pjsclass->m_strMangledName, ";\n",
					"	using ", pjsclass->m_strMangledName, " = js_ref<_impl", pjsclass->m_strMangledName, ">;\n"
				);
			})),
			tc::join(tc::transform(vecpjstypealiasSorted, [&](SJsTypeAlias const* pjtypealias) noexcept {
				return tc::explicit_cast<std::string>(
					tc::concat(
						"	using ", pjtypealias->m_strMangledName, " = ", pjtypealias->MangleType().m_strWithComments,
						";\n"
					)
				);
			})),
			tc::join(tc::transform(vecpjsclassSorted, [&](SJsClass const* pjsclass) noexcept {
				return tc::explicit_cast<std::string>(tc::concat(
					"	struct _impl", pjsclass->m_strMangledName,
					" : ",
					tc_conditional_range(
						tc::empty(pjsclass->m_vecpjsclassBase),
						"virtual IObject",
						tc::join_separated(
							tc::transform(pjsclass->m_vecpjsclassBase,
								[](SJsClass const* pjsclass) noexcept {
									return tc::concat("virtual _impl", pjsclass->m_strMangledName);
								}
							),
							", "
						)
					),
					tc::join(tc::transform(
						pjsclass->m_vecjsymBaseUnknown,
						[](ts::Symbol jsym) noexcept {
							return tc::concat("/* : ", FullyQualifiedName(jsym), " */");
						}
					)),
					" {\n",
					"		struct _tcjs_definitions {\n",
					tc::join(tc::transform(
						pjsclass->m_vecvarjsExportEnum,
						ClassExportTypeUsingDecl
					)),
					tc::join(tc::transform(
						pjsclass->m_vecvarjsExportClass,
						ClassExportTypeUsingDecl
					)),
					tc::join(tc::transform(
						pjsclass->m_vecvarjsExportTypeAlias,
						ClassExportTypeUsingDecl
					)),
					tc::join(tc::transform(
						pjsclass->m_vecjsfunctionlikeExportFunction,
						[](SJsFunctionLike const& jsfunctionlike) noexcept {
							return tc::concat(
								"			static auto ", jsfunctionlike.m_strCppifiedName, "(", jsfunctionlike.CppifiedParametersWithCommentsDecl(), ") noexcept;\n"
							);
						}
					)),
					tc::join(tc::transform(
						pjsclass->m_vecjsvariablelikeExportVariable,
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
						pjsclass->m_vecjsvariablelikeProperty,
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
						pjsclass->m_vecjsfunctionlikeCtor,
						[](SJsFunctionLike const& jsfunctionlike) noexcept {
							return tc::concat(
								"		static auto _tcjs_construct(", jsfunctionlike.CppifiedParametersWithCommentsDecl(), ") noexcept;\n"
							);
						}
					)),
					tc_conditional_range(
						pjsclass->m_bHasImplicitDefaultConstructor,
						"		static auto _tcjs_construct() noexcept;\n"
					),
					tc::join(tc::transform(
						pjsclass->m_vecjsfunctionlikeMethod,
						[](SJsFunctionLike const& jsfunctionlike) noexcept {
							return tc::concat(
								"		auto ", jsfunctionlike.m_strCppifiedName, "(", jsfunctionlike.CppifiedParametersWithCommentsDecl(), ") noexcept;\n"
							);
						}
					)),
					"	};\n"
				));
			})),
			tc::join(tc::transform(vecpjsclassSorted, [&](SJsClass const* pjsclass) noexcept {
				auto strClassNamespace = tc::explicit_cast<std::string>(tc::concat(
					"_impl", pjsclass->m_strMangledName, "::"
				));
				auto strClassInstanceRetrieve = RetrieveSymbolFromCpp(pjsclass->m_jsym);
				return tc::explicit_cast<std::string>(tc::concat(
					tc::join(tc::transform(
						pjsclass->m_vecjsfunctionlikeExportFunction,
						[&](SJsFunctionLike const& jsfunctionlike) noexcept {
							return ExportFunctionImpl(tc::concat(strClassNamespace, "_tcjs_definitions::"), jsfunctionlike);
						}
					)),
					tc::join(tc::transform(
						pjsclass->m_vecjsvariablelikeExportVariable,
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
						pjsclass->m_vecjsvariablelikeProperty,
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
						pjsclass->m_vecjsfunctionlikeCtor,
						[&pjsclass, &strClassNamespace, &strClassInstanceRetrieve](SJsFunctionLike const& jsfunctionlike) noexcept {
							return tc::concat(
								"	inline auto ", strClassNamespace, "_tcjs_construct(", jsfunctionlike.CppifiedParametersWithCommentsDef(), ") noexcept {\n",
								"		return ", pjsclass->m_strMangledName, "(", 
											strClassInstanceRetrieve, ".new_(", 
												tc::join_separated(tc::transform(jsfunctionlike.m_vecjsvariablelikeParameters, TC_MEMBER(.m_strCppifiedName)), ", "), 
										"));\n",
								"	}\n"
							);
						}
					)),
					tc_conditional_range(
						pjsclass->m_bHasImplicitDefaultConstructor,
						tc::concat(
							"\tinline auto ", strClassNamespace, "_tcjs_construct() noexcept {\n"
							"\t\treturn ",
								tc_conditional_range(
									static_cast<bool>(ts::SymbolFlags::Interface & pjsclass->m_jsym->getFlags()),
									"emscripten::val::object()",
									tc::concat(pjsclass->m_strMangledName, "(", strClassInstanceRetrieve, ".new_())")
								),
								";\n"
							"\t}\n"
						)
					),
					tc::join(tc::transform(
						pjsclass->m_vecjsfunctionlikeMethod,
						[&strClassNamespace, &FunctionImpl](SJsFunctionLike const& jsfunctionlike) noexcept {
							return FunctionImpl(
								strClassNamespace,
								[&]() noexcept {
									return tc::concat("_call<", MangleType(jsfunctionlike.m_jsignature->getReturnType()).m_strWithComments, ">(", 
										tc::join_separated(
											tc::concat(
												tc::single(tc::concat("\"", tc::explicit_cast<std::string>(jsfunctionlike.m_jsym->getName()), "\"")),
												tc::transform(jsfunctionlike.m_vecjsvariablelikeParameters, TC_MEMBER(.m_strCppifiedName))
											),
											", "
										), 
										")"
									);
								},
								jsfunctionlike
							);
						}
					))
				));
			}))
		);

		tc::append(std::cout,
			tc::join(tc::transform(
				scopeGlobal.m_vecjsfunctionlikeExportFunction,
				[&](SJsFunctionLike& jsfunctionlike) {
					return ExportFunctionImpl(tc::empty_range(), jsfunctionlike);
				}
			)),
			tc::join(tc::transform(
				scopeGlobal.m_vecjsvariablelikeExportVariable,
				[&](SJsVariableLike const& jsvariablelikeVariable) noexcept {
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

		auto GlobalExportTypeUsingDecl = [](auto const& js) noexcept {
			return tc::concat(
				"\t\t\tusing ",
				js.m_strCppifiedName,
				" = js_defs::",
				js.m_strMangledName,
				";\n"
			);
		};

		tc::append(std::cout,
			tc::join(tc::transform(
				scopeGlobal.m_vecvarjsExportEnum,
				GlobalExportTypeUsingDecl
			)),
			tc::join(tc::transform(
				scopeGlobal.m_vecvarjsExportClass,
				GlobalExportTypeUsingDecl
			)),
			tc::join(tc::transform(
				scopeGlobal.m_vecvarjsExportTypeAlias,
				GlobalExportTypeUsingDecl
			)),
			tc::join(tc::transform(
				scopeGlobal.m_vecjsfunctionlikeExportFunction,
				[](SJsFunctionLike& jsfunctionlike) {
					return tc::concat("	using js_defs::", jsfunctionlike.m_strCppifiedName, ";\n");
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
