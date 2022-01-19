#include <emscripten/val.h>
#include <tc/explicit_cast.h>
#include <tc/range.h>
#include <tc/assert_defs.h>
#include <typescripten/typescripten.h>

namespace js = tc::js;
namespace jst = tc::jst;

struct _js_MyJsBase : virtual jst::object_base {
	static auto _tcjs_construct(js::string, js::string) noexcept { return emscripten::val::object(); }
};
struct _js_MyJsDerived : virtual _js_MyJsBase {
	static auto _tcjs_construct(js::string, js::string) noexcept { return emscripten::val::object(); }
};
struct _js_MyJsDerived2 : virtual _js_MyJsBase {
	static auto _tcjs_construct(js::string, js::string) noexcept { return emscripten::val::object(); }
};

using MyJsBase = jst::ref<_js_MyJsBase>;
using MyJsDerived = jst::ref<_js_MyJsDerived>;
using MyJsDerived2 = jst::ref<_js_MyJsDerived2>;

int main() {
	// Pre: Repeat asserts from JsUnion test
	using UnionUndefined = jst::union_t<js::undefined, js::string, double, MyJsBase>;
	using UnionNull = jst::union_t<js::null, js::string, bool>;
	using UnionBoth = jst::union_t<js::null, js::undefined, js::string, double, MyJsBase, bool>; // = UnionUndefined | UnionNull

	// Conversion from element to union	
	static_assert(std::is_convertible<double, UnionUndefined>::value);
	static_assert(std::is_convertible<js::string, UnionUndefined>::value);
	static_assert(std::is_convertible<MyJsBase, UnionUndefined>::value);
	static_assert(std::is_convertible<MyJsDerived, UnionUndefined>::value);

	// Conversion between compatible union types
	static_assert(std::is_convertible<UnionUndefined, UnionBoth>::value);
	static_assert(std::is_convertible<UnionNull, UnionBoth>::value);
	static_assert(std::is_convertible<jst::union_t<js::undefined, js::string, MyJsDerived>, UnionBoth>::value);
	
	// Implicit conversion to narrower union must not be allowed
	static_assert(!std::is_convertible<UnionBoth, UnionUndefined>::value);
	static_assert(!std::is_convertible<UnionBoth, UnionNull>::value);

	using UnionDerived = jst::union_t<MyJsDerived, MyJsDerived2>;
	static_assert(std::is_convertible<UnionDerived, MyJsBase>::value);
	
	// Function co-variance and contra-variance
	static_assert(std::is_convertible<jst::function<double(MyJsBase)>, jst::function<UnionUndefined(MyJsDerived)>>::value);
	static_assert(!std::is_convertible<jst::function<double(MyJsDerived)>, jst::function<UnionUndefined(MyJsBase)>>::value);
	static_assert(!std::is_convertible<jst::function<UnionUndefined(MyJsBase)>, jst::function<double(MyJsDerived)>>::value);

	static_assert(std::is_convertible<jst::function<MyJsBase(MyJsBase, MyJsDerived, MyJsBase)>, jst::function<UnionUndefined(MyJsDerived, MyJsDerived, MyJsDerived2)>>::value);
	static_assert(!std::is_convertible<jst::function<MyJsBase(MyJsBase, MyJsDerived, MyJsBase)>, jst::function<UnionUndefined(MyJsDerived, MyJsDerived2, MyJsDerived2)>>::value);
	static_assert(std::is_convertible<jst::function<MyJsDerived(MyJsBase, MyJsDerived, MyJsBase)>, jst::function<UnionUndefined(MyJsDerived, MyJsDerived, MyJsDerived2)>>::value);
	static_assert(!std::is_convertible<jst::function<UnionBoth(MyJsBase, MyJsDerived, MyJsBase)>, jst::function<UnionUndefined(MyJsDerived, MyJsDerived, MyJsDerived2)>>::value);
	return 0;
}
