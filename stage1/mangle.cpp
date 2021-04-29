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

std::optional<ts::TypeReference> IsTypeReference(ts::Type jtypeRoot) noexcept {
	if (ts::TypeFlags::Object != jtypeRoot->flags()) {
		return std::nullopt;
	}
	ts::ObjectType jobjecttypeRoot(tc_move(jtypeRoot));
	if (ts::ObjectFlags::Reference != jobjecttypeRoot->objectFlags()) {
		return std::nullopt;
	}
	return ts::TypeReference(tc_move(jobjecttypeRoot));
}

std::optional<ts::Symbol> IsAnonymousTypeWithTypeLiteral(ts::Type jtypeRoot) noexcept {
	if (ts::TypeFlags::Object != jtypeRoot->flags()) {
		return std::nullopt;
	}
	ts::ObjectType jobjecttypeRoot(tc_move(jtypeRoot));
	if (ts::ObjectFlags::Anonymous != jobjecttypeRoot->objectFlags()) {
		return std::nullopt;
	}
	auto josymTypeSymbol = jobjecttypeRoot->getSymbol();
	if (!josymTypeSymbol || ts::SymbolFlags::TypeLiteral != (*josymTypeSymbol)->getFlags()) {
		return std::nullopt;
	}
	return tc_move(*josymTypeSymbol);
}

bool IsTrivialType(ts::InterfaceType jinterfacetypeRoot) noexcept {
	if (jinterfacetypeRoot->typeParameters()) return false;
	if (jinterfacetypeRoot->outerTypeParameters()) return false;
	if (jinterfacetypeRoot->localTypeParameters()) return false;
	if (jinterfacetypeRoot->thisType()) {
		auto jtypeThis = *jinterfacetypeRoot->thisType();
		_ASSERTEQUAL(jtypeThis->flags(), ts::TypeFlags::TypeParameter);
		if (!tc::js::ts_ext::TypeParameter(jtypeThis)->constraint().getEmval().strictlyEquals(jinterfacetypeRoot.getEmval())) return false;
	}
	return true;
}

SMangledType WrapType(std::string strPrefix, tc::js::ReadonlyArray<ts::Type> atypeArguments, std::string strSuffix) noexcept {
	auto const vecmt = tc::make_vector(tc::transform(atypeArguments, [](ts::Type jtype) noexcept { return MangleType(jtype); }));
	return {
		tc::make_str<char>(strPrefix, tc::join_separated(tc::transform(vecmt, TC_MEMBER(.m_strWithComments)), ", "), strSuffix),
		tc::make_str<char>(strPrefix, tc::join_separated(tc::transform(vecmt, TC_MEMBER(.m_strCppCanonized)), ", "), strSuffix)
	};
}

std::optional<SMangledType> MangleBootstrapType(ts::Symbol jsym, tc::jst::js_union<tc::js::ReadonlyArray<ts::Type>, tc::jst::js_undefined> jorarrtypeArguments) noexcept {
	if(jorarrtypeArguments) {
		std::string strTarget = FullyQualifiedName(jsym);
		auto const jrarrTypeArguments = *jorarrtypeArguments;
		if ("Array" == strTarget) {
			_ASSERTEQUAL(jrarrTypeArguments->length(), 1);
			return WrapType("js::Array<", jrarrTypeArguments, ">");
		} else if ("ReadonlyArray" == strTarget) {
			_ASSERTEQUAL(jrarrTypeArguments->length(), 1);
			return WrapType("js::ReadonlyArray<", jrarrTypeArguments, ">");
		} else if ("Promise" == strTarget) {
			_ASSERTEQUAL(jrarrTypeArguments->length(), 1);
			return WrapType("js::Promise<", jrarrTypeArguments, ">");
		} else if ("Record" == strTarget) {
			_ASSERTEQUAL(jrarrTypeArguments->length(), 2);
			return WrapType("js::Record<", jrarrTypeArguments, ">");
		}
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

SMangledType MangleType(tc::js::ts::Type jtypeRoot, bool bUseTypeAlias) noexcept {
	_ASSERT(g_bGlobalScopeConstructionComplete);
	
	// See checker.ts:typeToTypeNodeHelper
	if (ts::TypeFlags::Any == jtypeRoot->flags() ||
		ts::TypeFlags::Unknown == jtypeRoot->flags()
		) {
		return {mangled_no_comments, "js_unknown"};
	}
	if (ts::TypeFlags::String == jtypeRoot->flags()) {
		return {mangled_no_comments, "js_string"};
	}
	if (ts::TypeFlags::StringLiteral == jtypeRoot->flags()) {
		return CommentType("js_string", jtypeRoot);
	}
	if (ts::TypeFlags::Number == jtypeRoot->flags()) {
		return {mangled_no_comments, "double"};
	}
	if (ts::TypeFlags::NumberLiteral == jtypeRoot->flags()) {
		return CommentType("double", jtypeRoot);
	}
	if (ts::TypeFlags::Boolean == jtypeRoot->flags()) {
		return {mangled_no_comments, "bool"};
	}
	if (ts::TypeFlags::BooleanLiteral == jtypeRoot->flags()) {
		return CommentType("bool", jtypeRoot);
	}
	if (ts::TypeFlags::Void == jtypeRoot->flags()) {
		return {mangled_no_comments, "void"};
	}
	if (ts::TypeFlags::Undefined == jtypeRoot->flags()) {
		return {mangled_no_comments, "js_undefined"};
	}
	if (ts::TypeFlags::Null == jtypeRoot->flags()) {
		return {mangled_no_comments, "js_null"};
	}
	if(bUseTypeAlias) {
		if(auto const ojsymAlias = jtypeRoot->aliasSymbol()) {
			if(ecpptypeTYPEALIAS & CppType(*ojsymAlias)) {
				if(auto omt = MangleBootstrapType(*ojsymAlias, jtypeRoot->aliasTypeArguments())) {
					return *omt;
				} else if(auto const ojstypealias = tc::cont_find<tc::return_element_or_null>(g_setjstypealias, FullyQualifiedName(*ojsymAlias))) {
					return {mangled_no_comments, ojstypealias->m_strMangledName};
				}
			}
		}	
	}
	if (auto const jouniontypeRoot = jtypeRoot->isUnion()) {
		_ASSERT(1 < (*jouniontypeRoot)->types()->length());
		auto vecmtType = tc::make_vector(tc::transform((*jouniontypeRoot)->types(), [&](ts::Type const jtypeUnionOption) noexcept {
			return MangleType(jtypeUnionOption);
		}));
		if(!tc::find_first_if<tc::return_bool>(
			vecmtType, 
			[](SMangledType const& mt) noexcept { return tc::equal("js_unknown", mt.m_strCppCanonized); }
		)) {
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
						"js_union<", tc::join_separated(tc::transform(vecmtType, TC_MEMBER(.m_strWithComments)), ", "), ">"
					),
					tc::make_str(
						"js_union<", tc::join_separated(tc::transform(vecmtType, TC_MEMBER(.m_strCppCanonized)), ", "), ">"
					)
				};
			}
		}
	}
	std::vector<std::string> vecstrExtraInfo;
	if (auto jotypereferenceRoot = IsTypeReference(jtypeRoot)) {
		if (auto josymTargetSymbol = (*jotypereferenceRoot)->target()->getSymbol(); josymTargetSymbol) {
			if(auto const omt = MangleBootstrapType(*josymTargetSymbol, (*jotypereferenceRoot)->typeArguments())) {
				return *omt;
			}
			tc::cont_emplace_back(vecstrExtraInfo, tc::concat("TypeReference=", FullyQualifiedName(*josymTargetSymbol)));
		}
	}
	if (auto josymTypeLiteral = IsAnonymousTypeWithTypeLiteral(jtypeRoot)) {
		auto jsymTypeLiteral = std::move(*josymTypeLiteral);
		std::vector<ts::Symbol> vecjsymMember = tc::make_vector(*tc::js::ts_ext::Symbol(jsymTypeLiteral)->members());
		std::vector<std::string> vecstrMemberName = tc::make_vector(tc::transform(vecjsymMember, [](ts::Symbol const& jsymMember) {
			return tc::explicit_cast<std::string>(jsymMember->getName());
		}));
		tc::cont_emplace_back(vecstrExtraInfo, tc::make_str(
			"AnonymousTypeWithTypeLiteral(members:[",
			tc::join_separated(vecstrMemberName, ", "),
			"])"
		));
		if (vecstrMemberName == std::vector<std::string>{"__call"}) {
			ts::Symbol const& jsymSignature = vecjsymMember[0];
			_ASSERTEQUAL(jsymSignature->getFlags(), ts::SymbolFlags::Signature);
			_ASSERTEQUAL(jsymSignature->declarations()->length(), 1);
			ts::Signature const jtsSignature = *(*g_ojtsTypeChecker)->getSignatureFromDeclaration(
				ts::CallSignatureDeclaration(jsymSignature->declarations()[0])
			);
			auto mtReturnType = MangleType(jtsSignature->getReturnType());
			auto vecmtParameters = tc::make_vector(tc::transform(jtsSignature->getParameters(),
				[&](ts::Symbol const jsymParameter) noexcept {
					// TODO: deduplicate with SJsVariableLike.
					_ASSERTEQUAL(jsymParameter->declarations()->length(), 1);
					// TODO: add parameter name to the type.
					return MangleType((*g_ojtsTypeChecker)->getTypeOfSymbolAtLocation(jsymParameter, jsymParameter->declarations()[0]));
				}
			));
			return {
				tc::make_str(
					"js_function<", mtReturnType.m_strWithComments, "(", tc::join_separated(tc::transform(vecmtParameters, TC_MEMBER(.m_strWithComments)), ", "), ")>"
				),
				tc::make_str(
					"js_function<", mtReturnType.m_strCppCanonized, tc::join_separated(tc::transform(vecmtParameters, TC_MEMBER(.m_strCppCanonized)), ", "), ">"
				)
			};
		}
	}
	if (auto jointerfacetypeRoot = jtypeRoot->isClassOrInterface()) {
		if (IsTrivialType(*jointerfacetypeRoot)) {
			_ASSERTEQUAL((*jointerfacetypeRoot)->flags(), ts::TypeFlags::Object);
			auto const strName = FullyQualifiedName(*(*jointerfacetypeRoot)->getSymbol());
			if(auto ojsclass = tc::cont_find<tc::return_element_or_null>(g_setjsclass, FullyQualifiedName(*(*jointerfacetypeRoot)->getSymbol()))) {
				return {mangled_no_comments, ojsclass->m_strMangledName};
			} else {
				tc::cont_emplace_back(vecstrExtraInfo, tc::concat("UnknownMangledClassOrInterface=", strName));
			}
		}
		std::string strThisType = "undefined";
		if ((*jointerfacetypeRoot)->thisType()) {
			strThisType = FullyQualifiedName(*(*(*jointerfacetypeRoot)->thisType())->getSymbol());
		}
		tc::cont_emplace_back(vecstrExtraInfo, tc::concat(
			"ClassOrInterface(",
			"typeParameters:", tc::as_dec((*jointerfacetypeRoot)->typeParameters() ? (*(*jointerfacetypeRoot)->typeParameters())->length() : -1), ","
			"outerTypeParameters:", tc::as_dec((*jointerfacetypeRoot)->outerTypeParameters() ? (*(*jointerfacetypeRoot)->outerTypeParameters())->length() : -1), ",",
			"localTypeParameters:", tc::as_dec((*jointerfacetypeRoot)->localTypeParameters() ? (*(*jointerfacetypeRoot)->localTypeParameters())->length() : -1), ","
			"thisType:", strThisType,
			")"
		));
	}
	if ((ts::TypeFlags::NumberLiteral | ts::TypeFlags::EnumLiteral) == jtypeRoot->flags() ||
			(ts::TypeFlags::StringLiteral | ts::TypeFlags::EnumLiteral) == jtypeRoot->flags()) {
		auto jsymParentSymbol = *tc::js::ts_ext::Symbol(*jtypeRoot->getSymbol())->parent();
		_ASSERT(ts::SymbolFlags::RegularEnum == jsymParentSymbol->getFlags() ||
			ts::SymbolFlags::ConstEnum == jsymParentSymbol->getFlags());

		auto const strName = FullyQualifiedName(jsymParentSymbol);
		if(auto ojsenum = tc::cont_find<tc::return_element_or_null>(g_setjsenum, strName)) {
			return CommentType(ojsenum->m_strMangledName, jtypeRoot);
		} else {
			tc::cont_emplace_back(vecstrExtraInfo, tc::concat("UnknownMangledEnum=", strName));
		}
	}
	return {
		tc::make_str(
			"js_unknown /*flags=",
			tc::as_dec(static_cast<int>(jtypeRoot->flags())),
			": ",
			tc::explicit_cast<std::string>((*g_ojtsTypeChecker)->typeToString(jtypeRoot)),
			" (", tc::join_separated(vecstrExtraInfo, ","), ")",
			"*/"
		),
		"js_unknown"
	};
};
