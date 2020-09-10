#ifndef MANGLE_H_
#define MANGLE_H_

#include <string>
#include <utility>
#include "typescript.d.bootstrap.h"

extern std::unordered_set<std::string> g_usstrAllowedMangledTypes;

std::string MangleSymbolName(tc::js::ts::TypeChecker const& jtsTypeChecker, tc::js::ts::Symbol jsymType) noexcept;
std::string CppifyName(tc::js::ts::Symbol jsymSymbol) noexcept;

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

SMangledType MangleType(tc::js::ts::TypeChecker jtsTypeChecker, tc::js::ts::Type jtypeRoot, bool bUseTypeAlias = true) noexcept;

#endif // MANGLE_H_
