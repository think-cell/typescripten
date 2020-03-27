#include "precompiled.h"
#include "typescript.d.bootstrap.h"

using tc::js::globals::ts;

std::string MangleSymbolName(ts::TypeChecker const& jtsTypeChecker, ts::Symbol const jsymType) {
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
	if (jtypeRoot->flags() != ts::TypeFlags::Object) {
		return std::nullopt;
	}
	ts::ObjectType jobjecttypeRoot(tc_move(jtypeRoot));
	if (jobjecttypeRoot->objectFlags() != ts::ObjectFlags::Reference) {
		return std::nullopt;
	}
	return ts::TypeReference(tc_move(jobjecttypeRoot));
}

bool IsTrivialType(ts::InterfaceType jinterfacetypeRoot) {
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
	// TODO: more assertions: "I've seen these flags, I think they are unimportant, explicitly ignoring".
	// See checker.ts:typeToTypeNodeHelper
	if (jtypeRoot->flags() & ts::TypeFlags::Any ||
		jtypeRoot->flags() & ts::TypeFlags::Unknown
		) {
		return "js_unknown";
	}
	if (jtypeRoot->flags() & ts::TypeFlags::String) {
		return "js_string";
	}
	if (jtypeRoot->flags() & ts::TypeFlags::Number) {
		return "double";
	}
	if (jtypeRoot->flags() & ts::TypeFlags::Boolean) {
		return "bool";
	}
	if (jtypeRoot->flags() & ts::TypeFlags::Void) {
		return "void";
	}
	if (jtypeRoot->flags() & ts::TypeFlags::Undefined) {
		return "js_undefined";
	}
	if (jtypeRoot->flags() & ts::TypeFlags::Null) {
		return "js_null";
	}
	if (auto jouniontypeRoot = jtypeRoot->isUnion()) {
		_ASSERT(1 < (*jouniontypeRoot)->types()->length());
		return tc::explicit_cast<std::string>(tc::concat(
			"js_union<",
			tc::join_separated(
				tc::transform((*jouniontypeRoot)->types(), [&](ts::Type const jtypeUnionOption) noexcept {
					return MangleType(jtsTypeChecker, jtypeUnionOption);
				}),
				", "
			),
			">"
		));
	}
	std::vector<std::string> vecstrExtraInfo;
	if (auto jotypereferenceRoot = IsTypeReference(jtypeRoot)) {
		std::string strTarget = tc::explicit_cast<std::string>(jtsTypeChecker->getFullyQualifiedName(*(*jotypereferenceRoot)->target()->getSymbol()));
		auto jrarrTypeArguments = (*jotypereferenceRoot)->typeArguments();
		if (strTarget == "Array") {
			_ASSERTEQUAL(1, jrarrTypeArguments->length());
			return tc::explicit_cast<std::string>(tc::concat(
				"Array<", MangleType(jtsTypeChecker, jrarrTypeArguments[0]), ">"
			));
		} else if (strTarget == "ReadonlyArray") {
			_ASSERTEQUAL(1, jrarrTypeArguments->length());
			return tc::explicit_cast<std::string>(tc::concat(
				"ReadonlyArray<", MangleType(jtsTypeChecker, jrarrTypeArguments[0]), ">"
			));
		}
		tc::cont_emplace_back(vecstrExtraInfo, "TypeReference");
	}
	if (auto jointerfacetypeRoot = jtypeRoot->isClassOrInterface()) {
	    if (IsTrivialType(*jointerfacetypeRoot)) {
			_ASSERTEQUAL(ts::TypeFlags::Object, (*jointerfacetypeRoot)->flags());
			return tc::explicit_cast<std::string>(tc::concat(
				"js_ref<", MangleSymbolName(jtsTypeChecker, *(*jointerfacetypeRoot)->getSymbol()), ">"
			));
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
	return tc::explicit_cast<std::string>(tc::concat(
		"js_unknown /*flags=",
		tc::as_dec(static_cast<int>(jtypeRoot->flags())),
		": ",
		tc::explicit_cast<std::string>(jtsTypeChecker->typeToString(jtypeRoot)),
		" (", tc::join_separated(vecstrExtraInfo, ","), ")",
		"*/")
	);
};
