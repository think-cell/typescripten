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
	SMangledType WrapType(std::string strPrefix, tc::js::ReadonlyArray<ts::Type> atypeArguments, std::string strSuffix) noexcept {
		auto const vecmt = tc::make_vector(tc::transform(atypeArguments, [](ts::Type jtype) noexcept { return MangleType(jtype); }));
		return {
			tc::make_str<char>(strPrefix, tc::join_separated(tc::transform(vecmt, TC_MEMBER(.m_strWithComments)), ", "), strSuffix),
			tc::make_str<char>(strPrefix, tc::join_separated(tc::transform(vecmt, TC_MEMBER(.m_strCppCanonized)), ", "), strSuffix)
		};
	}

	std::optional<SMangledType> MangleBootstrapType(ts::Symbol jsym, tc::js::ReadonlyArray<ts::Type> jatypeargs) noexcept {
		std::string strTarget = FullyQualifiedName(jsym);
		if ("Array" == strTarget) {
			_ASSERTEQUAL(jatypeargs->length(), 1);
			return WrapType("js::Array<", jatypeargs, ">");
		} else if ("ReadonlyArray" == strTarget) {
			_ASSERTEQUAL(jatypeargs->length(), 1);
			return WrapType("js::ReadonlyArray<", jatypeargs, ">");
		} else if ("Promise" == strTarget) {
			_ASSERTEQUAL(jatypeargs->length(), 1);
			return WrapType("js::Promise<", jatypeargs, ">");
		} else if ("Record" == strTarget) {
			_ASSERTEQUAL(jatypeargs->length(), 2);
			return WrapType("js::Record<", jatypeargs, ">");
		}
		return std::nullopt;
	}

	SMangledType CommentType(std::string const strCppType, tc::js::ts::Type const jtypeRoot) noexcept {
		return {
			tc::make_str(
				strCppType,
				" /*",
				tc::explicit_cast<std::string>((*g_ojtsTypeChecker)->typeToString(jtypeRoot)),
				"*/"
			),
			strCppType
		};
	}
}

SMangledType MangleType(tc::js::ts::Type jtypeRoot, bool bUseTypeAlias) noexcept {
	_ASSERT(g_bGlobalScopeConstructionComplete);
	
	switch(jtypeRoot->flags()) {
	case ts::TypeFlags::Any:
	case ts::TypeFlags::Unknown:
		return {mangled_no_comments, "tc::js::any"};
	case ts::TypeFlags::String:
		return {mangled_no_comments, "tc::js::string"};
	case ts::TypeFlags::Number:
		return {mangled_no_comments, "double"};
	case ts::TypeFlags::Boolean:
		return {mangled_no_comments, "bool"};
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
		auto jsymParentSymbol = *tc::js::ts_ext::Symbol(*jtypeRoot->getSymbol())->parent();
		_ASSERT(ts::SymbolFlags::RegularEnum == jsymParentSymbol->getFlags() ||
			ts::SymbolFlags::ConstEnum == jsymParentSymbol->getFlags());

		auto const strName = FullyQualifiedName(jsymParentSymbol);
		if(auto ojsenum = tc::cont_find<tc::return_element_or_null>(g_setjsenum, strName)) {
			return CommentType(ojsenum->m_strMangledName, jtypeRoot);
		} else {
			return {
				tc::make_str(
					"tc::js::any /*UnknownMangledEnum=", strName, "*/"
				),
				"tc::js::any"
			};
		}
	}
	// BigIntLiteral
	// ESSymbol
	// UniqueESSymbol
	case ts::TypeFlags::Void:
		return {mangled_no_comments, "void"};
	case ts::TypeFlags::Undefined:
		return {mangled_no_comments, "tc::js::undefined"};
	case ts::TypeFlags::Null:
		return {mangled_no_comments, "tc::js::null"};
	// Never
	// TypeParameter
	default:
		{
			if(bUseTypeAlias) {
				if(auto const ojsymAlias = jtypeRoot->aliasSymbol()) {
					if(ecpptypeTYPEALIAS & CppType(*ojsymAlias)) {
						if(auto const oatypeargs = jtypeRoot->aliasTypeArguments()) {
							if(auto omt = MangleBootstrapType(*ojsymAlias, *oatypeargs)) {
								return *omt;
							} else {
								return {
									tc::make_str(
										"tc::js::any /*UnsupportedGenericTypeAlias=", tc::explicit_cast<std::string>((*g_ojtsTypeChecker)->typeToString(jtypeRoot)), "*/"
									),
									"tc::js::any"
								};
							} 
						} else if(auto const ojstypealias = tc::cont_find<tc::return_element_or_null>(g_setjstypealias, FullyQualifiedName(*ojsymAlias))) {
							return {mangled_no_comments, ojstypealias->m_strMangledName};
						} else {
							return {
								tc::make_str(
									"tc::js::any /*UnknownTypeAlias=", tc::explicit_cast<std::string>((*g_ojtsTypeChecker)->typeToString(jtypeRoot)), "*/"
								),
								"tc::js::any"
							};
						}
					}
				}	
			}
			if (auto const jouniontypeRoot = jtypeRoot->isUnion()) {
				_ASSERT(0!=static_cast<int>(jtypeRoot->getFlags() & ts::TypeFlags::Union));
				_ASSERT(1 < (*jouniontypeRoot)->types()->length());
				auto vecmtType = tc::make_vector(tc::transform((*jouniontypeRoot)->types(), [&](ts::Type const jtypeUnionOption) noexcept {
					return MangleType(jtypeUnionOption);
				}));
				if(tc::find_first_if<tc::return_bool>(
					vecmtType, 
					[](SMangledType const& mt) noexcept { return tc::equal("tc::js::any", mt.m_strCppCanonized); }
				)) {
					return {
						tc::make_str(
							"tc::js::any /*UnionWithUnsupportedSubtype=", 
								tc::explicit_cast<std::string>((*g_ojtsTypeChecker)->typeToString(jtypeRoot)), 
								"[", tc::join_separated(tc::transform(vecmtType, TC_MEMBER(.m_strCppCanonized)), ","), "] */"
						),
						"tc::js::any"
					};
				} else {
					// NOTE: sort_unique works with final names which go to C++. It may potentially hide
					// some errors in mangling (e.g. if two different types map to the same type in C++).
					tc::sort_unique_inplace(vecmtType, [&](SMangledType const& a, SMangledType const& b) noexcept {
						return a.m_strCppCanonized < b.m_strCppCanonized; // TODO: glue duplicates' m_strWithComments
					});
					_ASSERT(!tc::empty(vecmtType));
					if (1 == tc::size(vecmtType)) {
						return tc::front(vecmtType);
					} else {
						return {
							tc::make_str(
								"tc::jst::union_t<", tc::join_separated(tc::transform(vecmtType, TC_MEMBER(.m_strWithComments)), ", "), ">"
							),
							tc::make_str(
								"tc::jst::union_t<", tc::join_separated(tc::transform(vecmtType, TC_MEMBER(.m_strCppCanonized)), ", "), ">"
							)
						};
					}
				}
			} else if(ts::TypeFlags::Object==jtypeRoot->getFlags()) {
				ts::ObjectType jobjecttypeRoot(tc_move(jtypeRoot));
				if (ts::ObjectFlags::Reference == jobjecttypeRoot->objectFlags()) {
					ts::TypeReference jtypereferenceRoot(tc_move(jobjecttypeRoot));
					if(auto josymTargetSymbol = jtypereferenceRoot->target()->getSymbol(); josymTargetSymbol) {
						if(auto const oatypeargs = jtypereferenceRoot->typeArguments()) {
							if(auto const omt = MangleBootstrapType(*josymTargetSymbol, *oatypeargs)) {
								return *omt;
							}
						}
					}
					return {
						tc::make_str(
							"tc::js::any /*TypeReference=", tc::explicit_cast<std::string>((*g_ojtsTypeChecker)->typeToString(jtypereferenceRoot)), "*/"
						),
						"tc::js::any"
					};
				} else if(ts::ObjectFlags::Anonymous == jobjecttypeRoot->objectFlags()) {
					auto josymTypeSymbol = jobjecttypeRoot->getSymbol();
					if(josymTypeSymbol && ts::SymbolFlags::TypeLiteral == (*josymTypeSymbol)->getFlags()) {
						auto const vecjsymMember = tc::make_vector(*tc::js::ts_ext::Symbol(*josymTypeSymbol)->members());
						auto const rngstrMemberName = tc::transform(vecjsymMember, [](ts::Symbol const& jsymMember) noexcept {
							return tc::explicit_cast<std::string>(jsymMember->getName());
						});
						if(tc::equal(rngstrMemberName, tc::single("__call"))) {
							ts::Symbol jsymSignature = tc::front(vecjsymMember);
							_ASSERTEQUAL(jsymSignature->getFlags(), ts::SymbolFlags::Signature);
							_ASSERTEQUAL(jsymSignature->declarations()->length(), 1);
							ts::Signature const jtsSignature = *(*g_ojtsTypeChecker)->getSignatureFromDeclaration(
								ts::CallSignatureDeclaration(tc::front(jsymSignature->declarations()))
							);
							auto mtReturnType = MangleType(jtsSignature->getReturnType());
							auto const vecmtParameters = tc::make_vector(tc::transform(jtsSignature->getParameters(),
								[&](ts::Symbol const jsymParameter) noexcept {
									// TODO: deduplicate with SJsVariableLike.
									_ASSERTEQUAL(jsymParameter->declarations()->length(), 1);
									// TODO: add parameter name to the type.
									return MangleType((*g_ojtsTypeChecker)->getTypeOfSymbolAtLocation(jsymParameter, tc::front(jsymParameter->declarations())));
								}
							));
							return {
								tc::make_str(
									"tc::jst::function<", mtReturnType.m_strWithComments, "(", tc::join_separated(tc::transform(vecmtParameters, TC_MEMBER(.m_strWithComments)), ", "), ")>"
								),
								tc::make_str(
									"tc::jst::function<", mtReturnType.m_strCppCanonized, tc::join_separated(tc::transform(vecmtParameters, TC_MEMBER(.m_strCppCanonized)), ", "), ">"
								)
							};
						} else {
							return {
								tc::make_str(
									"tc::js::any /*AnonymousTypeWithLiteralType=", 
										tc::explicit_cast<std::string>((*g_ojtsTypeChecker)->typeToString(jobjecttypeRoot)), 
										"[", tc::join_separated(rngstrMemberName, ", "), "]*/"
								),
								"tc::js::any"
							};
						}
					} else {
						return {
							tc::make_str(
								"tc::js::any /*AnonymousType=", tc::explicit_cast<std::string>((*g_ojtsTypeChecker)->typeToString(jobjecttypeRoot)), "*/"
							),
							"tc::js::any"
						};
					}
				} else if (auto jointerfacetypeRoot = jobjecttypeRoot->isClassOrInterface()) {
					auto const jinterfacetypeRoot = *jointerfacetypeRoot;
					// _ASSERT(ts::ObjectFlags::Interface==jobjecttypeRoot->objectFlags() || ts::ObjectFlags::Class==jobjecttypeRoot->objectFlags());
					auto const strName = FullyQualifiedName(*jinterfacetypeRoot->getSymbol());

					if(!jinterfacetypeRoot->typeParameters()
					&& !jinterfacetypeRoot->outerTypeParameters()
					&& !jinterfacetypeRoot->localTypeParameters()
					&& (!jinterfacetypeRoot->thisType() 
						|| tc::js::ts_ext::TypeParameter(*jinterfacetypeRoot->thisType())->constraint().getEmval().strictlyEquals(jinterfacetypeRoot.getEmval()))
					){
						if(auto ojsclass = tc::cont_find<tc::return_element_or_null>(g_setjsclass, FullyQualifiedName(*(*jointerfacetypeRoot)->getSymbol()))) {
							return {mangled_no_comments, ojsclass->m_strMangledName};
						} else {
							return {
								tc::make_str(
									"tc::js::any /*UnknownClassOrInterface=", strName, "*/"
								),
								"tc::js::any"
							};
						}
					} else {
						return {
							tc::make_str(
								"tc::js::any /*NonTrivialClassOrInterface=", strName, "*/"
							),
							"tc::js::any"
						};
					}
				} else {
					return {
						tc::make_str(
							"tc::js::any /*UnknownObjectType=", tc::explicit_cast<std::string>((*g_ojtsTypeChecker)->typeToString(jobjecttypeRoot)), " objectFlags=", tc::as_dec(static_cast<int>(jobjecttypeRoot->objectFlags())), "*/"
						),
						"tc::js::any"
					};
				}
			} else {
				return {
					tc::make_str(
						"tc::js::any /* unsupported type flags =",
						tc::as_dec(static_cast<int>(jtypeRoot->flags())),
						": ",
						tc::explicit_cast<std::string>((*g_ojtsTypeChecker)->typeToString(jtypeRoot)),
						"*/"
					),
					"tc::js::any"
				};
			}
		}
	}
};
