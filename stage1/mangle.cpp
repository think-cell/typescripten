#include "precompiled.h"
#include "typescript.d.bootstrap.h"
#include "mangle.h"

using tc::js::globals::ts;

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
		_ASSERTEQUAL(ts::TypeFlags::TypeParameter, jtypeThis->flags());
		if (!jtypeThis->constraint().getEmval().strictlyEquals(jinterfacetypeRoot.getEmval())) return false;
	}
	return true;
}

std::string MangleType(ts::TypeChecker const jtsTypeChecker, ts::Type const jtypeRoot) noexcept {
	// See checker.ts:typeToTypeNodeHelper
	if (ts::TypeFlags::Any == jtypeRoot->flags() ||
		ts::TypeFlags::Unknown == jtypeRoot->flags()
		) {
		return "js_unknown";
	}
	if (ts::TypeFlags::String == jtypeRoot->flags()) {
		return "js_string";
	}
	if (ts::TypeFlags::Number == jtypeRoot->flags()) {
		return "double";
	}
	if (ts::TypeFlags::Boolean == jtypeRoot->flags() || ts::TypeFlags::BooleanLiteral == jtypeRoot->flags()) {
		return "bool";
	}
	if (ts::TypeFlags::Void == jtypeRoot->flags()) {
		return "void";
	}
	if (ts::TypeFlags::Undefined == jtypeRoot->flags()) {
		return "js_undefined";
	}
	if (ts::TypeFlags::Null == jtypeRoot->flags()) {
		return "js_null";
	}
	if (auto jouniontypeRoot = jtypeRoot->isUnion()) {
		_ASSERT(1 < (*jouniontypeRoot)->types()->length());
		auto vecstrType = tc::make_vector(tc::transform((*jouniontypeRoot)->types(), [&](ts::Type const jtypeUnionOption) noexcept {
			return MangleType(jtsTypeChecker, jtypeUnionOption);
		}));
		// NOTE: sort_unique works with final names which go to C++. It may potentially hide
		// some errors in mangling (e.g. if two different types map to the same type in C++).
		tc::sort_unique_inplace(vecstrType);
		_ASSERT(0 < vecstrType.size());
		if (1 == vecstrType.size()) {
			return vecstrType[0];
		} else {
			return tc::explicit_cast<std::string>(tc::concat(
				"js_union<", tc::join_separated(vecstrType, ", "), ">"
			));
		}
	}
	std::vector<std::string> vecstrExtraInfo;
	if (auto jotypereferenceRoot = IsTypeReference(jtypeRoot)) {
		std::string strTarget = tc::explicit_cast<std::string>(jtsTypeChecker->getFullyQualifiedName(*(*jotypereferenceRoot)->target()->getSymbol()));
		auto jrarrTypeArguments = (*jotypereferenceRoot)->typeArguments();
		if ("Array" == strTarget) {
			_ASSERTEQUAL(1, jrarrTypeArguments->length());
			return tc::explicit_cast<std::string>(tc::concat(
				"globals::Array<", MangleType(jtsTypeChecker, jrarrTypeArguments[0]), ">"
			));
		} else if ("ReadonlyArray" == strTarget) {
			_ASSERTEQUAL(1, jrarrTypeArguments->length());
			return tc::explicit_cast<std::string>(tc::concat(
				"globals::ReadonlyArray<", MangleType(jtsTypeChecker, jrarrTypeArguments[0]), ">"
			));
		}
		tc::cont_emplace_back(vecstrExtraInfo, "TypeReference");
	}
	if (auto jointerfacetypeRoot = jtypeRoot->isClassOrInterface()) {
		if (IsTrivialType(*jointerfacetypeRoot)) {
			_ASSERTEQUAL(ts::TypeFlags::Object, (*jointerfacetypeRoot)->flags());
			auto strMangledType = MangleSymbolName(jtsTypeChecker, *(*jointerfacetypeRoot)->getSymbol());
			if (0 < g_usstrAllowedMangledTypes.count(strMangledType)) {
				return strMangledType;
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
			return strMangledType;
		} else {
			tc::cont_emplace_back(vecstrExtraInfo, tc::concat("UnknownMangledEnum=", strMangledType));
		}
	}
	return tc::explicit_cast<std::string>(tc::concat(
		"js_unknown /*flags=",
		tc::as_dec(static_cast<int>(jtypeRoot->flags())),
		": ",
		tc::explicit_cast<std::string>(jtsTypeChecker->typeToString(jtypeRoot)),
		" (", tc::join_separated(vecstrExtraInfo, ","), ")",
		"*/")
	);
};
