#include <emscripten/val.h>
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

struct _js_MyJsBase : virtual tc::js::IObject {
    static auto _construct(js_string, js_string) { return emscripten::val::object(); }
};
struct _js_MyJsDerived : virtual _js_MyJsBase {
    static auto _construct(js_string, js_string) { return emscripten::val::object(); }
};
using MyJsBase = tc::js::js_ref<_js_MyJsBase>;
using MyJsDerived = tc::js::js_ref<_js_MyJsDerived>;

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
            // Test explicit construction.
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
        }
        {
            MyJsDerived derived(js_string("foo"), js_string("bar"));
            static_cast<void>(BigUnion(derived));
            BigUnion u = derived;
            _ASSERT(u.getEmval().strictlyEquals(derived.getEmval()));
            _ASSERT(u);
            _ASSERT(MyJsBase{u}.getEmval().strictlyEquals(derived.getEmval()));
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
    {
        using ObjectOrBaseOrString = js_union<js_ref<tc::js::IObject>, MyJsBase, js_string>;
        MyJsBase base(js_string(""), js_string(""));
        MyJsDerived derived(js_string(""), js_string(""));
        js_string str("");

        // Test implicit construction.
        {
            static_cast<void>(ObjectOrBaseOrString(base));
            ObjectOrBaseOrString v = base;
            static_cast<void>(v);
        }
        {
            static_cast<void>(ObjectOrBaseOrString(derived));
            ObjectOrBaseOrString v = derived;
            static_cast<void>(v);
        }
        {
            static_cast<void>(ObjectOrBaseOrString(str));
            ObjectOrBaseOrString v = str;
            static_cast<void>(v);
        }

        // No downcasts in construction unless there is an explicit_cast to exactly one option.
        {
            static_assert(!std::is_constructible<ObjectOrBaseOrString, bool>::value);
            // static_assert(!std::is_constructible<ObjectOrBaseOrString, js_unknown>::value); // Does not compile because of ambiguity.
            static_assert(!std::is_convertible<js_unknown, ObjectOrBaseOrString>::value); // Does not compile.
        }

        // Upcast.
        {
            ObjectOrBaseOrString bod(base);
            static_cast<void>(js_unknown(bod));
            js_unknown unk = bod;
            static_cast<void>(unk);
        }

        // Element extraction.
        static_cast<void>(js_ref<tc::js::IObject>(ObjectOrBaseOrString(base)));
        static_assert(!std::is_convertible<ObjectOrBaseOrString, js_ref<tc::js::IObject>>::value);

        static_cast<void>(MyJsBase(ObjectOrBaseOrString(derived)));
        static_assert(!std::is_convertible<ObjectOrBaseOrString, MyJsBase>::value);

        static_cast<void>(js_string(ObjectOrBaseOrString(str)));
        static_assert(!std::is_convertible<ObjectOrBaseOrString, js_string>::value);

        // No downcasts and crosscasts.
        static_assert(!std::is_constructible<MyJsDerived, ObjectOrBaseOrString>::value);
        static_assert(!std::is_constructible<js_undefined, ObjectOrBaseOrString>::value);
    }
    {
        // Corner case: all options are subtypes of an option.
        using BaseOrDerived = js_union<MyJsBase, MyJsDerived>;
        static_assert(std::is_convertible<MyJsDerived, MyJsBase>::value);
        BaseOrDerived bod(MyJsDerived(js_string(""), js_string("")));

        // Upcast.
        {
            static_cast<void>(MyJsBase(bod));
            MyJsBase base = bod;
            static_cast<void>(base);
        }

        // Element extraction.
        static_assert(!std::is_convertible<BaseOrDerived, MyJsDerived>::value);
        static_cast<void>(MyJsDerived(bod));
    }
    return 0;
}
