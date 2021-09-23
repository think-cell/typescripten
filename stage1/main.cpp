#include "precompiled.h"
#include "typescript.d.bootstrap.h"
#include "mangle.h"
#include "walk_symbol.h"
#include "jstypes.h"

using tc::jst::create_js_object;
using tc::js::string;
using tc::jst::optional;
using tc::js::any;
using tc::js::console;
using tc::js::ts;
using tc::js::Array;
using tc::js::ReadonlyArray;

std::optional<ts::TypeChecker> g_ojtsTypeChecker;
bool g_bGlobalScopeConstructionComplete = true;

namespace {
	std::string RetrieveSymbolFromCpp(ts::Symbol jsymSymbol) noexcept {
		std::string strSymbolName = tc::explicit_cast<std::string>(jsymSymbol->getName());
		return tc::explicit_cast<std::string>(
			tc_conditional_range(
				!tc::js::ts_ext::Symbol(jsymSymbol)->parent(),
				tc::concat("emscripten::val::global(\"", StripQuotes(strSymbolName), "\")"),
				tc::concat(
					RetrieveSymbolFromCpp(*tc::js::ts_ext::Symbol(jsymSymbol)->parent()),
					"[\"", StripQuotes(strSymbolName), "\"]"
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

int main(int cArgs, char* apszArgs[]) {
	_ASSERT(2 <= cArgs);

	ts::CompilerOptions const jtsCompilerOptions(create_js_object);
	jtsCompilerOptions->strict(true);
	jtsCompilerOptions->target(ts::ScriptTarget::ES5);
	jtsCompilerOptions->module(ts::ModuleKind::CommonJS);

	auto const rngstrFileNames = tc::counted(boost::next(apszArgs), cArgs - 1);
	tc::for_each(rngstrFileNames, [](tc::ptr_range<char> rngch) noexcept {
		tc::for_each(rngch, [](auto& ch) noexcept { 
			if('\\'==ch) ch = '/'; // typescript createProgram does not support backslashes 
		});
	});
	ts::Program const jtsProgram = ts::createProgram(ReadonlyArray<string>(create_js_object, rngstrFileNames), jtsCompilerOptions);
	*g_ojtsTypeChecker = jtsProgram->getTypeChecker();

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
			tc::for_each(
				tc::filter(
					jtsProgram->getSourceFiles(),
					[&](ts::SourceFile jtsSourceFile) noexcept {
						// Only generate output for our input source files, not for their dependencies
						return tc::find_unique<tc::return_bool>(rngstrFileNames, tc::explicit_cast<std::string>(jtsSourceFile->fileName()));
					}
				),
				[&](ts::SourceFile jtsSourceFile) noexcept {
					if(auto const josymSourceFile = (*g_ojtsTypeChecker)->getSymbolAtLocation(jtsSourceFile)) {
						tc::cont_emplace_back(vecjsymExportedSymbol, *josymSourceFile);
					} else {
						tc::append(std::cerr, "Module not found for ", tc::explicit_cast<std::string>(jtsSourceFile->fileName()), ", treating at as a global library\n");
						ts::forEachChild(jtsSourceFile, tc::jst::lambda([&](ts::Node jnodeChild) noexcept -> tc::jst::union_t<double, tc::js::undefined> {
							if (auto const jotsFunctionDeclaration = ts::isFunctionDeclaration(jnodeChild)) {
								tc::cont_emplace_back(vecjsymExportedSymbol, *(*g_ojtsTypeChecker)->getSymbolAtLocation(*(*jotsFunctionDeclaration)->name()));
							} else if (auto const jotsVariableStatement = ts::isVariableStatement(jnodeChild)) {
								tc::for_each(tc::js::ts_ext::MakeReadOnlyArray<ts::VariableDeclaration>((*jotsVariableStatement)->declarationList()->declarations()), [&](ts::VariableDeclaration jtsVariableDeclaration) noexcept {
									tc::cont_emplace_back(vecjsymExportedSymbol, *(*g_ojtsTypeChecker)->getSymbolAtLocation(jtsVariableDeclaration->name()));
								});
							} else if (auto const jotsClassDeclaration = ts::isClassDeclaration(jnodeChild)) {
								tc::cont_emplace_back(vecjsymExportedSymbol, *(*g_ojtsTypeChecker)->getSymbolAtLocation(*(*jotsClassDeclaration)->name()));
							} else if (auto const jotsInterfaceDeclaration = ts::isInterfaceDeclaration(jnodeChild)) {
								tc::cont_emplace_back(vecjsymExportedSymbol, *(*g_ojtsTypeChecker)->getSymbolAtLocation((*jotsInterfaceDeclaration)->name()));
							} else if (auto const jotsEnumDeclaration = ts::isEnumDeclaration(jnodeChild)) {
								tc::cont_emplace_back(vecjsymExportedSymbol, *(*g_ojtsTypeChecker)->getSymbolAtLocation((*jotsEnumDeclaration)->name()));
							} else if (auto const jotsModuleDeclaration = ts::isModuleDeclaration(jnodeChild)) {
								tc::cont_emplace_back(vecjsymExportedSymbol, *(*g_ojtsTypeChecker)->getSymbolAtLocation((*jotsModuleDeclaration)->name()));
							} else if(auto const jotsTypeAliasDeclaration = ts::isTypeAliasDeclaration(jnodeChild)) {
								tc::cont_emplace_back(vecjsymExportedSymbol, *(*g_ojtsTypeChecker)->getSymbolAtLocation((*jotsTypeAliasDeclaration)->name()));
							} else if (ts::SyntaxKind::EndOfFileToken == jnodeChild->kind()) {
								// Do nothing
							} else {
								tc::append(std::cerr, "Unknown source file-level child kind: ", tc::as_dec(static_cast<int>(jnodeChild->kind())), "\n");
							}
							return tc::js::undefined();
						}));
					}
				}
			);

			// Make list of exported symbols unique

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
			tc::sort_unique_inplace(vecjsymExportedSymbol, tc::projected(tc::fn_less(), [](ts::Symbol jsym) noexcept {
				return FullyQualifiedName(jsym);
			}));

			tc::for_each(
				vecjsymExportedSymbol,
				[](ts::Symbol jsymSourceFile) noexcept {
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
			MergeWithSameCppSignatureInplace(jsclass.m_vecjsfunctionlikeExport);
		});
		MergeWithSameCppSignatureInplace(scopeGlobal.m_vecjsfunctionlikeExport);

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
				tc::for_each(jsclass.m_vecpjsclassSortDependency, [&](SJsClass const* pjsclass) noexcept {
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

		auto TemplateDecl = [](auto const& strPrefix, auto const& js) noexcept {
			return tc_conditional_range(
				!tc::empty(js.m_vectypeparam),
				tc::concat(
					strPrefix,
					"template<", 
					tc::join_separated(
						tc::transform(js.m_vectypeparam,
							[](auto const& typeparam) noexcept {
								return tc::concat("typename ", typeparam.m_strName);
							}
						),
						", "
					),
					">\n"
				)
			);
		};
		
		auto TemplateArgs = [](auto const& js) noexcept {
			return tc_conditional_range(
				!tc::empty(js.m_vectypeparam),
				tc::concat("<", tc::join_separated(tc::transform(js.m_vectypeparam, TC_MEMBER(.m_strName)), ", "),">")
			);
		};

		auto FunctionImpl = [&TemplateDecl](auto&& rngstrNamespace, auto const& CallExpression, SJsFunctionLike const& jsfunctionlike) noexcept {
			auto ojstypenode = ts::SignatureDeclarationBase(jsfunctionlike.m_jsignature->getDeclaration())->type();
			return tc::concat(
				TemplateDecl("\t", jsfunctionlike),
				"\tinline auto ", std::forward<decltype(rngstrNamespace)>(rngstrNamespace), jsfunctionlike.m_strCppifiedName, "(", jsfunctionlike.CppifiedParametersWithCommentsDef(), ") noexcept {\n",
				tc_conditional_range(
					ojstypenode && ts::SyntaxKind::TypePredicate==(*ojstypenode)->kind(),
					// Functions may be type guards https://www.typescriptlang.org/docs/handbook/advanced-types.html
					// The actual callable JS function printed by CallExpression() returns a bool. The typescript compiler performs a cast when the function returns true.
					// Here we do the same in C++.
					tc::concat(
						"\t\tstd::optional<", MangleType((*g_ojtsTypeChecker)->getTypeFromTypeNode(ts::TypePredicateNode(*ojstypenode)->type())).m_strWithComments ,"> result;\n"
						"\t\tif(", CallExpression(), ") {\n"
							"\t\t\tresult.emplace(", 
								tc_conditional_range(
									ts::SyntaxKind::ThisType==ts::Node(ts::TypePredicateNode(*ojstypenode)->parameterName())->kind(),
									"_this<tc::js::any>()",
									tc::concat(
										"tc::js::any(",
										tc::find_first_if<tc::return_element>(
											jsfunctionlike.m_vecjsvariablelikeParameters,
											[&](auto const& jsvariable) noexcept {
												return jsvariable.m_strJsName==tc::explicit_cast<std::string>(ts::idText(ts::TypePredicateNode(*ojstypenode)->parameterName().get<ts::Identifier>()));
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

		auto ClassExportTypeParamUsingDecl = [&](auto const& js) noexcept {
			return tc::concat(
				TemplateDecl("\t", js),
				"\tusing ",
				js.m_strCppifiedName,
				" = ",
				js.m_strMangledName,
				TemplateArgs(js),
				";\n"
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
					"\tenum class ", jsenumEnum.m_strMangledName, " {\n",
					tc::join_separated(
						tc::transform(jsenumEnum.m_vecjsenumoption, [&jsenumEnum](SJsEnumOption const& jsenumoption) noexcept {
							return tc::concat(
								"\t\t", jsenumoption.m_strCppifiedName,
								tc_conditional_range(
									jsenumEnum.m_bIsIntegral && !std::holds_alternative<std::monostate>(jsenumoption.m_vardblstrValue),
									tc::concat(" = ", tc::as_dec(tc::explicit_cast<int>(std::get<double>(jsenumoption.m_vardblstrValue))))
								)
							);
						}),
						",\n"
					),
					"\n	};\n"
				);
			})),
			"} // namespace tc::js_defs\n"
			"namespace tc::jst {\n",
			tc::join(tc::transform(g_setjsenum, [](SJsEnum const& jsenumEnum) noexcept {
				// Enums are declared outside of the _jsall class because we have to mark them as IsJsIntegralEnum
				// before using in js interop.
				return tc_conditional_range(
					jsenumEnum.m_bIsIntegral,
					tc::concat(
						"\ttemplate<> struct IsJsIntegralEnum<js_defs::", jsenumEnum.m_strMangledName, "> : std::true_type {};\n"
					),
					tc::concat(
						"template<> struct IsJsHeterogeneousEnum<js_defs::", jsenumEnum.m_strMangledName, "> : std::true_type {\n"
						"\tstatic inline auto const& Values() {\n"
						"\t\tusing E = js_defs::", jsenumEnum.m_strMangledName, ";\n"
						"\t\tstatic tc::unordered_map<E, js::any> vals{\n",
						tc::join_separated(
							tc::transform(
								tc::filter(
									jsenumEnum.m_vecjsenumoption, 
									[](SJsEnumOption const& jsenumoption) noexcept { return !std::holds_alternative<std::monostate>(jsenumoption.m_vardblstrValue); }
								),
								[](SJsEnumOption const& jsenumoption) noexcept {
									return tc::concat(
										"\t\t\t{E::", jsenumoption.m_strCppifiedName, ", ",
										tc::visit(jsenumoption.m_vardblstrValue,
											[](double dblValue) noexcept {
												// TODO: std::to_string because of floating-point numbers. May be not enough precision.
												return tc::make_str("tc::js::any(", std::to_string(dblValue), ")");
											},
											[](std::string const& strValue) noexcept {
												return tc::make_str("tc::js::string(\"", strValue, "\")");
											},
											[](std::monostate const&) noexcept {
												_ASSERTFALSE;
												return tc::make_str("");
											}
										),
										"}"
									);
								}
							),
							",\n"
						),
						"\n"
						"\t\t};\n"
						"\t\treturn vals;\n"
						"\t}\n"
						"};\n"
					)
				);
			})),
			"} // namespace tc::jst\n"
			"namespace tc::js_defs {\n"
			"\tusing namespace jst; // no ADL\n",
			tc::join(tc::transform(vecpjsclassSorted, [&](SJsClass const* pjsclass) noexcept {
				return tc::concat(
					TemplateDecl("\t", *pjsclass),
					"\tstruct _impl", pjsclass->m_strMangledName, ";\n",
					TemplateDecl("\t", *pjsclass),
					"\tusing ", 
					pjsclass->m_strMangledName, 
					" = ref<_impl", pjsclass->m_strMangledName, 
					TemplateArgs(*pjsclass),
					">;\n"
				);
			})),
			tc::join(tc::transform(vecpjstypealiasSorted, [&](SJsTypeAlias const* pjtypealias) noexcept {
				return tc::explicit_cast<std::string>(
					tc::concat(
						TemplateDecl("\t", *pjtypealias),
						"\tusing ", pjtypealias->m_strMangledName, " = ", pjtypealias->MangleType().m_strWithComments,
						";\n"
					)
				);
			})),
			tc::join(tc::transform(vecpjsclassSorted, [&](SJsClass const* pjsclass) noexcept {
				return tc::make_str(
					TemplateDecl("\t", *pjsclass),
					"\tstruct _impl", pjsclass->m_strMangledName,
					" : ",
					tc_conditional_range(
						tc::empty(pjsclass->m_vecjtypeBaseClass),
						"virtual object_base",
						tc::join_separated(
							tc::transform(pjsclass->m_vecjtypeBaseClass,
								[](auto jtype) noexcept {
									// FIXME: MangleType should return optional
									// FIXME: Prefixing _impl here is pretty ugly
									return tc::concat("virtual _impl", MangleType(jtype).m_strCppCanonized);
								}
							),
							", "
						)
					),
					" {\n",
					"\t\tstruct _tcjs_definitions {\n",
					tc::join(tc::transform(
						pjsclass->m_vecjsenumExport,
						ClassExportTypeUsingDecl
					)),
					tc::join(tc::transform(
						pjsclass->m_vecjsclassExport,
						ClassExportTypeParamUsingDecl
					)),
					tc::join(tc::transform(
						pjsclass->m_vecjstypealiasExport,
						ClassExportTypeParamUsingDecl
					)),
					tc::join(tc::transform(
						pjsclass->m_vecjsfunctionlikeExport,
						[&](SJsFunctionLike const& jsfunctionlike) noexcept {
							return tc::concat(
								TemplateDecl("\t\t\t", jsfunctionlike),
								"\t\t\tstatic auto ", jsfunctionlike.m_strCppifiedName, "(", jsfunctionlike.CppifiedParametersWithCommentsDecl(), ") noexcept;\n"
							);
						}
					)),
					tc::join(tc::transform(
						pjsclass->m_vecjsvariablelikeExport,
						[](SJsVariableLike const& jsvariablelikeVariable) noexcept {
							return tc::concat(
								"\t\t\tstatic auto ", jsvariablelikeVariable.m_strCppifiedName, "() noexcept;\n",
								tc_conditional_range(
									jsvariablelikeVariable.m_bReadonly,
									"",
									tc::concat(
										"\t\t\tstatic void ", jsvariablelikeVariable.m_strCppifiedName, "(", jsvariablelikeVariable.MangleType().m_strWithComments, " v) noexcept;\n"
									)
								)
							);
						}
					)),
					"\t\t};\n",
					tc::join(tc::transform(
						pjsclass->m_vecjsvariablelikeProperty,
						[](SJsVariableLike const& jsvariablelikeProperty) noexcept {
							return tc::concat(
								"\t\tauto ", jsvariablelikeProperty.m_strCppifiedName, "() noexcept;\n",
								tc_conditional_range(
									jsvariablelikeProperty.m_bReadonly,
									"",
									tc::concat(
										"\t\tvoid ", jsvariablelikeProperty.m_strCppifiedName, "(", jsvariablelikeProperty.MangleType().m_strWithComments, " v) noexcept;\n"
									)
								)
							);
						}
					)),
					tc::join(tc::transform(
						pjsclass->m_vecjsfunctionlikeCtor,
						[](SJsFunctionLike const& jsfunctionlike) noexcept {
							return tc::concat(
								"\t\tstatic auto _tcjs_construct(", jsfunctionlike.CppifiedParametersWithCommentsDecl(), ") noexcept;\n"
							);
						}
					)),
					tc_conditional_range(
						pjsclass->m_bHasImplicitDefaultConstructor,
						"\t\tstatic auto _tcjs_construct() noexcept;\n"
					),
					tc::join(tc::transform(
						pjsclass->m_vecjsfunctionlikeMethod,
						[&](SJsFunctionLike const& jsfunctionlike) noexcept {
							return tc::concat(
								TemplateDecl("\t\t", jsfunctionlike),
								"\t\tauto ", jsfunctionlike.m_strCppifiedName, "(", jsfunctionlike.CppifiedParametersWithCommentsDecl(), ") noexcept;\n"
							);
						}
					)),
					"\t};\n"
				);
			})),
			tc::join(tc::transform(vecpjsclassSorted, [&](SJsClass const* pjsclass) noexcept {
				auto const strClassNamespace = tc::concat("_impl", pjsclass->m_strMangledName, TemplateArgs(*pjsclass), "::");
				auto const strClassInstanceRetrieve = RetrieveSymbolFromCpp(pjsclass->m_jsym);

				return tc::make_str<char>(
					tc::join(tc::transform(
						pjsclass->m_vecjsfunctionlikeExport,
						[&](SJsFunctionLike const& jsfunctionlike) noexcept {
							return tc::concat(
								TemplateDecl("\t", *pjsclass),
								ExportFunctionImpl(tc::concat(strClassNamespace, "_tcjs_definitions::"), jsfunctionlike)
							);
						}
					)),
					tc::join(tc::transform(
						pjsclass->m_vecjsvariablelikeExport,
						[&](SJsVariableLike const& jsvariablelikeVariable) noexcept {
							return tc::concat(
								TemplateDecl("\t", *pjsclass),
								"\tinline auto ", strClassNamespace, "_tcjs_definitions::", jsvariablelikeVariable.m_strCppifiedName, "() noexcept "
								"{ return ", strClassInstanceRetrieve, "[\"", jsvariablelikeVariable.m_strJsName, "\"].template as<", jsvariablelikeVariable.MangleType().m_strWithComments, ">(); }\n",
								tc_conditional_range(
									!jsvariablelikeVariable.m_bReadonly,
									tc::concat(
										TemplateDecl("\t", *pjsclass),
										"\tinline void ", strClassNamespace, "_tcjs_definitions::", jsvariablelikeVariable.m_strCppifiedName, "(", jsvariablelikeVariable.MangleType().m_strWithComments, " v) noexcept "
										"{ ", strClassInstanceRetrieve, ".set(\"", jsvariablelikeVariable.m_strJsName, "\", v); }\n"
									)
								)
							);
						}
					)),
					tc::join(tc::transform(
						pjsclass->m_vecjsvariablelikeProperty,
						[&](SJsVariableLike const& jsvariablelikeProperty) noexcept {
							return tc::concat(
								TemplateDecl("\t", *pjsclass),
								"\tinline auto ", strClassNamespace, jsvariablelikeProperty.m_strCppifiedName, "() noexcept "
								"{ return this->template _getProperty<", jsvariablelikeProperty.MangleType().m_strWithComments, ">(\"", jsvariablelikeProperty.m_strJsName, "\"); }\n",
								tc_conditional_range(
									!jsvariablelikeProperty.m_bReadonly,
									tc::concat(
										TemplateDecl("\t", *pjsclass),
										"\tinline void ", strClassNamespace, jsvariablelikeProperty.m_strCppifiedName, "(", jsvariablelikeProperty.MangleType().m_strWithComments, " v) noexcept "
										"{ this->template _setProperty(\"", jsvariablelikeProperty.m_strJsName, "\", v); }\n"
									)
								)
							);
						}
					)),
					tc::join(tc::transform(
						pjsclass->m_vecjsfunctionlikeCtor,
						[&](SJsFunctionLike const& jsfunctionlike) noexcept {
							return tc::concat(
								TemplateDecl("\t", *pjsclass),
								"\tinline auto ", strClassNamespace, "_tcjs_construct(", jsfunctionlike.CppifiedParametersWithCommentsDef(), ") noexcept {\n"
									"\t\treturn ", pjsclass->m_strMangledName, "(", 
										strClassInstanceRetrieve, ".new_(", 
											tc::join_separated(tc::transform(jsfunctionlike.m_vecjsvariablelikeParameters, TC_MEMBER(.m_strCppifiedName)), ", "), 
										"));\n"
								"\t}\n"
							);
						}
					)),
					tc_conditional_range(
						pjsclass->m_bHasImplicitDefaultConstructor,
						tc::concat(
							TemplateDecl("\t", *pjsclass),
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
						[&](SJsFunctionLike const& jsfunctionlike) noexcept {
							return tc::concat(
								TemplateDecl("\t", *pjsclass),
								FunctionImpl(
									strClassNamespace,
									[&]() noexcept {
										return tc::concat("this->template _call<", MangleType(jsfunctionlike.m_jsignature->getReturnType()).m_strWithComments, ">(", 
											tc::join_separated(
												tc::concat(
													tc::single(tc::concat("\"", tc::explicit_cast<std::string>(jsfunctionlike.m_jsym->getName()), "\"")), // FIXME?
													tc::transform(jsfunctionlike.m_vecjsvariablelikeParameters, TC_MEMBER(.m_strCppifiedName))
												),
												", "
											), 
											")"
										);
									},
									jsfunctionlike
								)
							);
						}
					))
				);
			}))
		);

		tc::append(std::cout,
			tc::join(tc::transform(
				scopeGlobal.m_vecjsfunctionlikeExport,
				[&](SJsFunctionLike const& jsfunctionlike) noexcept {
					return ExportFunctionImpl(tc::empty_range(), jsfunctionlike);
				}
			)),
			tc::join(tc::transform(
				scopeGlobal.m_vecjsvariablelikeExport,
				[&](SJsVariableLike const& jsvariablelikeVariable) noexcept {
					return tc::concat(
						"\tinline auto ", jsvariablelikeVariable.m_strCppifiedName, "() noexcept "
						"{ return emscripten::val::global(\"", jsvariablelikeVariable.m_strJsName, "\").template as<", jsvariablelikeVariable.MangleType().m_strWithComments, ">(); }\n",
						tc_conditional_range(
							jsvariablelikeVariable.m_bReadonly,
							"",
							tc::concat(
								"\tinline void ", jsvariablelikeVariable.m_strCppifiedName, "(", jsvariablelikeVariable.MangleType().m_strWithComments, " v) noexcept "
								"{ emscripten::val::global().set(\"", jsvariablelikeVariable.m_strJsName, "\", v); }\n"
							)
						)
					);
				}
			)),
			"}; // namespace tc::js_defs\n"
			"namespace tc::js {\n"
		);

		auto GlobalExportTypeParamUsingDecl = [&](auto const& js) noexcept {
			return tc::concat(
				TemplateDecl("\t\t\t", js),
				"\t\t\tusing ",
				js.m_strCppifiedName,
				" = js_defs::",
				js.m_strMangledName,
				TemplateArgs(js),
				";\n"
			);
		};

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
				scopeGlobal.m_vecjsenumExport,
				GlobalExportTypeUsingDecl
			)),
			tc::join(tc::transform(
				scopeGlobal.m_vecjsclassExport,
				GlobalExportTypeParamUsingDecl
			)),
			tc::join(tc::transform(
				scopeGlobal.m_vecjstypealiasExport,
				GlobalExportTypeParamUsingDecl
			)),
			tc::join(tc::transform(
				scopeGlobal.m_vecjsfunctionlikeExport,
				[](SJsFunctionLike const& jsfunctionlike) noexcept {
					return tc::concat("\tusing js_defs::", jsfunctionlike.m_strCppifiedName, ";\n");
				}
			)),
			tc::join(tc::transform(
				scopeGlobal.m_vecjsvariablelikeExport,
				[](SJsVariableLike const& jsvariablelikeVariable) noexcept {
					return tc::concat("\tusing js_defs::", jsvariablelikeVariable.m_strCppifiedName, ";\n");
				}
			)),
			"} // namespace tc::js\n"
		);
	}
	return 0;
}
