#pragma once

#include <tc/break_or_continue.h>
#include <tc/explicit_cast.h>
#include <tc/meta.h>
#include <tc/for_each.h>
#include <tc/tc_move.h>
#include <tc/type_traits.h>
#include <typescripten/callback.h>
#include <typescripten/ref.h>
#include <typescripten/types.h>

#include <emscripten/val.h>
#include <type_traits>
#include <utility>

#include <boost/range/iterator.hpp>

namespace tc::js_defs {
	// The remaining bootstrap types should eventually be generated from lib.es5.d.ts, too
	
	// Promise<void> will generate a then method taking as argument
	// a callback with signature js::function<bla(void)> which is
	// not valid C++
	template<typename T> 
	struct _impl_js_jPromise;
	template<typename T> 
	using _js_jPromise = ::tc::jst::ref<_impl_js_jPromise<T>>;
	

	// Mapped types are hard to parse because they have complicated
	// constraints
	template<typename K, typename V> 
	struct _impl_js_jRecord;
	template<typename K, typename V> 
	using _js_jRecord = ::tc::jst::ref<_impl_js_jRecord<K, V>>;
	template<typename K, typename V>
	using Record = _js_jRecord<K, V>;

	template<typename T>
	using Promise = _js_jPromise<T>;

	template<typename K, typename V>
	struct _impl_js_jRecord : virtual ::tc::jst::object_base {
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
	struct _impl_js_jPromise : virtual ::tc::jst::object_base {
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
	struct _impl_js_jPromise<void> : virtual _impl_js_jPromise<::tc::js::undefined> {
		// JavaScript passes 'undefined' to what TypeScript calls 'void' promise.
	};
}

namespace tc::js {
	using tc::js_defs::Promise;
	using tc::js_defs::Record;

	inline auto stackTrace() noexcept { // Expects non-standard `stackTrace()` function in JS to be available globally.
		return ::emscripten::val::global("stackTrace")().template as<tc::js::string>();
	}
} // namespace tc::js

inline bool IsBootstrapType(std::string const& strName) noexcept {
	return tc::binary_find_unique<tc::return_bool>(
		as_constexpr(tc::make_array<tc::ptr_range<char const>>(tc::aggregate_tag, "Promise", "Record")), 
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

namespace tc::jst::range_detail {
	namespace no_adl {
		template<typename Rng>
		struct FJsRangeIndexedAccess final {
		private:
			Rng m_rng;

		public:
			FJsRangeIndexedAccess(Rng rng) : m_rng(tc_move(rng)) {}

			template<typename S = Rng, std::enable_if_t< has_mem_fn_item<S>::value >* = nullptr>
			auto operator()(int i) const& noexcept {
				return m_rng->item(i);
			}

			template<typename S = Rng, std::enable_if_t< !has_mem_fn_item<S>::value >* = nullptr>
			auto operator()(int i) const& noexcept {
				return m_rng[i];
			}
		};
	}
}

// Define iterator types for tc::js::Array and tc::js::ReadonlyArray
#define JS_RANGE_WITH_ITERATORS_TMPL(JsNamespace, JsType)                                                          \
	namespace tc::jst::range_detail {                                                                              \
		template<typename T>                                                                                       \
		using JsType##Iterator = boost::transform_iterator<                                                        \
			no_adl::FJsRangeIndexedAccess<JsNamespace::JsType<T>>,                                                 \
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
				return tc::jst::range_detail::JsType##Iterator<T>(tc::explicit_cast<int>(t->length()), tc_move(t));                        \
			}                                                                                                      \
		}                                                                                                          \
	}

#define JS_RANGE_WITH_ITERATORS(JsNamespace, JsType)                                                               \
	namespace tc::jst::range_detail {                                                                              \
		using JsType##Iterator = boost::transform_iterator<                                                        \
			no_adl::FJsRangeIndexedAccess<JsNamespace::JsType>,                                                    \
			tc::counting_iterator<int>>;                                                                           \
	}                                                                                                              \
                                                                                                                   \
	namespace boost {	\
		template<>                                                                                              \
		struct range_mutable_iterator<JsNamespace::JsType> {                                                    \
			using type = tc::jst::range_detail::JsType##Iterator;                                               \
		}; \
		template<>                                                                                               \
		struct range_const_iterator<JsNamespace::JsType> {                                                      \
			using type = tc::jst::range_detail::JsType##Iterator;                                               \
		};                                                                                                         \
	}                                                                                                              \
	namespace tc {                                                                                                 \
		namespace begin_end_adl {                                                                                  \
			tc::jst::range_detail::JsType##Iterator begin(begin_end_tag_t, JsNamespace::JsType t) noexcept { \
				return tc::jst::range_detail::JsType##Iterator(0, tc_move(t));                                  \
			}                                                                                                      \
			tc::jst::range_detail::JsType##Iterator end(begin_end_tag_t, JsNamespace::JsType t) noexcept {   \
				return tc::jst::range_detail::JsType##Iterator(tc::explicit_cast<int>(t->length()), tc_move(t));   \
			}                                                                                                      \
		}                                                                                                          \
	}
