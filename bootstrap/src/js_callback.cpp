#include <emscripten/bind.h>
#include "js_callback.h"

namespace tc::js_types {
namespace callback_detail {
/**
 * [basic.compound] 6.7.2
 * (3) The type of a pointer to cv void or a pointer to an object type is called an object pointer type. ...
 *     ... The type of a pointer that can designate a function is called a function pointer type. A pointer to
 *     objects of type T is referred to as a “pointer to T”. ... Except for pointers to static members, text
 *     referring to “pointers” does not apply to pointers to members.
 *
 * [expr.reinterpret.cast] 8.5.1.10
 * (1) ... Conversions that can be performed explicitly using reinterpret_cast are listed below. ...
 * (4) A pointer can be explicitly converted to any integral type large enough to hold it. The mapping function is
 *     implementation-defined. ...
 * (5) A value of integral type or enumeration type can be explicitly converted to a pointer. A pointer converted to
 *     an integer of sufficient size (if any such exists on the implementation) and back to the same pointer type will
 *     have its original value; mappings between pointers and integers are otherwise implementation-defined. [ Note:
 *     Except as described in 6.6.4.4.3, the result of such a conversion will not be a safely-derived pointer value.
 *     — end note ]
 *
 * Hence, I conclude that converting between std::uintptr_t and non-member/static function pointers via reinterpret_cast
 * is safe as long as PointerNumber is "large enough", which is _hopefully_ checked by sizeof().
 * We cannot guarantee that pointers are safely-derived unless emscripten passes us integer representation of
 * a safely-derived pointer, so we require relaxed pointer safety.
 */
static_assert(sizeof(std::intptr_t) <= sizeof(PointerNumber));
static_assert(sizeof(FunctionPointer) <= sizeof(PointerNumber), "PointerNumber is not large enough to hold FunctionPointer, cannot pass it to JavaScript");
static_assert(sizeof(FirstArgument) <= sizeof(PointerNumber), "PointerNumber is not large enough to hold FirstArgument, cannot pass it to JavaScript");

emscripten::val Call(PointerNumber iFunctionPtr, PointerNumber iArg0, emscripten::val emvalThis, emscripten::val emvalArgs) noexcept {
	return reinterpret_cast<FunctionPointer>(iFunctionPtr)(
		reinterpret_cast<FirstArgument>(iArg0),
		emvalThis,
		emvalArgs
	);
}

EMSCRIPTEN_BINDINGS(tc_js_callback_detail_bind) {
	emscripten::function("tc_js_callback_detail_js_Call", &Call);
}

} // namespace callback_detail
} // namespace tc::js_types
