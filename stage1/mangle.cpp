#include "precompiled.h"
#include "typescript.d.bootstrap.h"
#include "mangle.h"
#include "walk_symbol.h"
#include "jstypes.h"

using tc::js::ts;

extern std::optional<tc::js::ts::TypeChecker> g_ojtsTypeChecker;
std::unordered_set<std::string> g_usstrAllowedMangledTypes;

std::string MangleSymbolName(ts::Symbol const jsymType) noexcept {
	std::string strMangled = "_js_j";
	tc::for_each(tc::explicit_cast<std::string>((*g_ojtsTypeChecker)->getFullyQualifiedName(jsymType)), [&](char c) noexcept {
		switch (c) {
		case '_': tc::append(strMangled, "_u"); break;
		case ',': tc::append(strMangled, "_c"); break;
		case '.': tc::append(strMangled, "_d"); break;
		case '-': tc::append(strMangled, "_m"); break;
		case '"': tc::append(strMangled, "_q"); break;
		default: tc::cont_emplace_back(strMangled, c); break;
		}
	});
	return strMangled;
}

std::string CppifyName(ts::Symbol jsymSymbol) noexcept {
    std::string strSourceName = tc::explicit_cast<std::string>(jsymSymbol->getName());
    if ('"' == strSourceName.front() && '"' == strSourceName.back()) {
        strSourceName = strSourceName.substr(1, strSourceName.length() - 2);
    }
    // TODO: https://en.cppreference.com/w/cpp/language/identifiers
    std::string strResult;
    bool bLastIsUnderscore = false;
    tc::for_each(
        tc::transform(strSourceName, [&](char c) noexcept {
            if(('a' <= c && c <= 'z')
            || ('A' <= c && c <= 'Z')
            || ('0' <= c && c <= '9')
            || '_' == c) {
                return c;
            } else {
                return '_';
            }
        }),
        [&](char c) noexcept {
            if (bLastIsUnderscore && c == '_') {
                tc::append(strResult, "u_u"); // ABC__DEF --> ABC_u_uDEF
                return;
            }
            bLastIsUnderscore = c == '_';
            strResult += c;
        }
    );
    static std::unordered_set<std::string> usstrCppKeywords = {"alignas", "alignof", "and", "and_eq", "asm", "auto", "bitand", "bitor", "bool", "break", "case", "catch", "char", "char16_t", "char32_t", "class", "compl", "const", "constexpr", "const_cast", "continue", "decltype", "default", "delete", "do", "double", "dynamic_cast", "else", "enum", "explicit", "export", "extern", "false", "float", "for", "friend", "goto", "if", "inline", "int", "long", "mutable", "namespace", "new", "noexcept", "not", "not_eq", "nullptr", "operator", "or", "or_eq", "private", "protected", "public", "register", "reinterpret_cast", "return", "short", "signed", "sizeof", "static", "static_assert", "static_cast", "struct", "switch", "template", "this", "thread_local", "throw", "true", "try", "typedef", "typeid", "typename", "union", "unsigned", "using", "virtual", "void", "volatile", "wchar_t", "while", "xor", "xor_eq"};
    if (usstrCppKeywords.count(strResult)) {
        tc::append(strResult, "_");
    }
    _ASSERT(!strResult.empty());
    return strResult;
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

SMangledType WrapType(std::string const strPrefix, tc::js::ReadonlyArray<ts::Type> atypeArguments, std::string const strSuffix) noexcept {
	auto const rngmt = tc::transform(atypeArguments, [](ts::Type jtype) noexcept { return MangleType(jtype); });
	return {
		tc::explicit_cast<std::string>(tc::concat(strPrefix, tc::join_separated(tc::transform(rngmt, TC_MEMBER(.m_strWithComments)), ", "), strSuffix)),
		tc::explicit_cast<std::string>(tc::concat(strPrefix, tc::join_separated(tc::transform(rngmt, TC_MEMBER(.m_strCppCanonized)), ", "), strSuffix)),
	};
}

std::optional<SMangledType> MangleBootstrapType(ts::Symbol jsym, tc::jst::js_union<tc::js::ReadonlyArray<ts::Type>, tc::jst::js_undefined> jorarrtypeArguments) noexcept {
	if(jorarrtypeArguments) {
		std::string strTarget = tc::explicit_cast<std::string>((*g_ojtsTypeChecker)->getFullyQualifiedName(jsym));
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
		tc::explicit_cast<std::string>(tc::concat(
			strCppType,
			" /*",
			tc::explicit_cast<std::string>((*g_ojtsTypeChecker)->typeToString(jtypeRoot)),
			"*/"
		)),
		strCppType
	};
}

SMangledType MangleType(tc::js::ts::Type jtypeRoot, bool bUseTypeAlias) noexcept {
	_ASSERT(!tc::empty(g_usstrAllowedMangledTypes));
	
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
		if(auto ojsymAlias = jtypeRoot->aliasSymbol()) {
			if(ecpptypeTYPEALIAS & CppType(*ojsymAlias)) {
				if(auto omt = MangleBootstrapType(*ojsymAlias, jtypeRoot->aliasTypeArguments())) {
					return *omt;
				}
				return {mangled_no_comments, MangleSymbolName(*ojsymAlias)};
			}
		}	
	}
	if (auto jouniontypeRoot = tc::js::ts_ext::isUnion(jtypeRoot)) {
		_ASSERT(1 < (*jouniontypeRoot)->types()->length());
		auto vecmtType = tc::make_vector(tc::transform((*jouniontypeRoot)->types(), [&](ts::Type const jtypeUnionOption) noexcept {
			return MangleType(jtypeUnionOption);
		}));
		auto isJsUnknown = [](SMangledType const& mt) noexcept { return mt.m_strCppCanonized == "js_unknown"; };
		if (std::find_if(vecmtType.begin(), vecmtType.end(), isJsUnknown) == vecmtType.end()) {
			// NOTE: sort_unique works with final names which go to C++. It may potentially hide
			// some errors in mangling (e.g. if two different types map to the same type in C++).
			tc::sort_unique_inplace(vecmtType, [&](SMangledType const& a, SMangledType const& b) noexcept {
				return a.m_strCppCanonized < b.m_strCppCanonized; // TODO: glue duplicates' m_strWithComments
			});
			_ASSERT(0 < vecmtType.size());
			if (1 == vecmtType.size()) {
				return vecmtType[0];
			} else {
				return {
					tc::explicit_cast<std::string>(tc::concat(
						"js_union<", tc::join_separated(tc::transform(vecmtType, TC_MEMBER(.m_strWithComments)), ", "), ">"
					)),
					tc::explicit_cast<std::string>(tc::concat(
						"js_union<", tc::join_separated(tc::transform(vecmtType, TC_MEMBER(.m_strCppCanonized)), ", "), ">"
					))
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
			tc::cont_emplace_back(vecstrExtraInfo, tc::concat("TypeReference=", tc::explicit_cast<std::string>((*g_ojtsTypeChecker)->getFullyQualifiedName(*josymTargetSymbol))));
		}
	}
	if (auto josymTypeLiteral = IsAnonymousTypeWithTypeLiteral(jtypeRoot)) {
		auto jsymTypeLiteral = std::move(*josymTypeLiteral);
		std::vector<ts::Symbol> vecjsymMember = tc::make_vector(*tc::js::ts_ext::Symbol(jsymTypeLiteral)->members());
		std::vector<std::string> vecstrMemberName = tc::make_vector(tc::transform(vecjsymMember, [](ts::Symbol const& jsymMember) {
			return tc::explicit_cast<std::string>(jsymMember->getName());
		}));
		tc::cont_emplace_back(vecstrExtraInfo, tc::explicit_cast<std::string>(tc::concat(
			"AnonymousTypeWithTypeLiteral(members:[",
			tc::join_separated(vecstrMemberName, ", "),
			"])"
		)));
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
				tc::explicit_cast<std::string>(tc::concat(
					"js_function<", mtReturnType.m_strWithComments, "(", tc::join_separated(tc::transform(vecmtParameters, TC_MEMBER(.m_strWithComments)), ", "), ")>"
				)),
				tc::explicit_cast<std::string>(tc::concat(
					"js_function<", mtReturnType.m_strCppCanonized, tc::join_separated(tc::transform(vecmtParameters, TC_MEMBER(.m_strCppCanonized)), ", "), ">"
				))
			};
		}
	}
	if (auto jointerfacetypeRoot = tc::js::ts_ext::isClassOrInterface(jtypeRoot)) {
		if (IsTrivialType(*jointerfacetypeRoot)) {
			_ASSERTEQUAL((*jointerfacetypeRoot)->flags(), ts::TypeFlags::Object);
			auto strMangledType = MangleSymbolName(*(*jointerfacetypeRoot)->getSymbol());
			if (0 < g_usstrAllowedMangledTypes.count(strMangledType)) {
				return {mangled_no_comments, strMangledType};
			} else {
				tc::cont_emplace_back(vecstrExtraInfo, tc::concat("UnknownMangledClassOrInterface=", strMangledType));
			}
		}
		std::string strThisType = "undefined";
		if ((*jointerfacetypeRoot)->thisType()) {
			strThisType = tc::explicit_cast<std::string>((*g_ojtsTypeChecker)->getFullyQualifiedName(*(*(*jointerfacetypeRoot)->thisType())->getSymbol()));
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
		auto strMangledType = MangleSymbolName(jsymParentSymbol);
		if (0 < g_usstrAllowedMangledTypes.count(strMangledType)) {
			return CommentType(tc_move(strMangledType), jtypeRoot);
		} else {
			tc::cont_emplace_back(vecstrExtraInfo, tc::concat("UnknownMangledEnum=", strMangledType));
		}
	}
	return {
		tc::explicit_cast<std::string>(tc::concat(
			"js_unknown /*flags=",
			tc::as_dec(static_cast<int>(jtypeRoot->flags())),
			": ",
			tc::explicit_cast<std::string>((*g_ojtsTypeChecker)->typeToString(jtypeRoot)),
			" (", tc::join_separated(vecstrExtraInfo, ","), ")",
			"*/"
		)),
		"js_unknown"
	};
};
