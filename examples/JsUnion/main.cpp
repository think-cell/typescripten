#include <emscripten/val.h>
#include "explicit_cast.h"
#include "range.h"
#include "assert_defs.h"
#include "js_types.h"
#include "js_ref.h"

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

using MyJsBase = jst::js_ref<_js_MyJsBase>;
using MyJsDerived = jst::js_ref<_js_MyJsDerived>;
using MyJsDerived2 = jst::js_ref<_js_MyJsDerived2>;

int main() {
	using UnionUndefined = jst::union_t<js::undefined, js::string, double, MyJsBase>;
	using UnionNull = jst::union_t<js::null, js::string, bool>;
	using UnionBoth = jst::union_t<js::null, js::undefined, js::string, double, MyJsBase, bool>; // = UnionUndefined | UnionNull
	
	static_assert(std::is_default_constructible<UnionUndefined>::value);
	static_assert(std::is_nothrow_default_constructible<UnionUndefined>::value);
	static_assert(std::is_default_constructible<UnionNull>::value);
	static_assert(std::is_nothrow_default_constructible<UnionNull>::value);
	
	static_assert(!std::is_default_constructible<UnionBoth>::value);
	{
		UnionUndefined const u;
		_ASSERT(u.getEmval().isUndefined());

		UnionNull const u2;
		_ASSERT(u2.getEmval().isNull());
	}

	static_assert(std::is_copy_assignable<UnionUndefined>::value);
	static_assert(std::is_copy_constructible<UnionUndefined>::value);
	static_assert(std::is_move_constructible<UnionUndefined>::value);
	static_assert(std::is_move_assignable<UnionUndefined>::value);
	{
		UnionUndefined const u;
		UnionUndefined v1 = u;
		UnionUndefined v2(u);
		UnionUndefined const w1 = tc_move(v1);
		UnionUndefined const w2(tc_move(v2));
	}
	// Unrelated union types cannot be constructed from each other
	static_assert(!std::is_constructible<UnionNull, UnionUndefined>::value);
	static_assert(!std::is_constructible<UnionUndefined, UnionNull>::value);
	
	static_assert(std::is_constructible<UnionUndefined, double>::value);
	static_assert(!std::is_constructible<UnionUndefined, int>::value);
	static_assert(std::is_constructible<UnionUndefined, js::string>::value);
	static_assert(std::is_constructible<UnionUndefined, MyJsBase>::value);
	static_assert(std::is_constructible<UnionUndefined, MyJsDerived>::value);

	static_assert(!tc::jst::no_adl::is_safely_js_convertible<double, bool>::value);
	static_assert(!tc::jst::no_adl::is_safely_js_convertible<bool, double>::value);	
	static_assert(!tc::jst::no_adl::is_safely_js_convertible<double, js::string>::value);
	static_assert(!tc::jst::no_adl::is_safely_js_convertible<double, js::null>::value);
	static_assert(!std::is_constructible<UnionNull, double>::value); // No double -> bool conversion! 

	static_assert(std::is_convertible<double, UnionUndefined>::value);
	static_assert(std::is_convertible<js::string, UnionUndefined>::value);
	static_assert(std::is_convertible<MyJsBase, UnionUndefined>::value);
	static_assert(std::is_convertible<MyJsDerived, UnionUndefined>::value);

	{
		UnionUndefined const u(0.0);
		UnionUndefined const v = 0.0;

		_ASSERT(u.getEmval().strictlyEquals(emscripten::val(0)));
		_ASSERTEQUAL(u.get<double>(), 0);
	}
	{
		MyJsBase const base(jst::create_js_object, js::string("foo"), js::string("bar"));
		UnionUndefined const u = base;
		_ASSERT(u.getEmval().strictlyEquals(base.getEmval()));
		_ASSERT(u.get<MyJsBase>().getEmval().strictlyEquals(base.getEmval()));
	}
	{
		MyJsDerived const derived(jst::create_js_object, js::string("foo"), js::string("bar"));
		UnionUndefined const u = derived;
		_ASSERT(MyJsDerived(u.get<MyJsBase>()).getEmval().strictlyEquals(derived.getEmval()));
	}
	
	// Aggregate construction 
	{
		UnionUndefined const u(tc::aggregate_tag, "foo");
		_ASSERT(u.getEmval().strictlyEquals(emscripten::val("foo")));
		_ASSERTEQUAL(tc::explicit_cast<std::string>(u.get<js::string>()), "foo");

		UnionNull const u2(tc::aggregate_tag, false);
	}

	// Implicit conversion to wider union (permitting conversion to base class)
	static_assert(std::is_convertible<UnionUndefined, UnionBoth>::value);
	static_assert(std::is_convertible<UnionNull, UnionBoth>::value);
	static_assert(std::is_convertible<jst::union_t<js::undefined, js::string, MyJsDerived>, UnionBoth>::value);
	{
		UnionUndefined u;
		UnionBoth u1 = u;
	}
	
	// Implicit conversion to narrower union must not be allowed
	static_assert(!std::is_convertible<UnionBoth, UnionUndefined>::value);
	static_assert(!std::is_convertible<UnionBoth, UnionNull>::value);

	// At the moment, no explicit conversion either
	static_assert(!std::is_constructible<UnionUndefined, UnionBoth>::value);
	static_assert(!std::is_constructible<UnionNull, UnionBoth>::value);
		
	// No explicit or implicit conversion operators. Use get() accessor.
	static_assert(!std::is_constructible<js::string, UnionUndefined>::value);
	static_assert(!std::is_constructible<double, UnionUndefined>::value);
	static_assert(!std::is_constructible<MyJsBase, UnionUndefined>::value);

	// Explicit cast to common base type
	using UnionDerived = jst::union_t<MyJsDerived, MyJsDerived2>;
	static_assert(std::is_convertible<UnionDerived, MyJsBase>::value);
	{
		UnionDerived u(MyJsDerived(jst::create_js_object, js::string("foo"), js::string("bar")));
		MyJsBase b = u;
	}
	return 0;
}
