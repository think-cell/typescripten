#include "precompiled.h"
#include "typescript.d.bootstrap.h"
#include "mangle.h"
#include "walk_symbol.h"
#include "jstypes.h"

using tc::js::ts;

extern std::optional<tc::js::ts::TypeChecker> g_ojtsTypeChecker;
extern bool g_bGlobalScopeConstructionComplete;

std::string FullyQualifiedName(ts::Symbol jsymType) noexcept {
	return tc::explicit_cast<std::string>((*g_ojtsTypeChecker)->getFullyQualifiedName(jsymType));
}

namespace {
	template<typename Rng>
	SMangledType WrapType(std::string const& strWithComments, std::string const& strCppCanonized, Rng const& atypeArguments) noexcept {
		auto const vecmt = tc::make_vector(tc::transform(atypeArguments, [](ts::Type jtype) noexcept { return MangleType(jtype); }));
		return {
			tc::make_str<char>(strCppCanonized, "<", tc::join_separated(tc::transform(vecmt, TC_MEMBER(.ExpandType())), ", "), ">"),
			tc::make_str<char>(strWithComments, "<", tc::join_separated(tc::transform(vecmt, TC_MEMBER(.m_strWithComments)), ", "), ">")
		};
	}

	SMangledType CommentType(std::string strCppType, tc::js::ts::Type const jtypeRoot) noexcept {
		return {
			strCppType, 
			tc::make_str(
				strCppType,
				" /*",
				tc::explicit_cast<std::string>((*g_ojtsTypeChecker)->typeToString(jtypeRoot)),
				"*/"
			)
		};
	}

	SMangledType MangleClassOrInterface(ts::InterfaceType jinterfacetypeRoot) noexcept {
		auto const strName = FullyQualifiedName(*jinterfacetypeRoot->getSymbol());
		_ASSERT(!jinterfacetypeRoot->outerTypeParameters()); // TODO: Not supported at the moment

		if(auto ojsclass = tc::cont_find<tc::return_element_or_null>(g_setjsclass, strName)) {
			// When jinterfaceRoot refers to interface C declared as  
			// 	interface C<K, T> {}
			// (*jointerfacetypeRoot)->typeParameters() returns the uninstantiated generic type parameters K, T
			// We don't want to output them.
			return {ojsclass->m_strMangledName};
		} else if(IsBootstrapType(strName)) {
			return {tc::make_str("_js_", strName)};
		} else { 
			return {mangling_error, tc::make_str("tc::js::any /*UnknownClassOrInterface=", strName, "*/")};
		}
	}
}

SMangledType MangleType(tc::js::ts::Type jtypeRoot, bool bUseTypeAlias) noexcept {
	_ASSERT(g_bGlobalScopeConstructionComplete);
	
	switch(jtypeRoot->flags()) {
	case ts::TypeFlags::Any:
	case ts::TypeFlags::Unknown:
		return {"tc::js::any"};
	case ts::TypeFlags::String:
		return {"tc::js::string"};
	case ts::TypeFlags::Number:
		return {"double"};
	case ts::TypeFlags::Boolean:
		return {"bool"};
	// Enum?
	// BigInt?
	case ts::TypeFlags::StringLiteral:
		return CommentType("tc::js::string", jtypeRoot);
	case ts::TypeFlags::NumberLiteral:
		return CommentType("double", jtypeRoot);
	case ts::TypeFlags::BooleanLiteral:
		return CommentType("bool", jtypeRoot);

	case ts::TypeFlags::NumberLiteral | ts::TypeFlags::EnumLiteral:
	case ts::TypeFlags::StringLiteral | ts::TypeFlags::EnumLiteral:
	// Some enums e.g. ts.SymbolFlags are parsed as ts::TypeFlags::Union | ts::TypeFlags::EnumLiteral 
	// They are handled below together with all other unions
	{		
		auto jsymParentSymbol = *(*g_ojtsTypeChecker)->getTypeAtLocation(tc::front(*(*jtypeRoot->getSymbol())->declarations())->parent())->getSymbol();
		_ASSERT(ts::SymbolFlags::RegularEnum == jsymParentSymbol->getFlags() ||
			ts::SymbolFlags::ConstEnum == jsymParentSymbol->getFlags());

		auto const strName = FullyQualifiedName(jsymParentSymbol);
		if(auto ojsenum = tc::cont_find<tc::return_element_or_null>(g_setjsenum, strName)) {
			return CommentType(ojsenum->m_strMangledName, jtypeRoot);
		} else {
			return {mangling_error, tc::make_str("tc::js::any /*UnknownMangledEnum=", strName, "*/")};
		}
	}
	// BigIntLiteral
	// ESSymbol
	// UniqueESSymbol
	case ts::TypeFlags::Void:
		return {"void"};
	case ts::TypeFlags::Undefined:
		return {"tc::js::undefined"};
	case ts::TypeFlags::Null:
		return {"tc::js::null"};
	// Never
	case ts::TypeFlags::TypeParameter:
		// TypeScript allows template parameter shadowing, C++ doesn't
		// We currently don't disambiguate the template parameter names so we will
		// produce a compiler error when a method redeclares the same template
		// argument as a class.
		return {tc::explicit_cast<std::string>((*g_ojtsTypeChecker)->typeToString(jtypeRoot))};
	default:
		{
			if(bUseTypeAlias) {
				if(auto const ojsymAlias = jtypeRoot->aliasSymbol()) {
					if(ecpptypeTYPEALIAS & CppType(*ojsymAlias)) {
						auto const strName = FullyQualifiedName(*ojsymAlias);
						if(auto const ojstypealias = tc::cont_find<tc::return_element_or_null>(g_setjstypealias, strName)) {
							if(auto const oatypeargs = jtypeRoot->aliasTypeArguments()) {
								return WrapType(ojstypealias->m_strMangledName, ojstypealias->m_strMangledName, *oatypeargs);
							} else {
								return {ojstypealias->m_strMangledName};
							}
						} else if(IsBootstrapType(strName)) {
							auto strMangled = tc::make_str("_js_", strName);
							if(auto const oatypeargs = jtypeRoot->aliasTypeArguments()) {
								return WrapType(strMangled, strMangled, *oatypeargs);
							} else {
								return {tc_move(strMangled)};
							}
						} else {
							return {mangling_error, tc::make_str("tc::js::any /*UnknownTypeAlias=", strName, "*/")};
						}
					}
				}	
			}
			if (auto const jouniontypeRoot = jtypeRoot->isUnion()) {
				_ASSERT(0!=static_cast<int>(jtypeRoot->getFlags() & ts::TypeFlags::Union));
				_ASSERT(1 < (*jouniontypeRoot)->types()->length());
				auto vecmtType = tc::make_vector(
					tc::transform((*jouniontypeRoot)->types(), 
					[&](ts::Type const jtypeUnionOption) noexcept {
						return MangleType(jtypeUnionOption);
					})
				);
				if(tc::all_of(vecmtType)) {
					// NOTE: sort_unique works with final names which go to C++. It may potentially hide
					// some errors in mangling (e.g. if two different types map to the same type in C++).
					tc::sort_unique_inplace(vecmtType, [&](SMangledType const& a, SMangledType const& b) noexcept {
						return a.m_strCppCanonized < b.m_strCppCanonized;
					});
					_ASSERT(!tc::empty(vecmtType));
					if (1 == tc::size(vecmtType)) {
						return tc::front(vecmtType);
					} else {
						return {
							tc::make_str(
								"tc::jst::union_t<", tc::join_separated(tc::transform(vecmtType, TC_MEMBER(.m_strCppCanonized)), ", "), ">"
							),
							tc::make_str(
								"tc::jst::union_t<", tc::join_separated(tc::transform(vecmtType, TC_MEMBER(.m_strWithComments)), ", "), ">"
							)
						};
					}
				} else {
					return {mangling_error, tc::make_str("tc::js::any /*UnionWithUnsupportedSubtype=[", tc::join_separated(tc::transform(vecmtType, TC_MEMBER(.ExpandType())), ","), "]*/")};
				}
			} else if(ts::TypeFlags::Object==jtypeRoot->getFlags()) {
				ts::ObjectType jobjecttypeRoot(tc_move(jtypeRoot));
				if (static_cast<bool>(ts::ObjectFlags::Reference & jobjecttypeRoot->objectFlags())) {
					// When Collection is a generic class/interface, the return value Collection<K> 
					// is a TypeReference to that generic interface:
					//
					// interface B<K, T> {
					// 	get_collection(k: K) : Collection<K>;
					// }
					//
					// Output with type arguments
					//
					ts::TypeReference jtypereferenceRoot(tc_move(jobjecttypeRoot));	
					auto jtypeTarget = jtypereferenceRoot->target();
					if(0!=static_cast<int>(ts::ObjectFlags::Tuple&jtypeTarget->objectFlags())) {
						return WrapType("std::tuple", "std::tuple", *jtypereferenceRoot->typeArguments());
					} else {
						// In lib.dom.ts, the type of "parent" here is ts::ObjectFlags::Reference|ts::ObjectFlags::Interface
						// but jtypereferenceRoot->target() == jtypereferenceRoot 
						// So we don't call MangleType recursively because that would trigger a stack overflow
						// 
						// interface NodeListOf<TNode> {
						// 	forEach(callbackfn: (value: TNode, key: number, parent: NodeListOf<TNode>) => void, thisArg?: any): void;
						// }

						auto mt = MangleClassOrInterface(*jtypereferenceRoot->target()->isClassOrInterface()); // Can we have a type reference to anything else?
						if(auto ojatypearg = jtypereferenceRoot->typeArguments(); ojatypearg && !tc::empty(*ojatypearg) && mt) {
							return WrapType(mt.m_strWithComments, mt.m_strCppCanonized, *ojatypearg);
						} else {
							return mt;
						}
					}
				} else if(ts::ObjectFlags::Anonymous == jobjecttypeRoot->objectFlags()) {
					auto josymTypeSymbol = jobjecttypeRoot->getSymbol();
					if(josymTypeSymbol && ts::SymbolFlags::TypeLiteral == (*josymTypeSymbol)->getFlags()) {
						auto const vecjsymMember = tc::make_vector(*(*josymTypeSymbol)->members());
						auto const rngstrMemberName = tc::transform(vecjsymMember, [](ts::Symbol const& jsymMember) noexcept {
							return tc::explicit_cast<std::string>(jsymMember->getName());
						});
						if(tc::equal(rngstrMemberName, tc::single("__call"))) {
							ts::Symbol jsymSignature = tc::front(vecjsymMember);
							_ASSERTEQUAL(jsymSignature->getFlags(), ts::SymbolFlags::Signature);
							if(1==(*jsymSignature->declarations())->length()) {
								ts::Signature const jtsSignature = *(*g_ojtsTypeChecker)->getSignatureFromDeclaration(
									ts::CallSignatureDeclaration(tc::front(*jsymSignature->declarations()))
								);
								auto mtReturnType = MangleType(jtsSignature->getReturnType());
								auto const vecmtParameters = tc::make_vector(tc::transform(jtsSignature->getParameters(),
									[&](ts::Symbol const jsymParameter) noexcept -> SMangledType {
										SJsVariableLike jsvariable(jsymParameter);
										return jsvariable.MangleType();
									}
								));

								return {
									tc::make_str(
										"tc::jst::function<", mtReturnType.ExpandType(), "(", tc::join_separated(tc::transform(vecmtParameters, TC_MEMBER(.ExpandType())), ", "), ">"
									),
									tc::make_str(
										"tc::jst::function<", mtReturnType.m_strWithComments, "(", tc::join_separated(tc::transform(vecmtParameters, TC_MEMBER(.m_strWithComments)), ", "), ")>"
									)
								};
							}
						} 
						
						return {
							mangling_error, 
							tc::make_str(
								"tc::js::any /*AnonymousTypeWithLiteralType=", 
									tc::explicit_cast<std::string>((*g_ojtsTypeChecker)->typeToString(jobjecttypeRoot)), 
									"[", tc::join_separated(rngstrMemberName, ", "), "]*/"
							)
						};
					} else {
						return {mangling_error, tc::make_str("tc::js::any /*AnonymousType=", tc::explicit_cast<std::string>((*g_ojtsTypeChecker)->typeToString(jobjecttypeRoot)), "*/")};
					}
				} else if (auto jointerfacetypeRoot = jobjecttypeRoot->isClassOrInterface()) {
					return MangleClassOrInterface(*jointerfacetypeRoot);
				} else {
					return {mangling_error, tc::make_str("tc::js::any /*UnknownObjectType=", tc::explicit_cast<std::string>((*g_ojtsTypeChecker)->typeToString(jobjecttypeRoot)), " objectFlags=", tc::as_dec(static_cast<int>(jobjecttypeRoot->objectFlags())), "*/")};
				}
			} else {
				return {
					mangling_error, 
					tc::make_str(
						"tc::js::any /* unsupported type flags =",
						tc::as_dec(static_cast<int>(jtypeRoot->flags())),
						": ",
						tc::explicit_cast<std::string>((*g_ojtsTypeChecker)->typeToString(jtypeRoot)),
						"*/"
					)
				};
			}
		}
	}
};
