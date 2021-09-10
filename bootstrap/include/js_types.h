#pragma once
#include "algorithm.h"
#include "assert_defs.h"
#include "explicit_cast.h"
#include "subrange.h"
#include "tag_type.h"
#include "tc_move.h"
#include "type_list.h"
#include "type_traits.h"
#include <emscripten/val.h>
#include <emscripten/wire.h>
#include <string>
#include <type_traits>
#include <utility>

namespace tc::jst { // Library helper types are in tc::jst namespace
	namespace no_adl {
		template<typename, typename = void>
		struct IsJsInteropable : std::false_type {
		};

		template<typename T>
		struct IsJsInteropable<
			T,
			std::enable_if_t<tc::type::find_unique<
				tc::type::list<
					void,
					double,
					bool>,
				T>::found>> : std::true_type {
		};

		template<typename T>
		struct IsJsIntegralEnum : std::false_type {
		};

		template<typename T>
		struct IsJsHeterogeneousEnum : std::false_type {
		};

		template<typename T>
		struct js_ref;
	} // namespace no_adl
	using no_adl::IsJsHeterogeneousEnum;
	using no_adl::IsJsIntegralEnum;
	using no_adl::IsJsInteropable;
} // namespace tc::jst

namespace tc::js { // Implementations of TypeScript/JavaScript builtin types are in tc::js namespace
	namespace no_adl {
		struct any final {
			explicit any(emscripten::val const& _emval) noexcept
				: m_emval(_emval) {}
			explicit any(emscripten::val&& _emval) noexcept
				: m_emval(tc_move(_emval)) {}

			emscripten::val const& getEmval() const& noexcept { return m_emval; }
			emscripten::val&& getEmval() && noexcept { return tc_move(m_emval); }

			template<typename T, typename = std::enable_if_t<tc::jst::IsJsInteropable<tc::remove_cvref_t<T>>::value>>
			any(T&& value) noexcept
				: m_emval(std::forward<T>(value)) {
			}

			template<typename U, typename = std::enable_if_t<tc::jst::IsJsInteropable<tc::remove_cvref_t<U>>::value && !std::is_same<tc::remove_cvref_t<U>, bool>::value>>
			explicit operator U() const& noexcept { return m_emval.template as<U>(); }

			explicit operator bool() const& noexcept {
				_ASSERT(m_emval.isTrue() || m_emval.isFalse());
				return m_emval.template as<bool>();
			}

		  private:
			emscripten::val m_emval;
		};

		struct undefined final {
			undefined() noexcept {}
			explicit undefined(emscripten::val const& IF_TC_CHECKS(emval)) noexcept {
				_ASSERT(emval.isUndefined());
			}
			emscripten::val getEmval() const& noexcept { return emscripten::val::undefined(); }
		};

		struct null final {
			null() noexcept {}
			explicit null(emscripten::val const& IF_TC_CHECKS(emval)) noexcept {
				_ASSERT(emval.isNull());
			}
			emscripten::val getEmval() const& noexcept { return emscripten::val::null(); }
		};

		struct string final {
			// There only way to change the underlying value (both in C++ and JS) is to reassign.
			// There are no in-place modification methods in either C++ or JS.
			explicit string(emscripten::val const& _emval) noexcept
				: m_emval(_emval) {
				_ASSERT(m_emval.isString());
			}
			explicit string(emscripten::val&& _emval) noexcept
				: m_emval(tc_move(_emval)) {
				_ASSERT(m_emval.isString());
			}

			emscripten::val const& getEmval() const& noexcept { return m_emval; }
			emscripten::val&& getEmval() && noexcept { return tc_move(m_emval); }

			template<typename Rng, typename = std::enable_if_t<tc::is_explicit_castable<std::string, Rng&&>::value>>
			explicit string(Rng&& rng) noexcept
				// TODO: avoid allocating std::string (we may have to duplicate parts of embind)
				: m_emval(tc::explicit_cast<std::string>(std::forward<Rng>(rng))) 
			{}

			int length() const& noexcept { return m_emval["length"].as<int>(); }

			explicit operator std::string() const& noexcept { return m_emval.template as<std::string>(); }

		  private:
			emscripten::val m_emval;
		};
	} // namespace no_adl
	using no_adl::null;
	using no_adl::string;
	using no_adl::undefined;
	using no_adl::any;
} // namespace tc::js

namespace tc::jst {
	template<typename... Rng>
	[[nodiscard]] auto make_string(Rng&&... rng) MAYTHROW {
		return tc::js::string(::tc::make_str(std::forward<Rng>(rng)...));
	}

	// Any union of null|T T|null undefined|T or T|undefined behaves like a optional<T>
	namespace union_detail {
		namespace no_adl {
			template<typename, typename... Ts>
			struct CDetectOptionLike {
			};

			template<typename T>
			struct CDetectOptionLike<
				std::enable_if_t<!std::is_same<tc::js::null, T>::value>,
				tc::js::undefined,
				T> {
				using option_like_type = T;
			};

			template<typename T>
			struct CDetectOptionLike<
				std::enable_if_t<!std::is_same<tc::js::undefined, T>::value>,
				tc::js::null,
				T> {
				using option_like_type = T;
			};

			template<typename T>
			struct CDetectOptionLike<
				std::enable_if_t<!std::is_same<tc::js::null, T>::value>,
				T,
				tc::js::undefined> {
				using option_like_type = T;
			};

			template<typename T>
			struct CDetectOptionLike<
				std::enable_if_t<!std::is_same<tc::js::undefined, T>::value>,
				T,
				tc::js::null> {
				using option_like_type = T;
			};

			template<typename>
			struct IsExplicitCastableFrom {
			};

			template<typename... From>
			struct IsExplicitCastableFrom<tc::type::list<From...>> {
				template<typename To>
				using type = tc::is_explicit_castable<To, From...>;
			};
		} // namespace no_adl
		using no_adl::CDetectOptionLike;
		using no_adl::IsExplicitCastableFrom;

		template<typename ListFrom, typename ListTs>
		using FindUniqueExplicitCastableFrom = tc::type::find_unique_if<ListTs, no_adl::IsExplicitCastableFrom<ListFrom>::template type>;
	} // namespace union_detail

	namespace no_adl {
		// TODO: optimize by providing JS-side toWireType/fromWireType for integrals/bools and getting rid of emscripten::val
		template<typename... Ts>
		struct union_t : union_detail::CDetectOptionLike<void, Ts...> {
			static inline constexpr auto instantiated = true;

			static_assert(1 < sizeof...(Ts));
			static_assert((IsJsInteropable<Ts>::value && ...));
			static_assert((!std::is_same<Ts, tc::js::any>::value && ...));
			static_assert((!std::is_void<Ts>::value && ...));

			using ListTs = tc::type::list<Ts...>;

			static inline constexpr auto has_undefined = tc::type::find_unique<ListTs, tc::js::undefined>::found;
			static inline constexpr auto has_null = tc::type::find_unique<ListTs, tc::js::null>::found;
			static inline constexpr auto has_string = tc::type::find_unique<ListTs, tc::js::string>::found;
			static inline constexpr auto has_bool = tc::type::find_unique<ListTs, bool>::found;
			static inline constexpr auto has_number = tc::type::find_unique<ListTs, double>::found;

			explicit union_t(emscripten::val const& _emval) noexcept
				: m_emval(_emval) { assertEmvalInRange(); }
			explicit union_t(emscripten::val&& _emval) noexcept
				: m_emval(tc_move(_emval)) { assertEmvalInRange(); }

			emscripten::val const& getEmval() const& noexcept { return m_emval; }
			emscripten::val&& getEmval() && noexcept { return tc_move(m_emval); }

			template<ENABLE_SFINAE, std::enable_if_t<SFINAE_TYPE(union_t)::has_undefined && !SFINAE_TYPE(union_t)::has_null>* = nullptr>
			union_t() noexcept
				: union_t(emscripten::val::undefined()) {}

			template<ENABLE_SFINAE, std::enable_if_t<!SFINAE_TYPE(union_t)::has_undefined && SFINAE_TYPE(union_t)::has_null>* = nullptr>
			union_t() noexcept
				: union_t(emscripten::val::null()) {}

			// Constructing from a subtype. Assumption: the underlying representation does not depend on what element of ListTs is chosen.
			template<typename T, std::enable_if_t<IsJsInteropable<tc::remove_cvref_t<T>>::value && (std::is_convertible<T&&, Ts>::value || ...)>* = nullptr>
			union_t(T&& value) noexcept
				: m_emval(std::forward<T>(value)) {}

			// Constructing from a wider union_t (derived cast).
			template<typename T, std::enable_if_t<std::conjunction<std::negation<std::is_same<tc::remove_cvref_t<T>, union_t>>, tc::is_instance_or_derived<union_t, T>, std::is_convertible<union_t, tc::remove_cvref_t<T>>>::value>* = nullptr>
			explicit union_t(T&& other) noexcept
				: m_emval(std::forward<T>(other).m_emval) {
				assertEmvalInRange();
			}

			template<typename... Args>
			explicit union_t(tc::aggregate_tag_t, Args&&... args) noexcept
				: m_emval(
					tc::explicit_cast<typename union_detail::FindUniqueExplicitCastableFrom<tc::type::list<Args&&...>, ListTs>::type>(
						std::forward<Args>(args)...
					)
				) 
			{}

			// Base cast to a common type. Assumption: the underlying representation does not depend on what element of ListTs is chosen.
			template<typename T, std::enable_if_t<
				!std::is_same<bool, tc::remove_cvref_t<T>>::value 
				&& !std::is_same<tc::js::any, tc::remove_cvref_t<T>>::value // any(T&&) ctor takes care of that.
				&& (std::is_convertible<Ts, T>::value && ...)>* = nullptr
			>
			operator T() const& noexcept {
				return m_emval.template as<T>();
			}

			// Extracting a specific type.
			template<typename T, std::enable_if_t<
				!std::is_same<bool, tc::remove_cvref_t<T>>::value // operator bool() has special meaning, use get<> to get `bool` out of `union_t`.
				&& !(std::is_convertible<Ts, T>::value && ...) // implicit conversion takes care of that.
				&& (std::is_same<Ts, tc::remove_cvref_t<T>>::value || ...)>* = nullptr>
			explicit operator T() const& noexcept {
				static_assert((std::is_same<Ts, T>::value || ...));
				return get<tc::remove_cvref_t<T>>();
			}

			template<typename T, std::enable_if_t<(std::is_same<Ts, T>::value || ...)>* = nullptr>
			T get() const& noexcept {
				// Checks for 'double' vs 'undefined'/'null' are performed by emscripten.
				// Checks for 'js_ref'/'string' vs 'undefined'/'null' are performed by js_ref/string's ctor.
				if constexpr (std::is_same<T, bool>::value) {
					_ASSERT(m_emval.isTrue() || m_emval.isFalse());
				}
				return m_emval.template as<T>();
			}

		  private:
		  	// Members specific to optionals

			// We cannot derive optional<T> from union_t<T, js::undefined> and implement
			// these members in the derived class only, because  "optional" is a concept 
			// that union_t<T, js::undefined>, union_t<T, js::null>, union_t<js::undefined, T> 
			// and union_t<js::null, T> satisfy

			template<typename... Us>
			friend struct union_t;

			template<typename T>
			struct CArrowProxy final {
				T m_value;

				CArrowProxy(T&& value) noexcept : m_value(tc_move(value)) {}

				T* operator->() & noexcept { return std::addressof(m_value); }
				T const* operator->() const& noexcept { return std::addressof(m_value); }
			};

		  public:
			template<ENABLE_SFINAE>
			typename SFINAE_TYPE(union_t)::option_like_type operator*() const& noexcept {
				return get<typename union_t::option_like_type>();
			}

			template<ENABLE_SFINAE>
			CArrowProxy<typename SFINAE_TYPE(union_t)::option_like_type> operator->() const& noexcept {
				return get<typename union_t::option_like_type>();
			}

			template<ENABLE_SFINAE, std::enable_if_t<!std::is_void<typename SFINAE_TYPE(union_t)::option_like_type>::value>* = nullptr>
			operator bool() const& noexcept {
				// This matches the semantics of std::optional
				static_assert(has_undefined != has_null);
				if constexpr(has_undefined) {
					return !m_emval.isUndefined();
				} else {
					return !m_emval.isNull();
				}
			}

		  private:
			emscripten::val m_emval;

			void assertEmvalInRange() const& noexcept {
				if constexpr (!has_undefined) {
					_ASSERT(!m_emval.isUndefined());
				}
				if constexpr (!has_null) {
					_ASSERT(!m_emval.isNull());
				}
				if constexpr (!has_string) {
					_ASSERT(!m_emval.isString());
				}
				if constexpr (!has_bool) {
					_ASSERT(!m_emval.isTrue());
					_ASSERT(!m_emval.isFalse());
				}
				if constexpr (!has_number) {
					_ASSERT(!m_emval.isNumber());
				}
			}
		};

	} // namespace no_adl
	using no_adl::union_t;
	
	template<typename T>
	using optional = union_t<tc::js::undefined, T>;

	template<>
	struct IsJsInteropable<tc::js::any> : std::true_type {
	};

	template<>
	struct IsJsInteropable<tc::js::undefined> : std::true_type {
	};

	template<>
	struct IsJsInteropable<tc::js::null> : std::true_type {
	};

	template<typename... Ts>
	struct IsJsInteropable<union_t<Ts...>> : std::true_type {
		static_assert(union_t<Ts...>::instantiated);
	};

	template<>
	struct IsJsInteropable<tc::js::string> : std::true_type {
	};

	template<typename T>
	struct IsJsInteropable<T, std::enable_if_t<IsJsIntegralEnum<T>::value>> : std::true_type {
	};

	template<typename T>
	struct IsJsInteropable<T, std::enable_if_t<IsJsHeterogeneousEnum<T>::value>> : std::true_type {
	};

	namespace emscripten_interop_detail {
		namespace no_adl {
			template<typename T, typename = void>
			struct IsEmvalWrapper : std::false_type {
			};

			template<typename T>
			struct IsEmvalWrapper<T, std::enable_if_t<tc::type::find_unique<tc::type::list<tc::js::any, tc::js::undefined, tc::js::null, tc::js::string>, T>::found>> : std::true_type {
			};

			template<typename... Ts>
			struct IsEmvalWrapper<union_t<Ts...>, std::enable_if_t<IsJsInteropable<union_t<Ts...>>::value>> : std::true_type {
			};
		} // namespace no_adl
		using no_adl::IsEmvalWrapper;
	} // namespace emscripten_interop_detail
} // namespace tc::jst

// Custom marshalling
namespace emscripten::internal {
	template<typename T>
	struct TypeID<T, std::enable_if_t<tc::jst::emscripten_interop_detail::IsEmvalWrapper<tc::remove_cvref_t<T>>::value>> {
		static constexpr TYPEID get() {
			return TypeID<val>::get();
		}
	};

	template<typename T>
	struct BindingType<T, std::enable_if_t<tc::jst::emscripten_interop_detail::IsEmvalWrapper<T>::value>> {
		typedef typename BindingType<emscripten::val>::WireType WireType;

		static WireType toWireType(T const& js) {
			return BindingType<emscripten::val>::toWireType(js.getEmval());
		}

		static auto fromWireType(WireType v) {
			return static_cast<T>(BindingType<emscripten::val>::fromWireType(v));
		}
	};

	template<typename T>
	struct TypeID<T, std::enable_if_t<tc::jst::IsJsIntegralEnum<tc::remove_cvref_t<T>>::value>> {
		static constexpr TYPEID get() {
			return TypeID<double>::get();
		}
	};

	template<typename T>
	struct BindingType<T, std::enable_if_t<tc::jst::IsJsIntegralEnum<T>::value>> {
		using UnderlyingType = std::underlying_type_t<T>;
		static_assert(std::is_integral<UnderlyingType>::value);

		// We want rounding checks, so underlying type is 'double' directly, not 'int' (which will silently round).
		typedef typename BindingType<double>::WireType WireType;

		static WireType toWireType(T const& en) {
			return BindingType<double>::toWireType(tc::explicit_cast<double>(static_cast<UnderlyingType>(en)));
		}

		static auto fromWireType(WireType wire) {
			return static_cast<T>(tc::explicit_cast<UnderlyingType>(BindingType<double>::fromWireType(wire)));
		}
	};

	template<typename T>
	struct TypeID<T, std::enable_if_t<tc::jst::IsJsHeterogeneousEnum<tc::remove_cvref_t<T>>::value>> {
		using UnderlyingType = typename std::remove_reference_t<decltype(tc::jst::IsJsHeterogeneousEnum<tc::remove_cvref_t<T>>::Values())>::mapped_type;

		static constexpr TYPEID get() {
			return TypeID<UnderlyingType>::get();
		}
	};

	template<typename T>
	struct BindingType<T, std::enable_if_t<tc::jst::IsJsHeterogeneousEnum<T>::value>> {
		using UnderlyingType = typename TypeID<T>::UnderlyingType;

		typedef typename BindingType<UnderlyingType>::WireType WireType;

		static WireType toWireType(T const& en) {
			auto const& umValueToUnderlying = tc::jst::IsJsHeterogeneousEnum<T>::Values();
			return BindingType<UnderlyingType>::toWireType(tc::cont_find<tc::return_element>(umValueToUnderlying, en)->second);
		}

		static auto fromWireType(WireType wire) {
			auto const& umValueToUnderlying = tc::jst::IsJsHeterogeneousEnum<T>::Values();
			auto const underlying = BindingType<UnderlyingType>::fromWireType(wire);
			return tc::find_first_if<tc::return_element>(umValueToUnderlying, [&](auto const& kv) {
				return kv.second.getEmval().strictlyEquals(underlying.getEmval());
			})->first;
		}
	};
} // namespace emscripten::internal
