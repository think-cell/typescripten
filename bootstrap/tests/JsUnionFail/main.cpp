#include <emscripten/val.h>
#include <emscripten/bind.h>
#include "explicit_cast.h"
#include "range.h"
#include "assert_defs.h"
#include "js_types.h"
#include "js_ref.h"

using tc::jst::object;
using tc::js::string;
using tc::js::undefined;
using tc::js::any;
using tc::js::null;
using tc::jst::union_t;
using tc::jst::ref;

void derefUndefinedJsRefGet() noexcept {
	union_t<undefined, object> const u;
	static_cast<void>(u.get<object>());
}

void derefNullJsRefGet() noexcept {
	union_t<null, object> const u;
	static_cast<void>(u.get<object>());
}

void derefUndefinedStringGet() noexcept {
	union_t<undefined, string> const u;
	static_cast<void>(u.get<string>());
}

void derefNullStringGet() noexcept {
	union_t<null, string> const u;
	static_cast<void>(u.get<string>());
}

void derefUndefinedDoubleGet() noexcept {
	union_t<undefined, double> const u;
	static_cast<void>(u.get<double>());
}

void derefNullDoubleGet() noexcept {
	union_t<null, double> const u;
	static_cast<void>(u.get<double>());
}

void derefUndefinedBoolGet() noexcept {
	union_t<undefined, bool> const u;
	static_cast<void>(u.get<bool>());
}

void derefNullBoolGet() noexcept {
	union_t<null, bool> const u;
	static_cast<void>(u.get<bool>());
}

EMSCRIPTEN_BINDINGS(union_fail) {
	emscripten::function("derefUndefinedJsRefGet", &derefUndefinedJsRefGet);
	emscripten::function("derefNullJsRefGet", &derefNullJsRefGet);
	emscripten::function("derefUndefinedStringGet", &derefUndefinedStringGet);
	emscripten::function("derefNullStringGet", &derefNullStringGet);
	emscripten::function("derefUndefinedDoubleGet", &derefUndefinedDoubleGet);
	emscripten::function("derefNullDoubleGet", &derefNullDoubleGet);
	emscripten::function("derefUndefinedBoolGet", &derefUndefinedBoolGet);
	emscripten::function("derefNullBoolGet", &derefNullBoolGet);
}

int main() { return 0; }