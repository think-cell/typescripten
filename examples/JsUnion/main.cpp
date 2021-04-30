#include <emscripten/val.h>
#include "explicit_cast.h"
#include "range.h"
#include "assert_defs.h"
#include "js_types.h"
#include "js_ref.h"

using tc::jst::create_js_object;
using tc::jst::js_object;
using tc::js::string;
using tc::js::undefined;
using tc::jst::js_unknown;
using tc::js::null;
using tc::jst::js_union;
using tc::jst::js_ref;

struct _js_MyJsBase : virtual tc::jst::IObject {
	static auto _tcjs_construct(string, string) noexcept { return emscripten::val::object(); }
};
struct _js_MyJsDerived : virtual _js_MyJsBase {
	static auto _tcjs_construct(string, string) noexcept { return emscripten::val::object(); }
};
using MyJsBase = tc::jst::js_ref<_js_MyJsBase>;
using MyJsDerived = tc::jst::js_ref<_js_MyJsDerived>;

int main() {
	{
		using BigUnion = js_union<undefined, string, double, MyJsBase>;
		{
			BigUnion const u;
			_ASSERT(u.getEmval().isUndefined());
			static_cast<void>(undefined{u});
		}
		{
			BigUnion const u;
			BigUnion v1 = u;
			BigUnion v2(u);
			BigUnion const w1 = tc_move(v1);
			BigUnion const w2(tc_move(v2));
		}
		{
			// Test explicit construction.
			BigUnion const u(tc::aggregate_tag, "foo");
			_ASSERT(u.getEmval().strictlyEquals(emscripten::val("foo")));
			_ASSERTEQUAL(tc::explicit_cast<std::string>(string(u)), "foo");
		}
		{
			static_cast<void>(BigUnion(0.0));
			BigUnion const u = 0.0;
			_ASSERT(u.getEmval().strictlyEquals(emscripten::val(0)));
			_ASSERTEQUAL(double{u}, 0);
		}
		{
			static_cast<void>(BigUnion(123.5));
			BigUnion const u = 123.5;
			_ASSERT(u.getEmval().strictlyEquals(emscripten::val(123.5)));
			_ASSERTEQUAL(double{u}, 123.5);
		}
		{
			MyJsBase const base(create_js_object, string("foo"), string("bar"));
			static_cast<void>(BigUnion(base));
			BigUnion const u = base;
			_ASSERT(u.getEmval().strictlyEquals(base.getEmval()));
			_ASSERT(MyJsBase{u}.getEmval().strictlyEquals(base.getEmval()));
		}
		{
			MyJsDerived const derived(create_js_object, string("foo"), string("bar"));
			static_cast<void>(BigUnion(derived));
			BigUnion const u = derived;
			_ASSERT(u.getEmval().strictlyEquals(derived.getEmval()));
			_ASSERT(MyJsBase{u}.getEmval().strictlyEquals(derived.getEmval()));
		}
	}
	{
		using BigUnion = js_union<null, bool>;
		{
			BigUnion const u;
			_ASSERT(u.getEmval().isNull());
			static_cast<void>(null{u});
			static_cast<void>(u.get<null>());
		}
		{
			BigUnion const u(tc::aggregate_tag, false);
			_ASSERT(!u.getEmval().isNull());
			_ASSERT(u.getEmval().isFalse());
			_ASSERT(!u.get<bool>());
		}
		{
			BigUnion const u(tc::aggregate_tag, true);
			_ASSERT(!u.getEmval().isNull());
			_ASSERT(u.getEmval().isTrue());
			_ASSERT(u.get<bool>());
		}
	}
	{
		using BigUnion = js_union<null, string>;
		{
			BigUnion const u;
			_ASSERT(u.getEmval().isNull());
			static_cast<void>(null{u});
		}
		{
			BigUnion const u(tc::aggregate_tag, "foo");
			_ASSERT(u.getEmval().strictlyEquals(emscripten::val("foo")));
			_ASSERTEQUAL(tc::explicit_cast<std::string>(string(u)), "foo");
		}
	}
	{
		using ObjectOrBaseOrString = js_union<js_object, MyJsBase, string>;
		MyJsBase const base(create_js_object, string(""), string(""));
		MyJsDerived const derived(create_js_object, string(""), string(""));
		string const str("");

		// Test implicit construction.
		{
			static_cast<void>(ObjectOrBaseOrString(base));
			ObjectOrBaseOrString const v = base;
			static_cast<void>(v);
		}
		{
			static_cast<void>(ObjectOrBaseOrString(derived));
			ObjectOrBaseOrString const v = derived;
			static_cast<void>(v);
		}
		{
			static_cast<void>(ObjectOrBaseOrString(str));
			ObjectOrBaseOrString const v = str;
			static_cast<void>(v);
		}

		// No derivedcasts in construction unless there is an explicit_cast to exactly one option.
		{
			static_assert(!std::is_constructible<ObjectOrBaseOrString, bool>::value);
			// static_assert(!std::is_constructible<ObjectOrBaseOrString, js_unknown>::value); // Does not compile because of ambiguity.
			static_assert(!std::is_convertible<js_unknown, ObjectOrBaseOrString>::value); // Does not compile.
		}

		// Basecast.
		{
			ObjectOrBaseOrString const bod(base);
			static_cast<void>(js_unknown(bod));
			js_unknown const unk = bod;
			static_cast<void>(unk);
		}

		// Element extraction.
		static_cast<void>(js_object(ObjectOrBaseOrString(base)));
		static_assert(!std::is_convertible<ObjectOrBaseOrString, js_object>::value);

		static_cast<void>(MyJsBase(ObjectOrBaseOrString(derived)));
		static_assert(!std::is_convertible<ObjectOrBaseOrString, MyJsBase>::value);

		static_cast<void>(string(ObjectOrBaseOrString(str)));
		static_assert(!std::is_convertible<ObjectOrBaseOrString, string>::value);

		// No derivedcasts and crosscasts.
		static_assert(!std::is_constructible<MyJsDerived, ObjectOrBaseOrString>::value);
		static_assert(!std::is_constructible<undefined, ObjectOrBaseOrString>::value);
	}
	{
		// Corner case: all options are subtypes of an option.
		using BaseOrDerived = js_union<MyJsBase, MyJsDerived>;
		static_assert(std::is_convertible<MyJsDerived, MyJsBase>::value);
		BaseOrDerived const bod(MyJsDerived(create_js_object, string(""), string("")));

		// Basecast.
		{
			static_cast<void>(MyJsBase(bod));
			MyJsBase const base = bod;
			static_cast<void>(base);
		}

		// Element extraction.
		static_assert(!std::is_convertible<BaseOrDerived, MyJsDerived>::value);
		static_cast<void>(MyJsDerived(bod));
	}
	{
		// Union basecast.
		{
			static_cast<void>(js_union<MyJsBase, string, undefined, null>(js_union<MyJsDerived, undefined>()));
			js_union<MyJsBase, string, undefined, null> const x = js_union<MyJsDerived, undefined>();
			static_cast<void>(x);
		}

		// Union derivedcast.
		static_assert(!std::is_convertible<
			js_union<MyJsBase, string, undefined>,
			js_union<MyJsDerived, string>
		>::value);
		static_cast<void>(js_union<MyJsDerived, undefined>(js_union<MyJsBase, string, undefined>()));

		// Union crosscasts are prohibited.
		static_assert(!std::is_constructible<
			js_union<MyJsDerived, undefined, null>, // null is removed, but MyJsDerived is generalized.
			js_union<MyJsBase, undefined>
		>::value);
		static_assert(!std::is_constructible<
			js_union<MyJsBase, undefined>,
			js_union<MyJsDerived, undefined, null> // null is removed, but MyJsDerived is generalized.
		>::value);
	}
	{
		js_union<undefined, MyJsBase, MyJsDerived> const u;
		static_cast<void>(u.get<undefined>());
		static_cast<void>(undefined{u});
		_ASSERT(!u);
	}
	{
		js_union<undefined, MyJsBase, MyJsDerived> const u(MyJsBase(create_js_object, string(""), string("")));
		static_cast<void>(u.get<MyJsBase>());
		static_cast<void>(MyJsBase{u});
		_ASSERT(u);
	}
	return 0;
}
