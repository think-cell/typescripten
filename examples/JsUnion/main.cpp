#include <emscripten/val.h>
#include "explicit_cast.h"
#include "range.h"
#include "range_defines.h"
#include "js_types.h"
#include "js_ref.h"

using tc::js::js_string;
using tc::js::js_undefined;
using tc::js::js_null;
using tc::js::js_union;
using tc::js::js_ref;

struct _js_MyJsBase : virtual tc::js::IObject {
    static auto _construct(js_string, js_string) { return emscripten::val::object(); }
};
struct _js_MyJsDerived : virtual _js_MyJsBase {
    static auto _construct(js_string, js_string) { return emscripten::val::object(); }
};
using MyJsBase = tc::js::js_ref<_js_MyJsBase>;
using MyJsDerived = tc::js::js_ref<_js_MyJsDerived>;

static_assert(!std::is_constructible<MyJsBase, double>::value);
static_assert(std::is_constructible<double, double>::value);
static_assert(std::is_convertible<double, double>::value);
static_assert(!tc::js::js_union_detail::no_adl::ExplicitlyConstructibleFrom<double&&>::template type<js_string>::value);
static_assert(!tc::js::js_union_detail::HasUniqueExplicitlyConstructibleFrom<double&&, tc::type::list<js_undefined, js_string, double, MyJsBase>>::value);

int main() {
    {
        using BigUnion = js_union<js_undefined, js_string, double, MyJsBase>;
        {
            BigUnion u;
            _ASSERT(u.getEmval().isUndefined());
            _ASSERT(!u);
            static_cast<void>(js_undefined{u});
        }
        {
            BigUnion u("foo");
            _ASSERT(u.getEmval().strictlyEquals(emscripten::val("foo")));
            _ASSERT(u);
            _ASSERTEQUAL(std::string(js_string(u)), "foo");
        }
        {
            static_cast<void>(BigUnion(0.0));
            BigUnion u = 0.0;
            _ASSERT(u.getEmval().strictlyEquals(emscripten::val(0)));
            _ASSERT(!u);
            _ASSERTEQUAL(double{u}, 0);
        }
        {
            static_cast<void>(BigUnion(123.5));
            BigUnion u = 123.5;
            _ASSERT(u.getEmval().strictlyEquals(emscripten::val(123.5)));
            _ASSERT(u);
            _ASSERTEQUAL(double{u}, 123.5);
        }
        {
            MyJsBase base(js_string("foo"), js_string("bar"));
            static_cast<void>(BigUnion(base));
            BigUnion u = base;
            _ASSERT(u.getEmval().strictlyEquals(base.getEmval()));
            _ASSERT(u);
            _ASSERT(MyJsBase{u}.getEmval().strictlyEquals(base.getEmval()));
            _ASSERT(js_ref<tc::js::IObject>{u}.getEmval().strictlyEquals(base.getEmval()));
        }
        {
            MyJsDerived derived(js_string("foo"), js_string("bar"));
            static_cast<void>(BigUnion(derived));
            BigUnion u = derived;
            _ASSERT(u.getEmval().strictlyEquals(derived.getEmval()));
            _ASSERT(u);
            _ASSERT(MyJsBase{u}.getEmval().strictlyEquals(derived.getEmval()));
            _ASSERT(js_ref<tc::js::IObject>{u}.getEmval().strictlyEquals(derived.getEmval()));
        }
    }
    {
        using BigUnion = js_union<js_null, js_string>;
        {
            BigUnion u;
            _ASSERT(u.getEmval().isNull());
            _ASSERT(!u);
            static_cast<void>(js_null{u});
        }
        {
            BigUnion u("foo");
            _ASSERT(u.getEmval().strictlyEquals(emscripten::val("foo")));
            _ASSERT(u);
            _ASSERTEQUAL(std::string(js_string(u)), "foo");
        }
    }
    return 0;
}
