#ifndef MANGLE_H_
#define MANGLE_H_

#include <string>
#include "typescript.d.bootstrap.h"

extern std::unordered_set<std::string> g_usstrAllowedMangledTypes;

std::string MangleSymbolName(tc::js::globals::ts::TypeChecker const& jtsTypeChecker, tc::js::globals::ts::Symbol jsymType) noexcept;

std::string MangleType(tc::js::globals::ts::TypeChecker jtsTypeChecker, tc::js::globals::ts::Type jtypeRoot) noexcept;

#endif // MANGLE_H_
