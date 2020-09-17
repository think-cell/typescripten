#pragma once

#include <string>
#include <utility>
#include "typescript.d.bootstrap.h"

std::string FullyQualifiedName(tc::js::ts::Symbol jsymType) noexcept;

DEFINE_ADL_TAG_TYPE(mangled_no_comments)

struct SMangledType {
	std::string m_strWithComments;
	std::string m_strCppCanonized;

	SMangledType(mangled_no_comments_t, std::string strWithComments) noexcept
		: m_strWithComments(std::move(strWithComments))
		, m_strCppCanonized(m_strWithComments)
	{}

	SMangledType(std::string strWithComments, std::string strCppCanonized) noexcept
		: m_strWithComments(std::move(strWithComments))
		, m_strCppCanonized(std::move(strCppCanonized))
	{}
};

SMangledType MangleType(tc::js::ts::Type jtypeRoot, bool bUseTypeAlias = true) noexcept;
