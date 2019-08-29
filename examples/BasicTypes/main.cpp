#include <emscripten/val.h>
#include <type_traits>
#include "explicit_cast.h"
#include "range.h"
#include "range_defines.h"
#include "js_types.h"
#include "js_ref.h"

using tc::js::js_string;
using tc::js::js_unknown;
using tc::js::js_optional;

int main() {
    {
        auto message = tc::explicit_cast<js_string>("Hello World");
        static_assert(!std::is_constructible<js_string, double>::value);
        _ASSERTEQUAL(message.length(), 11);
        _ASSERTEQUAL(tc::explicit_cast<std::string>(message), "Hello World");

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
        // Test optional of object.
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
        // Test optional of double.
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
    }
    return 0;
}
