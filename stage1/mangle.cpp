#include "precompiled.h"
#include "typescript.d.bootstrap.h"
#include "mangle.h"

using tc::js::ts;

std::unordered_set<std::string> g_usstrAllowedMangledTypes;

std::string MangleSymbolName(ts::TypeChecker const& jtsTypeChecker, ts::Symbol const jsymType) noexcept {
	std::string strMangled = "_js_j";
	tc::for_each(tc::explicit_cast<std::string>(jtsTypeChecker->getFullyQualifiedName(jsymType)), [&](char c) noexcept {
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

bool IsTrivialType(ts::InterfaceType jinterfacetypeRoot) noexcept {
	if (jinterfacetypeRoot->typeParameters()) return false;
	if (jinterfacetypeRoot->outerTypeParameters()) return false;
	if (jinterfacetypeRoot->localTypeParameters()) return false;
	if (jinterfacetypeRoot->thisType()) {
		auto jtypeThis = *jinterfacetypeRoot->thisType();
		_ASSERTEQUAL(jtypeThis->flags(), ts::TypeFlags::TypeParameter);
		if (!jtypeThis->constraint().getEmval().strictlyEquals(jinterfacetypeRoot.getEmval())) return false;
	}
	return true;
}

SMangledType WrapType(std::string const strPrefix, SMangledType const mtType, std::string const strSuffix) noexcept {
	return {
		tc::explicit_cast<std::string>(tc::concat(strPrefix, mtType.m_strWithComments, strSuffix)),
		tc::explicit_cast<std::string>(tc::concat(strPrefix, mtType.m_strCppCanonized, strSuffix)),
	};
}

SMangledType CommentType(tc::js::ts::TypeChecker const jtsTypeChecker, std::string const strCppType, tc::js::ts::Type const jtypeRoot) noexcept {
	return {
		tc::explicit_cast<std::string>(tc::concat(
			strCppType,
			" /*",
			tc::explicit_cast<std::string>(jtsTypeChecker->typeToString(jtypeRoot)),
			"*/"
		)),
		strCppType
	};
}

SMangledType MangleType(tc::js::ts::TypeChecker jtsTypeChecker, tc::js::ts::Type jtypeRoot) noexcept {
	// See checker.ts:typeToTypeNodeHelper
	if (ts::TypeFlags::Any == jtypeRoot->flags() ||
		ts::TypeFlags::Unknown == jtypeRoot->flags()
		) {
		return {mangled_no_comments, "js_unknown"};
	}
	if (ts::TypeFlags::String == jtypeRoot->flags()) {
		return {mangled_no_comments, "js_string"};
	}
	if (ts::TypeFlags::Number == jtypeRoot->flags()) {
		return {mangled_no_comments, "double"};
	}
	if (ts::TypeFlags::Boolean == jtypeRoot->flags()) {
		return {mangled_no_comments, "bool"}; // TODO: add comment for BooleanLiteral
	}
	if (ts::TypeFlags::BooleanLiteral == jtypeRoot->flags()) {
		return CommentType(jtsTypeChecker, "bool", jtypeRoot);
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
	if (auto jouniontypeRoot = jtypeRoot->isUnion()) {
		_ASSERT(1 < (*jouniontypeRoot)->types()->length());
		auto vecmtType = tc::make_vector(tc::transform((*jouniontypeRoot)->types(), [&](ts::Type const jtypeUnionOption) noexcept {
			return MangleType(jtsTypeChecker, jtypeUnionOption);
		}));
		auto isJsUnknown = [](const SMangledType &mt) noexcept { return mt.m_strCppCanonized == "js_unknown"; };
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
						"js_union<", tc::join_separated(tc::transform(vecmtType, [&](SMangledType const& mt) { return mt.m_strWithComments; }), ", "), ">"
					)),
					tc::explicit_cast<std::string>(tc::concat(
						"js_union<", tc::join_separated(tc::transform(vecmtType, [&](SMangledType const& mt) { return mt.m_strCppCanonized; }), ", "), ">"
					))
				};
			}
		}
	}
	std::vector<std::string> vecstrExtraInfo;
	if (auto jotypereferenceRoot = IsTypeReference(jtypeRoot)) {
		if (auto josymTargetSymbol = (*jotypereferenceRoot)->target()->getSymbol(); josymTargetSymbol) {
			std::string strTarget = tc::explicit_cast<std::string>(jtsTypeChecker->getFullyQualifiedName(*josymTargetSymbol));
			auto jrarrTypeArguments = (*jotypereferenceRoot)->typeArguments();
			if ("Array" == strTarget) {
				_ASSERTEQUAL(jrarrTypeArguments->length(), 1);
				return WrapType("js::Array<", MangleType(jtsTypeChecker, jrarrTypeArguments[0]), ">");
			} else if ("ReadonlyArray" == strTarget) {
				_ASSERTEQUAL(jrarrTypeArguments->length(), 1);
				return WrapType("js::ReadonlyArray<", MangleType(jtsTypeChecker, jrarrTypeArguments[0]), ">");
			}
			tc::cont_emplace_back(vecstrExtraInfo, "TypeReference");
		}
	}
	if (auto jointerfacetypeRoot = jtypeRoot->isClassOrInterface()) {
		if (IsTrivialType(*jointerfacetypeRoot)) {
			_ASSERTEQUAL((*jointerfacetypeRoot)->flags(), ts::TypeFlags::Object);
			auto strMangledType = MangleSymbolName(jtsTypeChecker, *(*jointerfacetypeRoot)->getSymbol());
			if (0 < g_usstrAllowedMangledTypes.count(strMangledType)) {
				return {mangled_no_comments, strMangledType};
			} else {
				tc::cont_emplace_back(vecstrExtraInfo, tc::concat("UnknownMangledClassOrInterface=", strMangledType));
			}
		}
		std::string strThisType = "undefined";
		if ((*jointerfacetypeRoot)->thisType()) {
			strThisType = tc::explicit_cast<std::string>(jtsTypeChecker->getFullyQualifiedName(*(*(*jointerfacetypeRoot)->thisType())->getSymbol()));
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
		auto jsymParentSymbol = *(*jtypeRoot->getSymbol())->parent();
		_ASSERT(ts::SymbolFlags::RegularEnum == jsymParentSymbol->getFlags() ||
			ts::SymbolFlags::ConstEnum == jsymParentSymbol->getFlags());
		auto strMangledType = MangleSymbolName(jtsTypeChecker, jsymParentSymbol);
		if (0 < g_usstrAllowedMangledTypes.count(strMangledType)) {
			return CommentType(jtsTypeChecker, tc_move(strMangledType), jtypeRoot);
		} else {
			tc::cont_emplace_back(vecstrExtraInfo, tc::concat("UnknownMangledEnum=", strMangledType));
		}
	}
	return {
		tc::explicit_cast<std::string>(tc::concat(
			"js_unknown /*flags=",
			tc::as_dec(static_cast<int>(jtypeRoot->flags())),
			": ",
			tc::explicit_cast<std::string>(jtsTypeChecker->typeToString(jtypeRoot)),
			" (", tc::join_separated(vecstrExtraInfo, ","), ")",
			"*/"
		)),
		"js_unknown"
	};
};
