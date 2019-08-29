#pragma once

#include <emscripten/val.h>
#include <string>
#include <utility>
#include <type_traits>
#include "type_traits.h"
#include "tc_move.h"
#include "range_defines.h"
#include "noncopyable.h"
#include "js_types.h"

namespace tc::js {
namespace no_adl {
struct IObject {
private:
    emscripten::val m_emval;

protected:
    explicit IObject(emscripten::val const& _emval) noexcept : m_emval(_emval) {}
    explicit IObject(emscripten::val&& _emval) noexcept : m_emval(tc_move(_emval)) {}
    explicit IObject() noexcept : m_emval(emscripten::val::undefined()) {
        // Should never be called.
        _ASSERTFALSE;
    }

    template<typename T, typename Name>
    T _getProperty(Name&& name) noexcept {
        static_assert(IsJsInteropable<T>::value);
        return m_emval[std::forward<Name>(name)].template as<T>();
    }

    template<typename T, typename Name>
    void _setProperty(Name&& name, T const& value) noexcept {
        static_assert(IsJsInteropable<T>::value);
        m_emval.set(std::forward<Name>(name), value);
    }

    template<typename R, typename... Args>
    R _call(char const* name, Args&&... args) noexcept {
        static_assert(IsJsInteropable<R>::value);
        static_assert(std::conjunction<IsJsInteropable<tc::remove_cvref_t<Args>>...>::value);
        return m_emval.call<R>(name, std::forward<Args>(args)...);
    }

    template<typename R, typename... Args>
    R _call_this(Args&&... args) noexcept {
        static_assert(IsJsInteropable<R>::value);
        static_assert(std::conjunction<IsJsInteropable<tc::remove_cvref_t<Args>>...>::value);
        return m_emval(std::forward<Args>(args)...).template as<R>();
    }

    // Make sure the class and its descendants are abstract.
    virtual void __IObject_and_derived_are_abstract_Use_js_ref_instead() noexcept = 0;
};

template<typename> struct js_ref;
} // namespace no_adl
using no_adl::IObject;
using no_adl::js_ref;

namespace no_adl {
// Non-final, but non-polymorphic as well. Derive with care.
template<typename T>
struct js_ref {
    static_assert(std::is_class<T>::value);  // void is explicitly excluded as well, even though void* is base of all pointers.
    static_assert(!std::is_volatile<T>::value);
    static_assert(!std::is_const<T>::value, "We cannot guarantee constness of JS values");
    static_assert(std::is_convertible<T*, IObject*>::value);

    // js_ref is non-nullable.
    explicit js_ref(emscripten::val const& _emval) noexcept : m_emval(_emval) {
        _ASSERT(!!m_emval);
    }
    explicit js_ref(emscripten::val&& _emval) noexcept : m_emval(tc_move(_emval)) {
        _ASSERT(!!m_emval);
    }

    template<typename... Args, typename = std::enable_if_t<
        sizeof...(Args) != 1 ||
        ((!tc::is_instance_or_derived<js_ref, Args>::value && ...) &&
         (!std::is_same<js_unknown, tc::remove_cvref_t<Args>>::value && ...) &&
         (!tc::is_instance_or_derived<js_union, Args>::value && ...) &&
         (!std::is_same<emscripten::val, tc::remove_cvref_t<Args>>::value && ...))
    >, typename T2 = T, typename = tc::void_t<decltype(
        T2::_construct(std::forward<Args>(std::declval<Args>())...)
    )>>
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

    template<typename U, typename T2 = T, typename = decltype(std::declval<T2>().operator U())>
    explicit operator U() const& noexcept {
        return CArrowProxy(m_emval).operator U();
    }

private:
    emscripten::val m_emval;

    template<typename> friend struct js_ref;

    struct CArrowProxy final : T, tc::nonmovable {
        explicit CArrowProxy(emscripten::val const& m_emval) noexcept : IObject(m_emval) {}
        T* operator->() && noexcept { return this; }
    private:
        void __IObject_and_derived_are_abstract_Use_js_ref_instead() noexcept override {
            // Should never be called.
            assert(false);
        }
    };

public:
    CArrowProxy operator->() const& noexcept { return CArrowProxy(m_emval); }
};
} // namespace no_adl

namespace no_adl {
template<typename T>
struct IsJsInteropable<T, std::enable_if_t<
    tc::is_decayed<T>::value &&
    tc::is_instance_or_derived<js_ref, T>::value
>> : std::true_type {};
} // namespace no_adl

namespace emscripten_interop_detail::no_adl {
template<typename T>
struct IsEmvalWrapper<T, std::enable_if_t<tc::is_decayed<T>::value && tc::is_instance_or_derived<tc::js::js_ref, T>::value>> : std::true_type {
};
} // namespace emscripten_interop_detail::no_adl
} // namespace tc::js
