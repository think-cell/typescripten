#pragma once

#include <string>
#include <utility>
#include "typescript.d.bootstrap.h"

std::string FullyQualifiedName(tc::js::ts::Symbol jsymType) noexcept;

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

SMangledType MangleType(tc::js::ts::Type jtypeRoot, bool bUseTypeAlias = true) noexcept;
