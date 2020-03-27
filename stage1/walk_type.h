#ifndef WALK_TYPE_H_
#define WALK_TYPE_H_

#include <vector>
#include "typescript.d.bootstrap.h"

extern std::vector<tc::js::globals::ts::Symbol> g_vecjsymEnum, g_vecjsymClass;

bool IsEnumInCpp(tc::js::globals::ts::Symbol jsymType) noexcept;

bool IsClassInCpp(tc::js::globals::ts::Symbol jsymType) noexcept;

void WalkType(tc::js::globals::ts::TypeChecker const& jtsTypeChecker, int nOffset, tc::js::globals::ts::Symbol jsymType) noexcept;

#endif  // WALK_TYPE_H_
