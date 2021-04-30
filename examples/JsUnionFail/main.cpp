#include <emscripten/val.h>
#include <emscripten/bind.h>
#include "explicit_cast.h"
#include "range.h"
#include "assert_defs.h"
#include "js_types.h"
#include "js_ref.h"

using tc::jst::js_object;
using tc::js::string;
using tc::js::undefined;
using tc::jst::js_unknown;
using tc::js::null;
using tc::jst::js_union;
using tc::jst::js_ref;

void derefUndefinedJsRefGet() noexcept {
	js_union<undefined, js_object> const u;
	static_cast<void>(u.get<js_object>());
}

void derefNullJsRefGet() noexcept {
	js_union<null, js_object> const u;
	static_cast<void>(u.get<js_object>());
}

void derefUndefinedStringGet() noexcept {
	js_union<undefined, string> const u;
	static_cast<void>(u.get<string>());
}

void derefNullStringGet() noexcept {
	js_union<null, string> const u;
	static_cast<void>(u.get<string>());
}

void derefUndefinedDoubleGet() noexcept {
	js_union<undefined, double> const u;
	static_cast<void>(u.get<double>());
}

void derefNullDoubleGet() noexcept {
	js_union<null, double> const u;
	static_cast<void>(u.get<double>());
}

void derefUndefinedBoolGet() noexcept {
	js_union<undefined, bool> const u;
	static_cast<void>(u.get<bool>());
}

void derefNullBoolGet() noexcept {
	js_union<null, bool> const u;
	static_cast<void>(u.get<bool>());
}

EMSCRIPTEN_BINDINGS(js_union_fail) {
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