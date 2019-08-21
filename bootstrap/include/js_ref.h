#pragma once

#include <emscripten/val.h>
#include <emscripten/wire.h>
#include <string>
#include <type_traits>
#include "type_traits.h"
#include "type_list.h"
#include "tc_move.h"
#include "range_defines.h"
#include "noncopyable.h"

namespace tc::js {
namespace no_adl {
template<typename, typename = void>
struct IsJsInteropable : std::false_type {};
} // namespace no_adl
using no_adl::IsJsInteropable;

namespace property_proxy_detail {
namespace no_adl {
template<typename T, typename Name>
struct CPropertyProxy : tc::nonmovable {
    static_assert(IsJsInteropable<T>::value);

    CPropertyProxy(emscripten::val&& m_emval, Name m_name)
        : m_emval(tc_move(m_emval))
        , m_name(tc_move(m_name)) {
    }

    // TODO: it's still possible to use incorrectly:
    // auto x = foo["bar"]; // Presumably copy.
    // foo["bar"] = undefined;
    // baz(tc_move(x)); // 'Moving' value to baz.
    operator T() && { return m_emval[m_name].template as<T>(); }
    T const& operator=(T const& value) && {
        m_emval.set(m_name, emscripten::val(value));
        return value;
    }

private:
    emscripten::val m_emval;
    Name m_name;
};
} // namespace no_adl
using no_adl::CPropertyProxy;
} // namespace property_proxy_detail

namespace no_adl {
struct IUnknown {
private:
    emscripten::val m_emval;

protected:
    explicit IUnknown(emscripten::val const& m_emval) noexcept : m_emval(m_emval) {}
    explicit IUnknown(emscripten::val&& m_emval) noexcept : m_emval(tc_move(m_emval)) {}
    explicit IUnknown() : m_emval(emscripten::val::undefined()) {
        // Should never be called.
        _ASSERTFALSE;
    }

    template<typename T, typename Name>
    T _getProperty(Name&& name) {
        static_assert(IsJsInteropable<T>::value);
        return m_emval[std::forward<Name>(name)].template as<T>();
    }

    template<typename T, typename Name>
    void _setProperty(Name&& name, T const& value) {
        static_assert(IsJsInteropable<T>::value);
        m_emval.set(std::forward<Name>(name), value);
    }

    template<typename R, typename... Args>
    R _call(char const* name, Args&&... args) {
        static_assert(IsJsInteropable<R>::value);
        static_assert(std::conjunction<IsJsInteropable<tc::remove_cvref_t<Args>>...>::value);
        return m_emval.call<R>(name, std::forward<Args>(args)...);
    }

    template<typename R, typename... Args>
    R _call_this(Args&&... args) {
        static_assert(IsJsInteropable<R>::value);
        static_assert(std::conjunction<IsJsInteropable<tc::remove_cvref_t<Args>>...>::value);
        return m_emval(std::forward<Args>(args)...).template as<R>();
    }

    template<typename Property, typename Name>
    auto _propertyProxy(Name name) && {
        static_assert(IsJsInteropable<Property>::value);
        return tc::js::property_proxy_detail::CPropertyProxy<Property, Name>(tc_move(m_emval), tc_move(name));
    }

    emscripten::val& _getEmval() {
        return m_emval;
    }

    // Make sure the class and its descendants are abstract.
    virtual void __IUnknown_and_derived_are_abstract_Use_js_ref_instead() = 0;
};

// Non-final, but non-polymorphic as well. Derive with care.
template<typename T>
struct js_ref {
    static_assert(std::is_class<T>::value);  // void is explicitly excluded as well, even though void* is base of all pointers.
    static_assert(!std::is_volatile<T>::value);
    static_assert(!std::is_const<T>::value, "We cannot guarantee constness of JS values");
    static_assert(std::is_convertible<T*, IUnknown*>::value);

    // js_ref is non-nullable.
    explicit js_ref(emscripten::val const& m_emval) noexcept : m_emval(m_emval) {
        _ASSERT(!m_emval.isUndefined() && !m_emval.isNull());
    }
    explicit js_ref(emscripten::val&& m_emval) noexcept : m_emval(tc_move(m_emval)) {
        _ASSERT(!m_emval.isUndefined() && !m_emval.isNull());
    }

    template<typename Arg, typename = std::enable_if_t<
        !tc::is_instance<js_ref, tc::remove_cvref_t<Arg>>::value &&
        !std::is_same<emscripten::val, tc::remove_cvref_t<Arg>>::value
    >>
    explicit js_ref(Arg&& arg) noexcept : js_ref(T::_construct(std::forward<Arg>(arg))) {}

    template<typename... Args, typename = std::enable_if_t<sizeof...(Args) != 1>>
    explicit js_ref(Args&&... args) noexcept : js_ref(T::_construct(std::forward<Args>(args)...)) {}

    /**
     * Upcasting.
     * [util.smartptr.shared] says:
     * 23.11.3 For the purposes of subclause 23.11, a pointer type Y* is said to be compatible with a pointer type T* when either Y* is convertible to T* or Y is U[N] and T is cv U[].
     * 23.11.3.1
     * shared_ptr(const shared_ptr& r) noexcept;
     * template<class Y> shared_ptr(const shared_ptr<Y>& r) noexcept;
     * 18. Remarks: The second constructor shall not participate in overload resolution unless Y* is compatible with T*.
     *
     * We do not allow arrays, so this sounds like std::is_convertible<> (although it's not defined via the word 'convertible').
     */
    template<typename U, std::enable_if_t<std::is_convertible<U*, T*>::value>* = nullptr>
    js_ref(js_ref<U> const& jsOther) noexcept : js_ref(jsOther.m_emval) {}

    template<typename U, std::enable_if_t<std::is_convertible<U*, T*>::value>* = nullptr>
    js_ref(js_ref<U>&& jsOther) noexcept : js_ref(tc_move(jsOther.m_emval)) {}

    template<typename U, typename = std::enable_if_t<std::is_convertible<U*, T*>::value>>
    js_ref& operator=(js_ref<U> const& jsOther) noexcept { m_emval = jsOther.m_emval; return *this; }

    template<typename U, typename = std::enable_if_t<std::is_convertible<U*, T*>::value>>
    js_ref& operator=(js_ref<U>&& jsOther) noexcept { m_emval = tc_move(jsOther.m_emval); return *this; }

    /**
     * Downcasting.
     * Although shared_ptr has dedicated static_pointer_cast, we decided to create
     * explicit constructors instead, as it allows deep explicit creation (e.g.
     * container.emplace() will work with explicit constructor, but not static_pointer_cast).
     *
     * No type checking is performed on JS side.
     */
    template<typename U, std::enable_if_t<std::is_convertible<T*, U*>::value>* = nullptr>
    explicit js_ref(js_ref<U> const& jsOther) noexcept : js_ref(jsOther.m_emval) {}

    template<typename U, std::enable_if_t<std::is_convertible<T*, U*>::value>* = nullptr>
    explicit js_ref(js_ref<U>&& jsOther) noexcept : js_ref(tc_move(jsOther.m_emval)) {}

    emscripten::val getEmval() const& noexcept { return m_emval; }
    emscripten::val getEmval() && noexcept { return tc_move(m_emval); }

    template<typename... Args, typename = decltype(std::declval<T>()(std::forward<Args>(std::declval<Args>())...))>
    auto operator()(Args&&... args) const& noexcept {
        return CArrowProxy(m_emval)(std::forward<Args>(args)...);
    }

    template<typename Index, typename = decltype(std::declval<T>()[std::forward<Index>(std::declval<Index>())])>
    auto operator[](Index&& index) const& noexcept {
        return CArrowProxy(m_emval)[std::forward<Index>(index)];
    }

    template<typename U, typename = decltype(U(std::declval<T>()))>
    explicit operator U() const& noexcept {
        return U(CArrowProxy(m_emval));
    }

private:
    emscripten::val m_emval;

    friend struct js_ref;

    struct CArrowProxy final : T, tc::nonmovable {
        explicit CArrowProxy(emscripten::val const& m_emval) noexcept : IUnknown(m_emval) {}
        T* operator->() && noexcept { return this; }
    private:
        void __IUnknown_and_derived_are_abstract_Use_js_ref_instead() override {
            // Should never be called.
            assert(false);
        }
    };

public:
    CArrowProxy operator->() const& noexcept { return CArrowProxy(m_emval); }
};
} // namespace no_adl
using no_adl::IUnknown;
using no_adl::js_ref;

namespace is_js_ref_detail {
namespace no_adl {
template<typename T>
struct JsRefFound : std::true_type {
    using element_type = T;
};
} // namespace no_adl
using no_adl::JsRefFound;
template<typename T> JsRefFound<T> is_js_ref_try_convert(js_ref<T>);
std::false_type is_js_ref_try_convert(...);

template<typename T>
using IsJsRef = std::conjunction<
    tc::is_decayed<T>,
    decltype(is_js_ref_try_convert(std::declval<T>()))  // TODO: explicitly specify namespace to avoid ADL?
>;
} // namespace is_js_ref_detail

namespace no_adl {
template<typename T>
struct IsJsInteropable<T, std::enable_if_t<is_js_ref_detail::IsJsRef<T>::value>> : std::true_type {};

template<typename T>
struct IsJsInteropable<
    T,
    std::enable_if_t<tc::type::find_unique<
        tc::type::list<
            void,
            char, signed char, unsigned char,
            signed short, unsigned short,
            signed int, unsigned int,
            signed long, unsigned long,
            float, double,
            bool,
            emscripten::val,
            std::string
        >,
        T
    >::found>
> : std::true_type {};
} // namespace no_adl
} // namespace tc::js

// Custom marshalling
namespace emscripten::internal {
    template<typename T>
    struct TypeID<T, std::enable_if_t<tc::js::is_js_ref_detail::IsJsRef<tc::remove_cvref_t<T>>::value>> {
        static constexpr TYPEID get() {
            return TypeID<val>::get();
        }
    };

    template<typename T>
    struct BindingType<T, std::enable_if_t<tc::js::is_js_ref_detail::IsJsRef<T>::value>> {
        typedef typename BindingType<emscripten::val>::WireType WireType;

        static WireType toWireType(T const& js) {
            return BindingType<emscripten::val>::toWireType(js.getEmval());
        }

        static auto fromWireType(WireType v) {
            return tc::js::js_ref<typename tc::js::is_js_ref_detail::IsJsRef<T>::element_type>(BindingType<emscripten::val>::fromWireType(v));
        }
    };

    template<typename T>
    struct TypeID<T, std::enable_if_t<!tc::js::IsJsInteropable<tc::remove_cvref_t<T>>::value>> {
        static_assert(
            tc::js::IsJsInteropable<tc::remove_cvref_t<T>>::value,
            "js_ref.h is included, prohibiting passing generic non-IsJsInteropable types to emscripten for extra type safety, even if there is a corresponding EMSCRIPTEN_BINDINGS"
        );
    };
}
