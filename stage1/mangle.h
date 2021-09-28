#pragma once

#include <string>
#include <utility>
#include "typescript.d.bootstrap.h"

std::string FullyQualifiedName(tc::js::ts::Symbol jsymType) noexcept;

DEFINE_ADL_TAG_TYPE(mangling_error)

struct SMangledType {
	std::string m_strCppCanonized; // may be empty, i.e., type could not be mangled, implies tc::js::any
	std::string m_strWithComments; // never empty, may include comment

	SMangledType(std::string strCppCanonized) noexcept
		: m_strCppCanonized(strCppCanonized)
		, m_strWithComments(tc_move(strCppCanonized))
	{}
	
	SMangledType(std::string strCppCanonized, std::string strWithComments) noexcept
		: m_strCppCanonized(tc_move(strCppCanonized))
		, m_strWithComments(tc_move(strWithComments))
	{}

	SMangledType(mangling_error_t, std::string strWithComments) noexcept
		: m_strWithComments(std::move(strWithComments))
	{}

	std::string ExpandType() const& noexcept {
		return tc::empty(m_strCppCanonized)
			? tc::make_str("tc::js::any")
			: m_strCppCanonized;
	}

	operator bool() const& noexcept {
		return !tc::empty(m_strCppCanonized);
	}
};

SMangledType MangleType(tc::js::ts::Type jtypeRoot, bool bUseTypeAlias = true) noexcept;
