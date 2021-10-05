#pragma once

#include "break_or_continue.h"
#include "explicit_cast.h"
#include "meta.h"
#include "for_each.h"
#include "tc_move.h"
#include "type_traits.h"
#include "js_callback.h"
#include "js_types.h"

#include <emscripten/val.h>
#include <type_traits>
#include <utility>

#include <boost/range/iterator.hpp>

namespace tc::js_defs {
	template<typename T> 
	struct _impl_js_Array;
	template<typename T> 
	using _js_Array = ::tc::jst::ref<_impl_js_Array<T>>;

	template<typename T> 
	struct _impl_js_ReadonlyArray;
	template<typename T> 
	using _js_ReadonlyArray = ::tc::jst::ref<_impl_js_ReadonlyArray<T>>;

	template<typename T> 
	struct _impl_js_Promise;
	template<typename T> 
	using _js_Promise = ::tc::jst::ref<_impl_js_Promise<T>>;

	template<typename K, typename V> 
	struct _impl_js_Record;
	template<typename K, typename V> 
	using _js_Record = ::tc::jst::ref<_impl_js_Record<K, V>>;

	template<typename T> 
	struct _impl_js_Iterable;
	template<typename T> 
	using _js_Iterable = ::tc::jst::ref<_impl_js_Iterable<T>>;

	struct _impl_js_console;
	using _js_console = ::tc::jst::ref<_impl_js_console>;
	template<typename T>
	using Array = _js_Array<T>;
	template<typename T>
	using ReadonlyArray = _js_ReadonlyArray<T>;
	template<typename T>
	using Promise = _js_Promise<T>;
	template<typename K, typename V>
	using Record = _js_Record<K, V>;
	template<typename T>
	using Iterable = _js_Iterable<T>;

	using console = _js_console;

	template<typename T>
	struct _impl_js_Array : virtual ::tc::jst::object_base {
		static_assert(::tc::jst::IsJsInteropable<T>::value);

		struct _tcjs_definitions {
			using value_type = T;
		};

		auto length() noexcept { return ::tc::explicit_cast<int>(_getProperty<double>("length")); }
		auto operator[](int i) && noexcept { return _getProperty<T>(i); }
		auto operator[](double i) && noexcept { return _getProperty<T>(tc::explicit_cast<int>(i)); } // See https://github.com/think-cell/tcjs/issues/8

		auto push(T const& item) noexcept { return _call<void>("push", item); }
		void _setIndex(int i, T value) noexcept { _setProperty(i, tc_move(value)); }

		static Array<T> _tcjs_construct() noexcept {
			return Array<T>(::emscripten::val::array());
		}

		template<typename Rng, typename = ::std::enable_if_t<::tc::is_explicit_castable<T, ::tc::range_reference_t<Rng>>::value>>
		static Array<T> _tcjs_construct(Rng&& rng) noexcept {
			Array<T> result(::emscripten::val::array());
			::tc::for_each(::std::forward<Rng>(rng), [&](auto&& value) noexcept {
				result->push(::tc::explicit_cast<T>(::std::forward<decltype(value)>(value)));
			});
			return result;
		}
	};

	template<typename T>
	struct _impl_js_ReadonlyArray : virtual _impl_js_Array<T> {
		auto push(T const& item) noexcept = delete;
		void _setIndex(int i, T value) noexcept = delete;
	};

	template<typename K, typename V>
	struct _impl_js_Record : virtual ::tc::jst::object_base {
		static_assert(::tc::jst::IsJsInteropable<K>::value);
		static_assert(::tc::jst::IsJsInteropable<V>::value);

		auto operator[](K k) noexcept { return _getProperty<V>(k); }
	};

	namespace no_adl {
		template<typename T>
		struct RemovePromise { using type = T; };
		template<typename T>
		struct RemovePromise<Promise<T>> { using type = T; };
		template<typename T>
		using RemovePromise_t = typename RemovePromise<T>::type;
	}

	template<typename T>
	struct _impl_js_Promise : virtual ::tc::jst::object_base {
		static_assert(::tc::jst::IsJsInteropable<T>::value);

		template<typename R>
		auto then(::tc::jst::function<R(T)> onfulfilled, ::tc::jst::function<R(::tc::js::any)> onrejected) noexcept {
			return _call<Promise<no_adl::RemovePromise_t<R>>>("then", onfulfilled, onrejected);
		}

		template<typename R1>
		auto then(::tc::jst::function<R1(T)> onfulfilled) noexcept {
			return _call<Promise<no_adl::RemovePromise_t<R1>>>("then", onfulfilled);
		}

		template<typename R1, typename R2>
		auto then(::tc::jst::function<R1(T)> onfulfilled, ::tc::jst::function<R2(::tc::js::any)> onrejected) noexcept {
			return _call<Promise<::tc::jst::union_t<no_adl::RemovePromise_t<R1>, no_adl::RemovePromise_t<R2>>>>("then", onfulfilled, onrejected);
		}
	};

	template<>
	struct _impl_js_Promise<void> : virtual _impl_js_Promise<::tc::js::undefined> {
		// JavaScript passes 'undefined' to what TypeScript calls 'void' promise.
	};
	
	template<typename T>
	struct _impl_js_Iterable : virtual ::tc::jst::object_base {
	};

	struct _impl_js_console : virtual ::tc::jst::object_base {
		struct _tcjs_definitions {
			template<typename... Args>
			static void log(Args&&... args) noexcept {
				static_assert((::tc::jst::IsJsInteropable<::tc::remove_cvref_t<Args>>::value && ...));
				::emscripten::val::global("console")["log"](::std::forward<Args>(args)...);
			}

			template<typename... Args>
			static void error(Args&&... args) noexcept {
				static_assert((::tc::jst::IsJsInteropable<::tc::remove_cvref_t<Args>>::value && ...));
				::emscripten::val::global("console")["error"](::std::forward<Args>(args)...);
			}

			template<typename... Args>
			static void warn(Args&&... args) noexcept {
				static_assert((::tc::jst::IsJsInteropable<::tc::remove_cvref_t<Args>>::value && ...));
				::emscripten::val::global("console")["warn"](::std::forward<Args>(args)...);
			}

			template<typename... Args>
			static void debug(Args&&... args) noexcept {
				static_assert((::tc::jst::IsJsInteropable<::tc::remove_cvref_t<Args>>::value && ...));
				::emscripten::val::global("console")["debug"](::std::forward<Args>(args)...);
			}
		};
	};
}

namespace tc::js {
	using tc::js_defs::Array;
	using tc::js_defs::console;
	using tc::js_defs::Promise;
	using tc::js_defs::ReadonlyArray;
	using tc::js_defs::Record;

	inline auto stackTrace() noexcept { // Expects non-standard `stackTrace()` function in JS to be available globally.
		return ::emscripten::val::global("stackTrace")().template as<tc::js::string>();
	}
} // namespace tc::js

inline bool IsBootstrapType(std::string const& strName) noexcept {
	return tc::binary_find_unique<tc::return_bool>(
		as_constexpr(tc::make_array<tc::ptr_range<char const>>(tc::aggregate_tag, "Array", "Iterable", "Promise", "ReadonlyArray", "Record")), 
		strName,
		tc::lessfrom3way(tc::fn_lexicographical_compare_3way())
	);
}

#define JS_HAS_MEM_FN_XXX_TRAIT_DEF( name, ... ) \
	template<typename U> \
	struct has_mem_fn_ ## name { \
	private: \
		template<typename T> static auto test(int) -> decltype(std::declval<T>()->name ( __VA_ARGS__ ), std::true_type()); \
		template<typename> static std::false_type test(...); \
	public: \
		static constexpr bool value = std::is_same<decltype(test<U>(0)), std::true_type>::value; \
	};

JS_HAS_MEM_FN_XXX_TRAIT_DEF(item, std::declval<double>())

// Define iterator types for tc::js::Array and tc::js::ReadonlyArray
#define JS_RANGE_WITH_ITERATORS(JsNamespace, JsType)                                                               \
	namespace tc::jst::range_detail {                                                                              \
		namespace no_adl {                                                                                         \
			template<typename T>                                                                                   \
			struct FGet##JsType##Index final {                                                                     \
			  private:                                                                                             \
				JsNamespace::JsType<T> m_t;                                                                        \
                                                                                                                   \
			  public:                                                                                              \
				FGet##JsType##Index(JsNamespace::JsType<T> t) : m_t(tc_move(t)) {}                                 \
																												   \
				template<typename S = T, std::enable_if_t< has_mem_fn_item<JsNamespace::JsType<S>>::value >* = nullptr>				\
				auto operator()(int i) const& noexcept {                                                              \
					return m_t->item(i);                                                                           \
				}                                                                                                  \
																												   \
				template<typename S = T, std::enable_if_t< !has_mem_fn_item<JsNamespace::JsType<S>>::value >* = nullptr>				\
				auto operator()(int i) const& noexcept {                                                              \
					return m_t[i];                                                                                 \
				}                                                                                                  \
			};                                                                                                     \
		}                                                                                                          \
                                                                                                                   \
		template<typename T>                                                                                       \
		using JsType##Iterator = boost::transform_iterator<                                                        \
			no_adl::FGet##JsType##Index<T>,                                                                        \
			tc::counting_iterator<int>>;                                                                           \
	}                                                                                                              \
                                                                                                                   \
	namespace boost {                                                                                              \
		template<typename T>                                                                                       \
		struct range_mutable_iterator<JsNamespace::JsType<T>> {                                                    \
			using type = tc::jst::range_detail::JsType##Iterator<T>;                                               \
		};                                                                                                         \
		template<typename T>                                                                                       \
		struct range_const_iterator<JsNamespace::JsType<T>> {                                                      \
			using type = tc::jst::range_detail::JsType##Iterator<T>;                                               \
		};                                                                                                         \
	}                                                                                                              \
	namespace tc {                                                                                                 \
		namespace begin_end_adl {                                                                                  \
			template<typename T>                                                                                   \
			tc::jst::range_detail::JsType##Iterator<T> begin(begin_end_tag_t, JsNamespace::JsType<T> t) noexcept { \
				return tc::jst::range_detail::JsType##Iterator<T>(0, tc_move(t));                                  \
			}                                                                                                      \
			template<typename T>                                                                                   \
			tc::jst::range_detail::JsType##Iterator<T> end(begin_end_tag_t, JsNamespace::JsType<T> t) noexcept {   \
				return tc::jst::range_detail::JsType##Iterator<T>(t->length(), tc_move(t));                        \
			}                                                                                                      \
		}                                                                                                          \
	}

JS_RANGE_WITH_ITERATORS(tc::js, ReadonlyArray)
JS_RANGE_WITH_ITERATORS(tc::js, Array)
