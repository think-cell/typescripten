#pragma once

using tc::js::ts;
namespace jst = tc::jst;
namespace js = tc::js;

std::string FullyQualifiedName(ts::Symbol jsymType) noexcept;
tc::jst::optional<tc::js::ts::Symbol> OptSymbolOrAliasSymbol(tc::js::ts::Type jtype) noexcept;
tc::js::ts::Symbol SymbolOrAliasSymbol(tc::js::ts::Type jtype) noexcept;

DEFINE_ENUM(ETypeParameter, etypeparam, (TYPE)(ENUM)(NUMBER)(KEYOF))

DEFINE_ADL_TAG_TYPE(mangling_error)

struct SMangledType {
	std::string m_strCppCanonized; // may be empty, i.e., type could not be mangled, implies tc::js::any
	std::string m_strWithComments; // never empty, may include comment

	SMangledType(std::string strCppCanonized) noexcept;
	SMangledType(std::string strCppCanonized, std::string strWithComments) noexcept;
	SMangledType(mangling_error_t, std::string strWithComments) noexcept;

	std::string ExpandType() const& noexcept;
	operator bool() const& noexcept;
	friend bool operator<(SMangledType const& lhs, SMangledType const& rhs) noexcept;
};

SMangledType MangleType(ts::Type jtypeRoot, bool bUseTypeAlias = true) noexcept;
