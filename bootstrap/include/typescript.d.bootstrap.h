#pragma once

#include <emscripten/val.h>
#include <string>
#include "js_types.h"
#include "js_callback.h"

namespace tc::js::globals {

namespace no_adl {
struct ts : virtual tc::js::IJsBase {
};
} // namespace no_adl

using no_adl::Array;
using no_adl::String;
using no_adl::Console;

inline auto ts() { return js_ref<ts>(emscripten::val::global("ts")); }

} // namespace tc::js::globals
