#include <emscripten/val.h>
#include <emscripten/bind.h>
#include "explicit_cast.h"
#include "range.h"
#include "range_defines.h"
#include "js_types.h"
#include "js_ref.h"

using tc::js::js_string;
using tc::js::js_undefined;
using tc::js::js_unknown;
using tc::js::js_null;
using tc::js::js_union;
using tc::js::js_ref;
using tc::js::IObject;

void derefUndefinedJsRefGet() {
    js_union<js_undefined, js_ref<IObject>> const u;
	static_cast<void>(u.get<js_ref<IObject>>());
}

void derefNullJsRefGet() {
    js_union<js_null, js_ref<IObject>> const u;
	static_cast<void>(u.get<js_ref<IObject>>());
}

void derefUndefinedStringGet() {
    js_union<js_undefined, js_string> const u;
	static_cast<void>(u.get<js_string>());
}

void derefNullStringGet() {
    js_union<js_null, js_string> const u;
	static_cast<void>(u.get<js_string>());
}

void derefUndefinedDoubleGet() {
    js_union<js_undefined, double> const u;
	static_cast<void>(u.get<double>());
}

void derefNullDoubleGet() {
    js_union<js_null, double> const u;
	static_cast<void>(u.get<double>());
}

void derefUndefinedBoolGet() {
    js_union<js_undefined, bool> const u;
	static_cast<void>(u.get<bool>());
}

void derefNullBoolGet() {
    js_union<js_null, bool> const u;
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
