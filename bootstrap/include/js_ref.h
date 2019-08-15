#pragma once

#include <emscripten/val.h>
#include <emscripten/wire.h>
#include <type_traits>
#include "tc_move.h"
#include "range_defines.h"
#include "noncopyable.h"

namespace tc::js {

namespace no_adl {
struct IJsBase {
protected:
    emscripten::val m_emval;

    explicit IJsBase(emscripten::val const& m_emval) noexcept : m_emval(m_emval) {}
    explicit IJsBase(emscripten::val&& m_emval) noexcept : m_emval(tc_move(m_emval)) {}
    explicit IJsBase() : m_emval(emscripten::val::undefined()) {
        // Should never be called.
        _ASSERT(false);
    }

    // Make sure the class and its descendants are abstract.
    virtual void __IJsBase_and_derived_are_abstract_Use_js_ref_instead() = 0;
};

struct IAny : virtual IJsBase {
};

// Non-final, but non-polymorphic as well. Derive with care and remember to add specialization for tc::js::IsJsRef.
template<typename T>
struct js_ref {
    static_assert(std::is_class<T>::value);  // void is explicitly excluded as well, even though void* is base of all pointers.
    static_assert(!std::is_volatile<T>::value);
    static_assert(!std::is_const<T>::value, "We cannot guarantee constness of JS values");
    static_assert(std::is_convertible<T*, IJsBase*>::value);

    // js_ref is non-nullable.
    explicit js_ref(emscripten::val const& m_emval) noexcept : m_emval(m_emval) {
        _ASSERT(!m_emval.isUndefined() && !m_emval.isNull());
    }
    explicit js_ref(emscripten::val&& m_emval) noexcept : m_emval(tc_move(m_emval)) {
        _ASSERT(!m_emval.isUndefined() && !m_emval.isNull());
    }

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

    emscripten::val get() const& noexcept { return m_emval; }
    emscripten::val get() && noexcept { return tc_move(m_emval); }

private:
    emscripten::val m_emval;

    friend struct js_ref;

    struct CArrowProxy final : T, tc::nonmovable {
        explicit CArrowProxy(emscripten::val const& m_emval) noexcept : IJsBase(m_emval) {}
        T* operator->() && noexcept { return this; }
    private:
        void __IJsBase_and_derived_are_abstract_Use_js_ref_instead() override {
            // Should never be called.
            assert(false);
        }
    };

public:
    CArrowProxy operator->() const& noexcept { return CArrowProxy(m_emval); }
};

template<typename> struct IsJsRef : std::false_type {};
template<typename T> struct IsJsRef<js_ref<T>> : std::true_type {
    using value_type = T;
};
} // namespace no_adl

using no_adl::IJsBase;
using no_adl::IAny;
using no_adl::js_ref;
using no_adl::IsJsRef;

} // namespace tc::js

// Custom marshalling
namespace emscripten::internal {
    template<typename T>
    struct TypeID<T, std::enable_if_t<tc::js::IsJsRef<std::remove_cv_t<std::remove_reference_t<T>>>::value>> {
        static constexpr TYPEID get() {
            return LightTypeID<val>::get();
        }
    };

    template<typename T>
    struct BindingType<T, std::enable_if_t<tc::js::IsJsRef<T>::value>> {
        typedef typename BindingType<emscripten::val>::WireType WireType;

        static WireType toWireType(T const& js) {
            return BindingType<emscripten::val>::toWireType(js.get());
        }

        static auto fromWireType(WireType v) {
          return tc::js::js_ref<typename tc::js::IsJsRef<T>::value_type>(BindingType<emscripten::val>::fromWireType(v));
        }
    };
}
