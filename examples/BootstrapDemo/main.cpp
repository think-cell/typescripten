#include <emscripten/val.h>
#include <vector>
#include <initializer_list>
#include "explicit_cast.h"
#include "range.h"
#include "range_defines.h"
#include "js_bootstrap.h"
#include "js_callback.h"
#include "js_types.h"

using tc::js::js_string;
using tc::js::js_unknown;
using tc::js::js_optional;
using tc::js::globals::Array;
using tc::js::globals::ReadonlyArray;
using tc::js::globals::console;

enum class MyIntEnum { Foo = 10 };

namespace tc::js {
template<> struct IsJsIntegralEnum<MyIntEnum> : std::true_type {};
} // namespace tc::js

int main() {
    {
        auto message = tc::explicit_cast<js_string>("Hello World");
        _ASSERTEQUAL(message.length(), 11);
        _ASSERTEQUAL(tc::explicit_cast<std::string>(message), "Hello World");
        console()->log(message);

        // Implicit upcast.
        js_unknown anyMessage = message;
        static_assert(std::is_convertible<js_string, js_unknown>::value);
        _ASSERTEQUAL(message.length(), 11);
        _ASSERT(message.getEmval().strictlyEquals(anyMessage.getEmval()));
        anyMessage = message;
        _ASSERTEQUAL(message.length(), 11);
        _ASSERT(message.getEmval().strictlyEquals(anyMessage.getEmval()));

        // Explicit downcast.
        js_string message2(anyMessage);
        static_assert(!std::is_convertible<js_unknown, js_string>::value);
        _ASSERT(message.getEmval().strictlyEquals(message2.getEmval()));
    }

    {
        auto arr = tc::explicit_cast<ReadonlyArray<double>>(std::initializer_list<double>{1, 2, 3});
        console()->log(arr);
        _ASSERTEQUAL(arr->length(), 3);
        _ASSERTEQUAL(arr[0], 1);
        _ASSERTEQUAL(arr[1], 2);
        _ASSERTEQUAL(arr[2], 3);
    }

    {
        Array<js_string> arr(std::initializer_list<char const*>{"Hello", "Hi!"});
        _ASSERTEQUAL(arr->length(), 2);
        _ASSERTEQUAL(arr[0].length(), 5);
        _ASSERTEQUAL(arr[1].length(), 3);
    }

    auto arr = tc::explicit_cast<Array<double>>(std::initializer_list<double>{1, 2, 3});
    console()->log(arr);
    _ASSERTEQUAL(arr->length(), 3);
    _ASSERTEQUAL(arr[1], 2);
    arr->_setIndex(1, 15);
    _ASSERTEQUAL(arr[1], 15);

    {
        std::vector<double> result;
        tc::for_each(arr, [&](double item) { result.push_back(item); });
        _ASSERTEQUAL(result, (std::vector{1.0, 15.0, 3.0}));
    }

    {
        std::vector<double> result;
        tc::for_each(
            tc::filter(
                tc::transform(arr,
                    [](double x) { return x * x; }
                ),
                [](double x) { return x >= 2; }
            ),
            [&](double item) { result.push_back(item); }
        );
        _ASSERTEQUAL(result, (std::vector{225.0, 9.0}));
    }

    {
        emscripten::val emval(MyIntEnum::Foo);
        _ASSERT(emval.isNumber());
        _ASSERTEQUAL(emval.template as<double>(), 10.0);
        _ASSERTEQUAL(emval.template as<MyIntEnum>(), MyIntEnum::Foo);
    }

    {
        emscripten::val emval{tc::js::js_undefined{}};
        _ASSERTE(emval.isUndefined());
        tc::js::js_undefined{emval};
    }

    {
        emscripten::val emval{tc::js::js_null{}};
        _ASSERTE(emval.isNull());
        tc::js::js_null{emval};
    }

    {
        // Test optional
        using OptionalUnknown = js_optional<tc::js::js_ref<tc::js::IObject>>;
        static_assert(
            std::is_same<
                typename emscripten::internal::BindingType<OptionalUnknown>::WireType,
                typename emscripten::internal::BindingType<emscripten::val>::WireType
            >::value);

        {
            emscripten::val emval{OptionalUnknown()};
            _ASSERT(!emval.template as<OptionalUnknown>());
            _ASSERT(emval.isUndefined());
        }
        {
            tc::js::js_ref<tc::js::IObject> jsOrigin(emscripten::val::object());
            emscripten::val emval{OptionalUnknown(jsOrigin)};
            auto ounkParsed = emval.template as<OptionalUnknown>();
            _ASSERT(ounkParsed);
            _ASSERT(ounkParsed->getEmval().strictlyEquals(jsOrigin.getEmval()));
            _ASSERT(!emval.isUndefined());
            _ASSERT(emval.strictlyEquals(jsOrigin.getEmval()));
        }
        {
            OptionalUnknown ounk;
            emscripten::val emval(ounk);
            _ASSERT(!emval.template as<OptionalUnknown>());
            _ASSERT(emval.isUndefined());
        }
    }

    {
        js_optional<double> iValue;
        emscripten::val emval(iValue);
        _ASSERT(emval.isUndefined());
        _ASSERT(!emval.isNumber());
    }
    {
        js_optional<double> iValue(123.5);
        emscripten::val emval(iValue);
        _ASSERT(!emval.isUndefined());
        _ASSERT(emval.isNumber());
        _ASSERT(emval.as<double>() == 123.5);
    }
    return 0;
}
