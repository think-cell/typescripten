#include "precompiled.h"
#include "typescript.d.bootstrap.h"
#include "mangle.h"
#include "walk_symbol.h"
#include "jstypes.h"
#include "mangle.inl"

extern std::optional<tc::js::ts::TypeChecker> g_ojtsTypeChecker;
extern bool g_bGlobalScopeConstructionComplete;

std::string FullyQualifiedName(ts::Symbol jsymType) noexcept {
	return tc::explicit_cast<std::string>((*g_ojtsTypeChecker)->getFullyQualifiedName(jsymType));
}

tc::jst::optional<tc::js::ts::Symbol> OptSymbolOrAliasSymbol(ts::Type jtype) noexcept {
    if(auto ojsym = jtype->getSymbol()) { // getSymbol is correctly marked as returning Symbol|undefined
        return ojsym;
    } else {
        return jtype->aliasSymbol();
    }
}

tc::js::ts::Symbol SymbolOrAliasSymbol(ts::Type jtype) noexcept {
    if(auto ojsym = OptSymbolOrAliasSymbol(jtype)) {
        return *ojsym;
    } else {
        _ASSERTFALSE;
    }
}

SMangledType::SMangledType(std::string strCppCanonized) noexcept
	: m_strCppCanonized(strCppCanonized)
	, m_strWithComments(tc_move(strCppCanonized))
{}

SMangledType::SMangledType(std::string strCppCanonized, std::string strWithComments) noexcept
	: m_strCppCanonized(tc_move(strCppCanonized))
	, m_strWithComments(tc_move(strWithComments))
{}

SMangledType::SMangledType(mangling_error_t, std::string strWithComments) noexcept
	: m_strWithComments(std::move(strWithComments))
{}

std::string SMangledType::ExpandType() const& noexcept {
	return tc::empty(m_strCppCanonized)
		? tc::make_str("tc::js::any")
		: m_strCppCanonized;
}

SMangledType::operator bool() const& noexcept {
	return !tc::empty(m_strCppCanonized);
}

bool operator<(SMangledType const& lhs, SMangledType const& rhs) noexcept {
	return lhs.m_strCppCanonized < rhs.m_strCppCanonized;
}

namespace {
	SMangledType MangleClassOrInterface(ts::InterfaceType jinterfacetypeRoot, jst::optional<js::ReadonlyArray<ts::Type>> oajtypeArgs = js::undefined()) noexcept {	
		auto const strName = FullyQualifiedName(*jinterfacetypeRoot->getSymbol());
		_ASSERT(!jinterfacetypeRoot->outerTypeParameters()); // TODO: Not supported at the moment

		if(oajtypeArgs && !tc::empty(*oajtypeArgs)) {
			return ::MangleClassOrInterface(strName, *oajtypeArgs);
		} else {
			return ::MangleClassOrInterface(strName, tc::make_empty_range<ts::Type>());
		}
	}
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

std::tuple<SMangledType, SJsEnum const*> LookupEnumFromLiteral(ts::Type jtype) noexcept {
	auto jsym = SymbolOrAliasSymbol(jtype);
	auto jsymParentSymbol = SymbolOrAliasSymbol((*g_ojtsTypeChecker)->getTypeAtLocation(tc::front(*jsym->declarations())->parent()));
	_ASSERT(ts::SymbolFlags::RegularEnum == jsymParentSymbol->getFlags() 
		|| ts::SymbolFlags::ConstEnum == jsymParentSymbol->getFlags()
	);

	if(auto ojsenum = tc::cont_find<tc::return_element_or_null>(g_setjsenum, FullyQualifiedName(jsymParentSymbol))) {
		return std::make_tuple(SMangledType(ojsenum->m_strMangledName), std::addressof(*ojsenum));
	} else {
		return std::make_tuple(SMangledType(mangling_error, tc::make_str("tc::js::any /*UnknownMangledEnum=", FullyQualifiedName(jsym), "*/")), nullptr);
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
		return std::get<0>(LookupEnumFromLiteral(jtypeRoot));
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
								return MangleTypeArguments(ojstypealias->m_strMangledName, ojstypealias->m_strMangledName, *oatypeargs, tc::transform(ojstypealias->m_vectypeparam, TC_MEMBER(.m_etypeparam)));
							} else {
								return {ojstypealias->m_strMangledName};
							}
						} else if(IsBootstrapType(strName)) {
							auto strMangled = tc::make_str("_js_", strName);
							if(auto const oatypeargs = jtypeRoot->aliasTypeArguments()) {
								return MangleTypeArguments(strMangled, strMangled, *oatypeargs, tc::repeat_n(tc::size(*oatypeargs), etypeparamTYPE));
							} else {
								return {tc_move(strMangled)};
							}
						} else {
							return {mangling_error, tc::make_str("tc::js::any /*UnknownTypeAlias=", strName, "*/")};
						}
					} else if(ecpptypeENUM & CppType(*ojsymAlias)) {
						auto const strName = FullyQualifiedName(*ojsymAlias);
						if(auto const ojsenum = tc::cont_find<tc::return_element_or_null>(g_setjsenum, strName)) {
							return {ojsenum->m_strMangledName};
						} else {
							return {mangling_error, tc::make_str("tc::js::any /*UnknownEnum=", strName, "*/")};
						}
					}
				}	
			}
			if (auto const jouniontypeRoot = jtypeRoot->isUnion()) {
				_ASSERT(0!=static_cast<int>(jtypeRoot->getFlags() & ts::TypeFlags::Union));
				
				auto const cUnionTypes = (*jouniontypeRoot)->types()->length();
				_ASSERT(1 < cUnionTypes);

				if((ts::TypeFlags::EnumLiteral | ts::TypeFlags::Union)==jtypeRoot->getFlags()) {
					// Enum as TypeParameter constraint: 
					//
					// enum E {a, b}
					// interface T<K extends E> {}
					// The E in "extends E" is EnumLiteral|Union
					//
					// Whereas in type S = T<E.a|E.b> 
					// E.a|E.b is simply a union (which is handled below)
					// with a two element types array of NumberLiteral|EnumLiteral
					//
					auto jsym = SymbolOrAliasSymbol(jtypeRoot);
					// _ASSERT(static_cast<bool>(ts::SymbolFlags::None!=(ts::SymbolFlags::RegularEnum&jsym->getFlags())));
					
					auto const strName = FullyQualifiedName(jsym);
					if(auto ojsenum = tc::cont_find<tc::return_element_or_null>(g_setjsenum, strName)) {
						return {ojsenum->m_strMangledName};
					} else {
						return {mangling_error, tc::make_str("tc::js::any /*UnknownMangledEnum=", strName, "*/")};
					}
				} else {
					int cExpectedUnionTypes = 0;
					std::unordered_set<SJsEnum const*> setpjsenum; 

					auto vecmtType = tc::make_vector(
						tc::transform((*jouniontypeRoot)->types(), 
						[&](ts::Type const jtypeUnionOption) noexcept -> SMangledType {
							// TypeScript represents a reference to enum types as a union of the possible values.
							// Thus 
							// 	export type OtherUnion = undefined | SyntaxKind;
							// is a union of undefined and *all* possible values of SyntaxKind. 
							//
							// We can represent that as union_t<undefined, SyntaxKind>
							//
							// 	export type OtherUnion = undefined | SyntaxKind.Unknown | SyntaxKind.Comment;
							//
							// is therefore a union of undefined and two values of SyntaxKind.
							// We cannot represent that and it becomes union_t<undefined, SyntaxKind> as well. 
							//
							// We issue a warning below when our C++ union has less member types than we expect.
							
							if((ts::TypeFlags::NumberLiteral | ts::TypeFlags::EnumLiteral)==jtypeUnionOption->getFlags()) {
								auto tupmtojsenum = LookupEnumFromLiteral(jtypeUnionOption);
								if(std::get<1>(tupmtojsenum)
								&& tc::cont_try_emplace(setpjsenum, std::get<1>(tupmtojsenum)).second) {
									cExpectedUnionTypes += std::get<1>(tupmtojsenum)->m_cTypes;
								}
								return std::get<0>(tupmtojsenum);
							} else {
								++cExpectedUnionTypes;
								return MangleType(jtypeUnionOption);
							}
						})
					);
					if(tc::all_of(vecmtType)) {
						// NOTE: sort_unique works with final names which go to C++. It may potentially hide
						// some errors in mangling (e.g. if two different types map to the same type in C++).
						tc::sort_unique_inplace(vecmtType);
						_ASSERT(!tc::empty(vecmtType));

						if(cExpectedUnionTypes != cUnionTypes) {
							tc::append(std::cerr, "[warning] Union of literals ", 
								tc::explicit_cast<std::string>((*g_ojtsTypeChecker)->typeToString(jtypeRoot)), 
								" cannot be represented in C++. (", tc::as_dec(cExpectedUnionTypes), " != ", tc::as_dec(cUnionTypes),") \n"
							);
						}

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
						auto atypeargs = *jtypereferenceRoot->typeArguments();
						return MangleTypeArguments("std::tuple", "std::tuple", atypeargs, tc::repeat_n(tc::size(atypeargs), etypeparamTYPE));
					} else {
						// In lib.dom.ts, the type of "parent" here is ts::ObjectFlags::Reference|ts::ObjectFlags::Interface
						// but jtypereferenceRoot->target() == jtypereferenceRoot 
						// So we don't call MangleType recursively because that would trigger a stack overflow
						// 
						// interface NodeListOf<TNode> {
						// 	forEach(callbackfn: (value: TNode, key: number, parent: NodeListOf<TNode>) => void, thisArg?: any): void;
						// }

						return MangleClassOrInterface(*jtypereferenceRoot->target()->isClassOrInterface(), jtypereferenceRoot->typeArguments());
					}
				} else if(static_cast<bool>(ts::ObjectFlags::Anonymous & jobjecttypeRoot->objectFlags())) {
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
