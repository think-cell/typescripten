#ifndef MANGLE_H_
#define MANGLE_H_

#include <string>
#include <utility>
#include "typescript.d.bootstrap.h"

extern std::unordered_set<std::string> g_usstrAllowedMangledTypes;

std::string MangleSymbolName(tc::js::globals::ts::TypeChecker const& jtsTypeChecker, tc::js::globals::ts::Symbol jsymType) noexcept;

DEFINE_ADL_TAG_TYPE(mangled_no_comments)

struct SMangledType {
    std::string m_strWithComments;
    std::string m_strCppCanonized;

    SMangledType(mangled_no_comments_t, std::string strWithComments)
        : m_strWithComments(std::move(strWithComments))
        , m_strCppCanonized(m_strWithComments)
    {}

    SMangledType(std::string strWithComments, std::string strCppCanonized)
        : m_strWithComments(std::move(strWithComments))
        , m_strCppCanonized(std::move(strCppCanonized))
    {}
};

SMangledType MangleType(tc::js::globals::ts::TypeChecker jtsTypeChecker, tc::js::globals::ts::Type jtypeRoot) noexcept;

#endif // MANGLE_H_
