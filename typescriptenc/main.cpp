#include "precompiled.h"
#include "typescript.d.bootstrap.h"

#include "mangle.h"
#include "walk_symbol.h"
#include "jstypes.h"

#include <structopt/app.hpp>

using ts = tc::js::ts;
namespace jst = tc::jst;
namespace js = tc::js;

std::optional<ts::TypeChecker> g_ojtsTypeChecker;
bool g_bGlobalScopeConstructionComplete = true;

namespace {
	std::string RetrieveSymbolFromCpp(ts::Symbol jsymSymbol) noexcept {
		auto const str = FullyQualifiedName(jsymSymbol);
		std::string strResult;

		auto strSplit = tc::as_pointers(str);
		while(!tc::empty(strSplit)) {
			auto const strLocal = [&]() noexcept {
				if(tc::starts_with<tc::return_bool>(strSplit, "\"")) {
					tc::drop_first_inplace(strSplit);

					auto itch = tc::find_first<tc::return_element>(strSplit, '\"');
					auto str = tc::take(strSplit, itch);

					tc::drop_inplace(strSplit, itch + 1);
					if(!tc::empty(strSplit)) {
						_ASSERT('.' == tc::front(strSplit));
						tc::drop_first_inplace(strSplit);
					}

					return str;
				} else if(auto itch = tc::find_first<tc::return_element_or_null>(strSplit, '.')) {
					auto str = tc::take(strSplit, itch);
					tc::drop_inplace(strSplit, itch + 1);
					return str;
				} else {
					auto str = strSplit;
					tc::drop_inplace(strSplit, tc::end(strSplit));
					return str;
				}
			}();

			if(tc::empty(strResult)) {
				tc::append(strResult, "emscripten::val::global(\"", strLocal, "\")");
			} else {
				tc::append(strResult, "[\"", strLocal, "\"]");
			}
		}
		return strResult;
	}

	template<typename SetJsXXX, typename Func>
	std::vector<typename SetJsXXX::value_type const*> SortDeclarationOrder(SetJsXXX& setjs, Func ForEachChild) noexcept {
		std::unordered_set<std::string> setstrSeen;
		std::vector<typename SetJsXXX::value_type const*> vecpjs;

		auto dfs = [&](auto const& js, auto const& dfs) {
			if(!setstrSeen.insert(js.m_strQualifiedName).second) {
				return;
			}
			ForEachChild(js, [&](auto const& jsChild) { dfs(jsChild, dfs); });
			tc::cont_emplace_back(vecpjs, std::addressof(js));
		};

		tc::for_each(setjs, [&](auto const& js) noexcept { dfs(js, dfs); });
		return vecpjs;
	}
} // namespace

SetJsEnum g_setjsenum;
SetJsClass g_setjsclass;
SetJsTypeAlias g_setjstypealias;

std::unordered_set<std::string> g_setstrIgnoredSymbols;
bool IsIgnoredSymbol(std::string const& strFullyQualified) noexcept {
	return tc::cont_find<tc::return_bool>(g_setstrIgnoredSymbols, strFullyQualified);
}

void CompileProgram(ts::Program jProgram) noexcept {
	*g_ojtsTypeChecker = jProgram->getTypeChecker();

	// Iterate over source files, enumerate global symbols and then walk the tree of child symbols
	SJsScope scopeGlobal([&]() noexcept {
		std::vector<ts::Symbol> vecjsymExportedSymbol;
		tc::for_each(
			tc::filter(
				jProgram->getSourceFiles(),
				[](ts::SourceFile jtsSourceFile) noexcept { return jtsSourceFile->isDeclarationFile(); }
			),
			[&](ts::SourceFile jtsSourceFile) noexcept {
				if(auto const josymSourceFile = (*g_ojtsTypeChecker)->getSymbolAtLocation(jtsSourceFile);
				   josymSourceFile && !ts::isExternalModule(jtsSourceFile))
				{
					// Source files defining an internal module like
					// declare module "MyLib" {
					// }
					// should expose the top-level symbol MyLib
					// "External typescript modules" are source files with export statements.
					// By default, the module name is the relative or absolute path to the file.
					// We ignore these top-level modules.
					tc::cont_emplace_back(vecjsymExportedSymbol, *josymSourceFile);
				} else {
					ts::forEachChild(
						jtsSourceFile,
						jst::lambda([&](ts::Node jnodeChild) noexcept -> jst::union_t<double, tc::js::undefined> {
							if(auto const jotsFunctionDeclaration = ts::isFunctionDeclaration(jnodeChild)) {
								tc::cont_emplace_back(
									vecjsymExportedSymbol,
									*(*g_ojtsTypeChecker)->getSymbolAtLocation(*(*jotsFunctionDeclaration)->name())
								);
							} else if(auto const jotsVariableStatement = ts::isVariableStatement(jnodeChild)) {
								tc::for_each(
									(*jotsVariableStatement)->declarationList()->declarations(),
									[&](ts::VariableDeclaration jtsVariableDeclaration) noexcept {
										tc::cont_emplace_back(
											vecjsymExportedSymbol,
											*(*g_ojtsTypeChecker)->getSymbolAtLocation(jtsVariableDeclaration->name())
										);
									}
								);
							} else if(auto const jotsClassDeclaration = ts::isClassDeclaration(jnodeChild)) {
								tc::cont_emplace_back(
									vecjsymExportedSymbol,
									*(*g_ojtsTypeChecker)->getSymbolAtLocation(*(*jotsClassDeclaration)->name())
								);
							} else if(auto const jotsInterfaceDeclaration = ts::isInterfaceDeclaration(jnodeChild)) {
								tc::cont_emplace_back(
									vecjsymExportedSymbol,
									*(*g_ojtsTypeChecker)->getSymbolAtLocation((*jotsInterfaceDeclaration)->name())
								);
							} else if(auto const jotsEnumDeclaration = ts::isEnumDeclaration(jnodeChild)) {
								tc::cont_emplace_back(
									vecjsymExportedSymbol,
									*(*g_ojtsTypeChecker)->getSymbolAtLocation((*jotsEnumDeclaration)->name())
								);
							} else if(auto const jotsModuleDeclaration = ts::isModuleDeclaration(jnodeChild)) {
								tc::cont_emplace_back(
									vecjsymExportedSymbol,
									*(*g_ojtsTypeChecker)->getSymbolAtLocation((*jotsModuleDeclaration)->name())
								);
							} else if(auto const jotsTypeAliasDeclaration = ts::isTypeAliasDeclaration(jnodeChild)) {
								tc::cont_emplace_back(
									vecjsymExportedSymbol,
									*(*g_ojtsTypeChecker)->getSymbolAtLocation((*jotsTypeAliasDeclaration)->name())
								);
							} else if(ts::SyntaxKind::EndOfFileToken == jnodeChild->kind()) {
								// Do nothing
							} else {
								tc::append(
									std::cerr,
									"Unknown source file-level child kind: ",
									tc::as_dec(static_cast<int>(jnodeChild->kind())),
									"\n"
								);
							}
							return tc::js::undefined();
						})
					);
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

		// For more combinations see tests/duplicate_symbols and
		// https://www.typescriptlang.org/docs/handbook/declaration-files/deep-dive.html#advanced-combinations
		tc::sort_unique_inplace(
			vecjsymExportedSymbol,
			tc::projected(tc::fn_less(), [](ts::Symbol jsym) noexcept { return FullyQualifiedName(jsym); })
		);

		return vecjsymExportedSymbol;
	}());

	tc::for_each(g_setjsclass, TC_MEMBER(.ResolveBaseClassesAndSortDependencies()));

	// Can call MangleType once construction of the global scope is complete
	g_bGlobalScopeConstructionComplete = true;

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
		// For each type alias declaration, we iterate depth-first over all type aliases referenced on the right-hand
		// side and emit them first.

		// Sort the sequenced indices of g_setjsclass/g_setjstypealias
		std::vector<SJsClass const*> vecpjsclassSorted =
			SortDeclarationOrder(g_setjsclass, [](SJsClass const& jsclass, auto ForEachChild) noexcept {
				tc::for_each(jsclass.m_vecpjsclassSortDependency, [&](SJsClass const* pjsclass) noexcept {
					ForEachChild(*pjsclass);
				});
			});

		std::vector<SJsTypeAlias const*> vecpjstypealiasSorted =
			SortDeclarationOrder(g_setjstypealias, [&](SJsTypeAlias const& jstypealias, auto ForEachChild) noexcept {
				ForEachChildTypeNode(jstypealias.m_jtypenode, [&](ts::TypeNode jtypenode) noexcept {
					if(ts::SyntaxKind::TypeReference == jtypenode->kind()) {
						auto jtype = (*g_ojtsTypeChecker)->getTypeFromTypeNode(jtypenode);
						if(auto const ojsym = jtype->aliasSymbol()) {
							if(auto const itjstypealias =
								   tc::cont_find<tc::return_element_or_null>(g_setjstypealias, FullyQualifiedName(*ojsym)))
							{ // stop dfs when the child class is not part of our source files
								ForEachChild(*itjstypealias);
							}
						}
						if(auto oatypeargs = ts::TypeReference(jtype)->typeArguments()) {
							tc::for_each(*oatypeargs, [&](auto jtype) noexcept {
								if(auto ojsym = OptSymbolOrAliasSymbol(jtype)) {
									if(auto const itjstypealias = tc::cont_find<tc::return_element_or_null>(
										   g_setjstypealias, FullyQualifiedName(*ojsym)
									   )) { // stop dfs when the child class is not part of our source files
										ForEachChild(*itjstypealias);
									}
								}
							});
						}
					}
					return erecurseCONTINUE;
				});
			});

		auto TemplateDecl = [](auto const& strPrefix, auto const& js) noexcept {
			return [&](auto&& sink) noexcept {
				if(!tc::empty(js.m_vectypeparam)) {
					tc::for_each(tc::concat(strPrefix, "template<"), sink);

					tc::for_each(
						tc::join_separated(
							tc::transform(
								js.m_vectypeparam,
								[](auto const& typeparam) noexcept {
									return tc_conditional_range(
										etypeparamTYPE == typeparam.m_etypeparam || etypeparamKEYOF == typeparam.m_etypeparam,
										tc::concat("typename ", typeparam.m_strName),
										tc::concat(typeparam.Type(), " ", typeparam.m_strName)
									);
								}
							),
							", "
						),
						sink
					);
					tc::for_each(">\n", sink);
				}
			};
		};

		auto TemplateArgs = [](auto const& js) noexcept {
			return tc_conditional_range(
				!tc::empty(js.m_vectypeparam),
				tc::concat("<", tc::join_separated(tc::transform(js.m_vectypeparam, TC_MEMBER(.m_strName)), ", "), ">")
			);
		};

		auto FunctionImpl = [&TemplateDecl](
								auto&& rngstrNamespace, auto const& CallExpression, SJsFunctionLike const& jsfunctionlike
							) noexcept {
			auto ojstypenode = ts::SignatureDeclarationBase(jsfunctionlike.m_jsignature->getDeclaration())->type();
			// TODO: Generate overloads when the last argument of jsfunctionlike is variadic
			// https://github.com/think-cell/typescripten/issues/23 our output variadic template argument list?
			return tc::concat(
				TemplateDecl("\t", jsfunctionlike),
				"\tinline auto ",
				std::forward<decltype(rngstrNamespace)>(rngstrNamespace),
				jsfunctionlike.m_strCppifiedName,
				"(",
				jsfunctionlike.CppifiedParametersWithCommentsDef(),
				") noexcept {\n",
				tc_conditional_range(
					ojstypenode && ts::SyntaxKind::TypePredicate == (*ojstypenode)->kind(),
					// Functions may be type guards https://www.typescriptlang.org/docs/handbook/advanced-types.html
					// The actual callable JS function printed by CallExpression() returns a bool. The typescript
					// compiler performs a cast when the function returns true. Here we do the same in C++.
					tc::concat(
						"\t\tstd::optional<",
						MangleType((*g_ojtsTypeChecker)->getTypeFromTypeNode(*ts::TypePredicateNode(*ojstypenode)->type()))
							.m_strWithComments,
						"> result;\n"
						"\t\tif(",
						CallExpression(),
						") {\n"
						"\t\t\tresult.emplace(",
						tc_conditional_range(
							ts::SyntaxKind::ThisType == ts::Node(ts::TypePredicateNode(*ojstypenode)->parameterName())->kind(),
							"_this<tc::js::any>()",
							tc::concat(
								"tc::js::any(",
								tc::find_first_if<tc::return_element>(
									jsfunctionlike.m_vecjsparam,
									[&](auto const& jsvariable) noexcept {
										return jsvariable.m_strJsName
											   == tc::explicit_cast<std::string>(ts::idText(
												   ts::TypePredicateNode(*ojstypenode)->parameterName().get<ts::Identifier>()
											   ));
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
						tc_conditional_range(ts::TypeFlags::Void != jsfunctionlike.m_jsignature->getReturnType()->flags(), "return"),
						" ",
						CallExpression(),
						";\n"
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
							RetrieveSymbolFromCpp(jsfunctionlike.m_jsym),
							"(",
							tc::join_separated(tc::transform(jsfunctionlike.m_vecjsparam, TC_MEMBER(.m_strCppifiedName)), ", "),
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
				TemplateDecl("\t\t\t", js), "\t\t\tusing ", js.m_strCppifiedName, " = ", js.m_strMangledName, TemplateArgs(js), ";\n"
			);
		};

		auto ClassExportTypeUsingDecl = [](auto const& js) noexcept {
			return tc::concat("\t\t\tusing ", js.m_strCppifiedName, " = ", js.m_strMangledName, ";\n");
		};

		tc::append(
			std::cout,
			"#include <boost/hana/assert.hpp>\n"
			"#include <boost/hana/integral_constant.hpp>\n"
			"#include <boost/hana/map.hpp>\n"
			"#include <boost/hana/pair.hpp>\n"
			"#include <boost/hana/type.hpp>\n"
			"#include <boost/hana/string.hpp>\n"
			"#include <boost/hana/hash.hpp>\n"
			"#include <boost/hana/at_key.hpp>\n"
			"using namespace boost::hana::literals;\n"
			"#pragma push_macro(\"assert\")\n"
			"#undef assert\n"
			"namespace tc::js_defs {\n",
			tc::join(tc::transform(
				g_setjsenum,
				[](SJsEnum const& jsenumEnum) noexcept {
					// We have to mark enums as IsJsIntegralEnum before using in js interop.
					// We have to generate them as enum class even though this is semantically stronger than
					// TypeScript enums actually are. Non-class C++ enum options must be globally unique though
					// and TypeScript enum option names are scoped names like C++ enum class options.
					return tc::concat(
						"\tenum class ",
						jsenumEnum.m_strMangledName,
						" {\n",
						tc::join_separated(
							tc::transform(
								jsenumEnum.m_vecjsenumoption,
								[&jsenumEnum](SJsEnumOption const& jsenumoption) noexcept {
									return tc::concat(
										"\t\t",
										jsenumoption.m_strCppifiedName,
										tc_conditional_range(
											jsenumEnum.m_bIsIntegral
												&& !std::holds_alternative<std::monostate>(jsenumoption.m_vardblstrValue),
											tc::concat(
												" = ", tc::as_dec(tc::explicit_cast<int>(std::get<double>(jsenumoption.m_vardblstrValue)))
											)
										)
									);
								}
							),
							",\n"
						),
						"\n	};\n"
					);
				}
			)),
			"} // namespace tc::js_defs\n"
			"namespace tc::jst {\n",
			tc::join(tc::transform(
				g_setjsenum,
				[](SJsEnum const& jsenumEnum) noexcept {
					// Enums are declared outside of the _jsall class because we have to mark them as IsJsIntegralEnum
					// before using in js interop.
					return tc_conditional_range(
						jsenumEnum.m_bIsIntegral,
						tc::concat(
							"\ttemplate<> struct IsJsIntegralEnum<js_defs::", jsenumEnum.m_strMangledName, "> : std::true_type {};\n"
						),
						tc::concat(
							"template<> struct IsJsHeterogeneousEnum<js_defs::",
							jsenumEnum.m_strMangledName,
							"> : std::true_type {\n"
							"\tstatic inline auto const& Values() {\n"
							"\t\tusing E = js_defs::",
							jsenumEnum.m_strMangledName,
							";\n"
							"\t\tstatic tc::unordered_map<E, js::any> vals{\n",
							tc::join_separated(
								tc::transform(
									tc::filter(
										jsenumEnum.m_vecjsenumoption,
										[](SJsEnumOption const& jsenumoption) noexcept {
											return !std::holds_alternative<std::monostate>(jsenumoption.m_vardblstrValue);
										}
									),
									[](SJsEnumOption const& jsenumoption) noexcept {
										return tc::concat(
											"\t\t\t{E::",
											jsenumoption.m_strCppifiedName,
											", ",
											tc::visit(
												jsenumoption.m_vardblstrValue,
												[](double dblValue) noexcept {
													// TODO: std::to_string because of floating-point numbers. May be
													// not enough precision.
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
				}
			)),
			"} // namespace tc::jst\n"
			"namespace tc::js_defs {\n"
			"\tusing namespace jst; // no ADL\n",
			tc::join(tc::transform(
				vecpjsclassSorted,
				[&](SJsClass const* pjsclass) noexcept {
					return tc::concat(
						TemplateDecl("\t", *pjsclass),
						"\tstruct _impl",
						pjsclass->m_strMangledName,
						";\n",
						TemplateDecl("\t", *pjsclass),
						"\tusing ",
						pjsclass->m_strMangledName,
						" = ref<_impl",
						pjsclass->m_strMangledName,
						TemplateArgs(*pjsclass),
						">;\n"
					);
				}
			)),
			tc::join(tc::transform(
				vecpjstypealiasSorted,
				[&](SJsTypeAlias const* pjtypealias) noexcept {
					return tc::explicit_cast<std::string>(tc::concat(
						TemplateDecl("\t", *pjtypealias),
						"\tusing ",
						pjtypealias->m_strMangledName,
						" = ",
						pjtypealias->MangleType().m_strWithComments,
						";\n"
					));
				}
			)),
			tc::join(tc::transform(
				vecpjsclassSorted,
				[&](SJsClass const* pjsclass) noexcept {
					return tc::make_str<char>(
						TemplateDecl("\t", *pjsclass),
						"\tstruct _impl",
						pjsclass->m_strMangledName,
						" : ",
						tc_conditional_range(
							tc::empty(pjsclass->m_vecmtBaseClass),
							"virtual object_base",
							tc::join_separated(
								tc::transform(
									pjsclass->m_vecmtBaseClass,
									[](auto const& mt) noexcept {
										return tc::concat("virtual _impl", mt.m_strCppCanonized);
									}
								),
								", "
							)
						),
						" {\n",
						"\t\tstruct _tcjs_definitions {\n",
						tc_conditional_range(
							!tc::empty((*g_ojtsTypeChecker)->getPropertiesOfType(pjsclass->m_jtype)),
							tc::concat(
								"\t\t\ttemplate<typename __TYPESCRIPTEN_DUMMY_ARG = tc::js::any>\n"
								"\t\t\tstatic auto constexpr keyof() noexcept { return boost::hana::make_map(\n"
								// We put the keyof map in a templated method to reduce compile times.
								// If the return value depends on the (defaulted) template argument, the
								// compiler does not need to compute the return type unless keyof() is
								// actually called. This reduces compile times by 90%.
								"\t\t\t\tboost::hana::make_pair(\"__typescripten_dummy_key\"_s, "
								"boost::hana::type_c<__TYPESCRIPTEN_DUMMY_ARG>),\n",
								tc::join_separated(
									tc::transform(
										// We enumerate all properties of pjsclass, including properties of derived
										// classes
										// TODO: keyof T also considers member functions as key
										(*g_ojtsTypeChecker)->getPropertiesOfType(pjsclass->m_jtype),
										[&](auto const& jsym) noexcept {
											return tc::concat(
												"\t\t\t\tboost::hana::make_pair(\"",
												tc::explicit_cast<std::string>(jsym->getName()),
												"\"_s, "
												"boost::hana::type_c<",
												[&]() noexcept {
													// This resolves the type of jsym in the context of pjsclass, i.e.,
													// interface B<T> {
													// 	t: T;
													// }
													// interface A extends B<string> {}
													//
													// A has a property t of type "string".
													//
													auto jtype =
														(*g_ojtsTypeChecker)
															->getTypeOfSymbolAtLocation(
																jsym, tc::front(*pjsclass->m_jsym->declarations())
															);
													if(static_cast<bool>(ts::TypeFlags::TypeParameter & jtype->getFlags()))
													{
														STypeParameter typeparam(ts::TypeParameterDeclaration(
															tc::front(*(*jtype->getSymbol())->declarations())
														));
														if(etypeparamNUMBER == typeparam.m_etypeparam) {
															return tc::make_str("number");
														} else if(etypeparamENUM == typeparam.m_etypeparam) {
															jtype = *typeparam.m_ojtype;
														}
													}
													return MangleType(jtype).m_strWithComments;
												}(),
												">)"
											);
										}
									),
									",\n"
								),
								"\n\t\t\t); }\n"
							)
						),
						tc::join(tc::transform(pjsclass->m_vecjsenumExport, ClassExportTypeUsingDecl)),
						tc::join(tc::transform(pjsclass->m_vecjsclassExport, ClassExportTypeParamUsingDecl)),
						tc::join(tc::transform(pjsclass->m_vecjstypealiasExport, ClassExportTypeParamUsingDecl)),
						tc::join(tc::transform(
							pjsclass->m_vecjsfunctionlikeExport,
							[&](SJsFunctionLike const& jsfunctionlike) noexcept {
								return tc::concat(
									TemplateDecl("\t\t\t", jsfunctionlike),
									"\t\t\tstatic auto ",
									jsfunctionlike.m_strCppifiedName,
									"(",
									jsfunctionlike.CppifiedParametersWithCommentsDecl(),
									") noexcept;\n"
								);
							}
						)),
						tc::join(tc::transform(
							pjsclass->m_vecjsvariablelikeExport,
							[&](SJsVariableLike const& jsvariablelikeVariable) noexcept {
								return tc::concat(
									"\t\t\tstatic auto ",
									jsvariablelikeVariable.m_strCppifiedName,
									"() noexcept;\n",
									tc_conditional_range(
										jsvariablelikeVariable.m_bReadonly,
										"",
										tc::concat(
											"\t\t\tstatic void ",
											jsvariablelikeVariable.m_strCppifiedName,
											"(",
											jsvariablelikeVariable.MangleType(pjsclass->m_vectypeparam).m_strWithComments,
											" v) noexcept;\n"
										)
									)
								);
							}
						)),
						"\t\t};\n",
						tc::join(tc::transform(
							pjsclass->m_vecjsvariablelikeProperty,
							[&](SJsVariableLike const& jsvariablelikeProperty) noexcept {
								return tc::concat(
									"\t\tauto ",
									jsvariablelikeProperty.m_strCppifiedName,
									"() noexcept;\n",
									tc_conditional_range(
										jsvariablelikeProperty.m_bReadonly,
										"",
										tc::concat(
											"\t\tvoid ",
											jsvariablelikeProperty.m_strCppifiedName,
											"(",
											jsvariablelikeProperty.MangleType(pjsclass->m_vectypeparam).m_strWithComments,
											" v) noexcept;\n"
										)
									)
								);
							}
						)),
						tc::join(tc::transform(
							pjsclass->m_vecjsfunctionlikeCtor,
							[](SJsFunctionLike const& jsfunctionlike) noexcept {
								return tc::concat(
									"\t\tstatic auto _tcjs_construct(",
									jsfunctionlike.CppifiedParametersWithCommentsDecl(),
									") noexcept;\n"
								);
							}
						)),
						tc_conditional_range(
							pjsclass->m_bHasImplicitDefaultConstructor, "\t\tstatic auto _tcjs_construct() noexcept;\n"
						),
						tc::join(tc::transform(
							pjsclass->m_vecjsfunctionlikeMethod,
							[&](SJsFunctionLike const& jsfunctionlike) noexcept {
								return tc::concat(
									TemplateDecl("\t\t", jsfunctionlike),
									"\t\tauto ",
									jsfunctionlike.m_strCppifiedName,
									"(",
									jsfunctionlike.CppifiedParametersWithCommentsDecl(),
									") noexcept;\n"
								);
							}
						)),
						"\t};\n"
					);
				}
			)),
			tc::join(tc::transform(
				vecpjsclassSorted,
				[&](SJsClass const* pjsclass) noexcept {
					auto const strClassNamespace =
						tc::concat("_impl", pjsclass->m_strMangledName, TemplateArgs(*pjsclass), "::");
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
									"\tinline auto ",
									strClassNamespace,
									"_tcjs_definitions::",
									jsvariablelikeVariable.m_strCppifiedName,
									"() noexcept "
									"{ return ",
									strClassInstanceRetrieve,
									"[\"",
									jsvariablelikeVariable.m_strJsName,
									"\"].template as<",
									jsvariablelikeVariable.MangleType(pjsclass->m_vectypeparam).m_strWithComments,
									">(); }\n",
									tc_conditional_range(
										!jsvariablelikeVariable.m_bReadonly,
										tc::concat(
											TemplateDecl("\t", *pjsclass),
											"\tinline void ",
											strClassNamespace,
											"_tcjs_definitions::",
											jsvariablelikeVariable.m_strCppifiedName,
											"(",
											jsvariablelikeVariable.MangleType(pjsclass->m_vectypeparam).m_strWithComments,
											" v) noexcept "
											"{ ",
											strClassInstanceRetrieve,
											".set(\"",
											jsvariablelikeVariable.m_strJsName,
											"\", v); }\n"
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
									"\tinline auto ",
									strClassNamespace,
									jsvariablelikeProperty.m_strCppifiedName,
									"() noexcept "
									"{ return this->template _getProperty<",
									jsvariablelikeProperty.MangleType(pjsclass->m_vectypeparam).m_strWithComments,
									">(\"",
									jsvariablelikeProperty.m_strJsName,
									"\"); }\n",
									tc_conditional_range(
										!jsvariablelikeProperty.m_bReadonly,
										tc::concat(
											TemplateDecl("\t", *pjsclass),
											"\tinline void ",
											strClassNamespace,
											jsvariablelikeProperty.m_strCppifiedName,
											"(",
											jsvariablelikeProperty.MangleType(pjsclass->m_vectypeparam).m_strWithComments,
											" v) noexcept "
											"{ this->template _setProperty(\"",
											jsvariablelikeProperty.m_strJsName,
											"\", v); }\n"
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
									"\tinline auto ",
									strClassNamespace,
									"_tcjs_construct(",
									jsfunctionlike.CppifiedParametersWithCommentsDef(),
									") noexcept {\n"
									"\t\treturn ",
									pjsclass->m_strMangledName,
									TemplateArgs(*pjsclass),
									"(",
									strClassInstanceRetrieve,
									".new_(",
									tc::join_separated(
										tc::transform(jsfunctionlike.m_vecjsparam, TC_MEMBER(.m_strCppifiedName)), ", "
									),
									"));\n"
									"\t}\n"
								);
							}
						)),
						tc_conditional_range(
							pjsclass->m_bHasImplicitDefaultConstructor,
							tc::concat(
								TemplateDecl("\t", *pjsclass),
								"\tinline auto ",
								strClassNamespace,
								"_tcjs_construct() noexcept {\n"
								"\t\treturn ",
								tc_conditional_range(
									static_cast<bool>(ts::SymbolFlags::Interface & pjsclass->m_jsym->getFlags()),
									"emscripten::val::object()",
									tc::concat(pjsclass->m_strMangledName, TemplateArgs(*pjsclass), "(", strClassInstanceRetrieve, ".new_())")
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
											return tc::concat(
												tc_conditional_range(
													jsfunctionlike.m_bIndexSignature, "this->template _getProperty<", "this->template _call<"
												),
												MangleType(jsfunctionlike.m_jsignature->getReturnType()).m_strWithComments,
												">(",
												tc_conditional_range(
													jsfunctionlike.m_bIndexSignature,
													tc::join_separated(
														tc::transform(jsfunctionlike.m_vecjsparam, TC_MEMBER(.m_strCppifiedName)), ", "
													),
													tc::join_separated(
														tc::concat(
															tc::single(tc::concat(
																"\"",
																tc::explicit_cast<std::string>(jsfunctionlike.m_jsym->getName()),
																"\""
															)),
															tc::transform(jsfunctionlike.m_vecjsparam, TC_MEMBER(.m_strCppifiedName))
														),
														", "
													)
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
				}
			))
		);

		tc::append(
			std::cout,
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
						"\tinline auto ",
						jsvariablelikeVariable.m_strCppifiedName,
						"() noexcept "
						"{ return emscripten::val::global(\"",
						jsvariablelikeVariable.m_strJsName,
						"\").template as<",
						jsvariablelikeVariable.MangleType(tc::make_empty_range<STypeParameter>()).m_strWithComments,
						">(); }\n",
						tc_conditional_range(
							jsvariablelikeVariable.m_bReadonly,
							"",
							tc::concat(
								"\tinline void ",
								jsvariablelikeVariable.m_strCppifiedName,
								"(",
								jsvariablelikeVariable.MangleType(tc::make_empty_range<STypeParameter>()).m_strWithComments,
								" v) noexcept "
								"{ emscripten::val::global().set(\"",
								jsvariablelikeVariable.m_strJsName,
								"\", v); }\n"
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
			return tc::concat("\t\t\tusing ", js.m_strCppifiedName, " = js_defs::", js.m_strMangledName, ";\n");
		};

		tc::append(
			std::cout,
			tc::join(tc::transform(scopeGlobal.m_vecjsenumExport, GlobalExportTypeUsingDecl)),
			tc::join(tc::transform(scopeGlobal.m_vecjsclassExport, GlobalExportTypeParamUsingDecl)),
			tc::join(tc::transform(scopeGlobal.m_vecjstypealiasExport, GlobalExportTypeParamUsingDecl)),
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
			"#pragma pop_macro(\"assert\")\n"
		);
	}
}

struct SCommandLine final {
	std::optional<std::string> tsconfig_file;
	std::optional<std::string> output_file;
	std::optional<std::vector<std::string>> lib; // lib because that is what typescript calls it

	// remaining arguments
	// e.g., ./typescriptenc file1 file2 file3
	std::vector<std::string> files;
};
STRUCTOPT(SCommandLine, tsconfig_file, output_file, lib, files);

int main(int cArgs, char* apszArgs[]) {
#ifdef DEBUG_DEVTOOLS
	// See README.md for an explanation on how to setup interactive debugging with Chrome DevTools
	static struct Callback final {
		ts::CompilerOptions m_jtsCompilerOptions;

		Callback() noexcept : m_jtsCompilerOptions(jst::create_js_object) {
			m_jtsCompilerOptions->strict(true);
			m_jtsCompilerOptions->target(ts::ScriptTarget::ES5);
			m_jtsCompilerOptions->module(ts::ModuleKind::CommonJS);
		}

		TC_JS_MEMBER_FUNCTION(Callback, WithDefaultMap, void, (js::Map<js::string, js::string> mapstrstr)) {
			mapstrstr->set(
				js::string("index.ts"),
				js::string(emscripten::val::global("document")
							   .call<emscripten::val>("getElementById", emscripten::val("text"))["innerHTML"])
			);

			auto const jtsHost = tc::js::tsvfs::createVirtualCompilerHost(
				tc::js::tsvfs::createSystem(mapstrstr), m_jtsCompilerOptions, tc::js::any(emscripten::val::global("ts"))
			);
			auto const jProgram = ts::createProgram(
				ReadonlyArray<string>(create_js_object, tc::single("index.ts")),
				m_jtsCompilerOptions,
				ts::CompilerHost(jtsHost.getEmval()["compilerHost"])
			);
			CompileProgram(jProgram);
		}
	} cb;

	tc::js::tsvfs::createDefaultMapFromCDN(
	)(cb.m_jtsCompilerOptions,
	  ts::version(),
	  true,
	  tc::js::any(emscripten::val::global("ts")),
	  js::undefined(),
	  js::undefined(),
	  js::undefined())
		->then(cb.WithDefaultMap);
#else
	auto app = structopt::app("typescriptenc");
	if(1==cArgs) {
		app.help();
		return EXIT_FAILURE;
	} else {
		try {
			auto options = app.parse<SCommandLine>(cArgs, apszArgs);

			tc::cont_must_emplace(g_setstrIgnoredSymbols, "ClassDecorator");
			tc::cont_must_emplace(g_setstrIgnoredSymbols, "MethodDecorator");
			tc::cont_must_emplace(g_setstrIgnoredSymbols, "IDBValidKey");

			tc::for_each(options.files, [](tc::ptr_range<char> rngch) noexcept {
				tc::for_each(rngch, [](auto& ch) noexcept {
					if('\\' == ch)
						ch = '/'; // typescript createProgram does not support backslashes
				});
			});

			ts::CompilerOptions jCompilerOptions(jst::create_js_object);
			jCompilerOptions->strict(true);
			jCompilerOptions->target(ts::ScriptTarget::ES5);
			jCompilerOptions->module(ts::ModuleKind::CommonJS);
			jCompilerOptions->types(jst::make_Array<js::string>(tc::empty_range()));

			if(options.tsconfig_file) {
				if(auto ojParsedCommandLine = ts::getParsedCommandLineOfConfigFile(
					js::string(*options.tsconfig_file),
					jCompilerOptions,
					ts::ParseConfigFileHost(emscripten::val::module_property("CreateParseConfigHost")())
				))
				{
					jCompilerOptions = (*ojParsedCommandLine)->options();
					tc::for_each((*ojParsedCommandLine)->fileNames(), [&](auto const& jstring) noexcept {
						tc::cont_emplace_back(options.files, tc::explicit_cast<std::string>(jstring));
					});
				} else {
					tc::append(std::cerr, "Loading ", *options.tsconfig_file, " failed.\n");
				}
			} else {
				if(options.lib) {
					jCompilerOptions->lib(jst::make_Array<js::string>(*options.lib));
				} else {
					jCompilerOptions->lib(jst::make_Array<js::string>(tc::single("lib.es2015.d.ts")));
				}
			}

			ts::Program const jProgram = ts::createProgram(jst::make_ReadonlyArray<js::string>(options.files), jCompilerOptions);
			{
				auto const jarrDiagnostics = ts::getPreEmitDiagnostics(jProgram);
				if(!tc::empty(jarrDiagnostics)) {
					tc::append(
						std::cerr,
						tc::explicit_cast<std::string>(ts::formatDiagnosticsWithColorAndContext(
							jarrDiagnostics, ts::FormatDiagnosticsHost(ts::createCompilerHost(jCompilerOptions).getEmval())
						))
					);
					return 1;
				}
			}
			CompileProgram(jProgram);
			return EXIT_SUCCESS;
		} catch(structopt::exception& e) {
			tc::append(std::cerr, e.what(), e.help());
			return EXIT_FAILURE;
		}
	}
#endif
}