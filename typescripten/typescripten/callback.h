#pragma once
#include <tc/casts.h>

#include <tc/assert_defs.h>
#include <tc/noncopyable.h>
#include <tc/tag_type.h>
#include <tc/tc_move.h>
#include <tc/type_list.h>
#include <tc/type_traits.h>
#include <boost/callable_traits.hpp>

#include <cstdint>
#include <emscripten/val.h>
#include <emscripten/wire.h>
#include <memory>
#include <type_traits>
#include <utility>

#include <typescripten/ref.h>

namespace tc::jst {
	// ---------------------------------------- Passing member functions to JS ----------------------------------------
	DEFINE_TAG_TYPE(pass_this)
	DEFINE_TAG_TYPE(pass_all_arguments)

	namespace callback_detail {
		void EnsureJsCallbackCppIsLinked();

		namespace no_adl {
			template<typename ListArgs, bool bPassedAllArguments = false>
			struct CCallableWrapper final {
				static_assert(!tc::type::find_unique<ListArgs, pass_this_t>::found);
				static_assert(!tc::type::find_unique<ListArgs, pass_all_arguments_t>::found);
				static_assert(tc::type::all_of<ListArgs, IsJsInteropable>::value);

				static inline constexpr bool c_bInstantiated = true;

				template<typename Fn>
				emscripten::val operator()(Fn&& fn, emscripten::val const& /*emvalThis*/, emscripten::val const& emvalArgs) const& noexcept {
					if constexpr (bPassedAllArguments) {
						_ASSERT(tc::type::size<ListArgs>::value <= emvalArgs["length"].as<std::size_t>());
					} else {
						_ASSERTEQUAL(emvalArgs["length"].as<std::size_t>(), tc::type::size<ListArgs>::value);
					}
					return [&]<typename... Args, std::size_t... Indices>(tc::type::list<Args...>, std::index_sequence<Indices...>) {
						auto fnWithArgs = [&]() noexcept -> decltype(auto) {
							return std::forward<Fn>(fn)(emvalArgs[Indices].template as<Args>()...);
						};
						using ReturnType = decltype(fnWithArgs());
						static_assert(IsJsInteropable<ReturnType>::value);
						if constexpr (std::is_void<ReturnType>::value) {
							fnWithArgs();
							return emscripten::val::undefined();
						} else {
							return emscripten::val(fnWithArgs());
						}
					}
					(ListArgs{}, std::make_index_sequence<tc::type::size<ListArgs>::value>{});
				}
			};

			template<typename TThis, typename... ArgsTail, bool bPassedAllArguments>
			struct CCallableWrapper<tc::type::list<pass_this_t, TThis, ArgsTail...>, bPassedAllArguments> final {
			  private:
				using ListArgsTail = tc::type::list<ArgsTail...>;
				using TailCCallableWrapper = CCallableWrapper<ListArgsTail, bPassedAllArguments>;

			  public:
				static_assert(!tc::type::find_unique<ListArgsTail, pass_this_t>::found);
				static_assert(IsJsInteropable<TThis>::value);

				static inline constexpr bool c_bInstantiated = true;

				template<typename Fn>
				emscripten::val operator()(Fn&& fn, emscripten::val const& emvalThis, emscripten::val const& emvalArgs) const& noexcept {
					return TailCCallableWrapper()(
						[&](auto&&... args) noexcept -> decltype(auto) {
							return fn(pass_this, emvalThis.template as<TThis>(), std::forward<decltype(args)>(args)...);
						},
						emvalThis,
						emvalArgs
					);
				}
			};

			template<typename TArgs, typename... ArgsTail, bool bPassedAllArguments>
			struct CCallableWrapper<tc::type::list<pass_all_arguments_t, TArgs, ArgsTail...>, bPassedAllArguments> final {
			  private:
				using ListArgsTail = tc::type::list<ArgsTail...>;
				using TailCCallableWrapper = CCallableWrapper<ListArgsTail, /*bPassedAllArguments=*/true>;

			  public:
				static_assert(!tc::type::find_unique<ListArgsTail, pass_this_t>::found);
				static_assert(!tc::type::find_unique<ListArgsTail, pass_all_arguments_t>::found);
				static_assert(IsJsInteropable<TArgs>::value);
				static_assert(!bPassedAllArguments);

				static inline constexpr bool c_bInstantiated = true;

				template<typename Fn>
				emscripten::val operator()(Fn&& fn, emscripten::val const& emvalThis, emscripten::val const& emvalArgs) const& noexcept {
					return TailCCallableWrapper()(
						[&](auto&&... args) noexcept -> decltype(auto) {
							return fn(pass_all_arguments, emvalArgs.template as<TArgs>(), std::forward<decltype(args)>(args)...);
						},
						emvalThis,
						emvalArgs
					);
				}
			};
		} // namespace no_adl
		using no_adl::CCallableWrapper;

		template<typename Pmf, typename = tc::void_t<boost::callable_traits::class_of_t<Pmf>>>
		using MemberFunctionExpectedArgs_t =
			tc::type::drop_first_t<boost::callable_traits::args_t<Pmf, tc::type::list>>;

		template<typename T>
		emscripten::val MemberFunctionWrapper(T pmfMember, void* pvThis, emscripten::val const& emvalThis, emscripten::val const& emvalArgs) noexcept {
			using ThisType = boost::callable_traits::class_of_t<T>;
			static_assert(boost::callable_traits::is_noexcept<T>::value, "Member functions exported to JS should be noexcept");
			static_assert(!std::is_copy_constructible<ThisType>::value, "The struct with JS-exported member functions should not have copy constructors");
			static_assert(!std::is_move_constructible<ThisType>::value, "The struct with JS-exported member functions should not have move constructors");

			return CCallableWrapper<MemberFunctionExpectedArgs_t<T>>()(
				[&](auto&&... args) noexcept -> decltype(auto) {
					return (tc::void_cast<ThisType>(pvThis)->*pmfMember)(std::forward<decltype(args)>(args)...);
				},
				emvalThis,
				emvalArgs
			);
		}

		namespace no_adl {
			template<typename>
			struct function {};

			template<typename R, typename... Args>
			struct function<R(Args...)> : virtual object_base {
				struct _tcjs_definitions {
					using function_type = R(Args...);
				};

				static_assert(IsJsInteropable<R>::value);
				// Implicit instantiation of CCallableWrapper to ensure Args are correct.
				static_assert(callback_detail::CCallableWrapper<tc::type::list<Args...>>::c_bInstantiated);

				R operator()(Args... args) noexcept {
					// These are limitations of emscripten::val, can be worked around.
					static_assert(std::is_same<tc::type::find_unique_if_result::type_not_found, tc::type::find_unique<tc::type::list<Args...>, pass_this_t>>::value, "Cannot call a JS function which needs 'this'");
					static_assert(std::is_same<tc::type::find_unique_if_result::type_not_found, tc::type::find_unique<tc::type::list<Args...>, pass_all_arguments_t>>::value, "Cannot call a JS function which takes an array of arguments");
					return _call_this<R>(tc_move(args)...);
				}
			};
		} // namespace no_adl	
	} // namespace callback_detail

	namespace function_detail {
		template<class... Args1> struct zip {
			template<class... Args2> struct with
			{
				typedef tc::type::list<std::pair<Args1, Args2>...> type;
			};
		};

		template<typename>
		struct is_convertible_pair;

		template<typename T1, typename T2>
		struct is_convertible_pair<std::pair<T1, T2>> : tc::is_safely_convertible<T1, T2> {};
	}

	template<typename>
	struct function;
	
	template<typename R, typename... Args>
	struct function<R(Args...)> : ref<callback_detail::no_adl::function<R(Args...)>> {
		using base = ref<callback_detail::no_adl::function<R(Args...)>>;
		explicit function(emscripten::val const& _emval) noexcept : base(_emval) {}
		explicit function(emscripten::val&& _emval) noexcept : base(tc_move(_emval)) {}

		template<typename R_, typename... Args_, typename = std::enable_if_t<
			tc::is_safely_convertible<R, R_>::value
			&& tc::type::all_of<typename function_detail::zip<Args_...>::template with<Args...>::type, function_detail::is_convertible_pair>::value
		>>
		operator function<R_(Args_...)>() const& noexcept {
			return function<R_(Args_...)>(this->getEmval());
		}
	};

	namespace callback_detail {
		using FirstArgument = void*;
		using FunctionPointer = emscripten::val (*)(FirstArgument, emscripten::val const& emvalThis, emscripten::val const& emvalArgs) noexcept;
		using PointerNumber = std::uintptr_t;

		namespace no_adl {
			// We do not care about slicing to ref<> or moving from this class, because
			// it is only stored as a by-value const field.
			template<typename T>
			struct CUniqueDetachableJsFunction 
				: private tc::nonmovable
				, tc::jst::function<T> 
			{
				CUniqueDetachableJsFunction(FunctionPointer pfunc, FirstArgument arg0) noexcept
					: tc::jst::function<T>([&]() {
						static auto creator = []() {
							EnsureJsCallbackCppIsLinked();
							auto creator = emscripten::val::module_property("tc_js_callback_detail_js_CreateJsFunction");
							_ASSERT(!creator.isUndefined() && "Unable to find a function from js_callback.js, did you pass '--pre-js js_callback.js' flags to em++?");
							return creator;
						}();
						return creator(reinterpret_cast<PointerNumber>(pfunc), reinterpret_cast<PointerNumber>(arg0));
					}()) {}

				~CUniqueDetachableJsFunction() {
					this->getEmval().template call<void>("detach");
				}
			};
		} // namespace no_adl
		using no_adl::CUniqueDetachableJsFunction;
	} // namespace callback_detail

// Can be used inline or to create a forward declaration
#define TC_JS_MEMBER_FUNCTION(ClassName, FieldName, ReturnType, Arguments)                                                                        \
	void FieldName##_tc_js_type_check() noexcept {                                                                                                \
		STATICASSERTSAME(ClassName&, decltype(*this), "ClassName specified in TC_JS_MEMBER_FUNCTION is incorrect");                               \
	}                                                                                                                                             \
	static emscripten::val FieldName##_tc_js_wrapper(void* pvThis, emscripten::val const& emvalThis, emscripten::val const& emvalArgs) noexcept { \
		return ::tc::jst::callback_detail::MemberFunctionWrapper(&ClassName::FieldName##_tc_js_impl, pvThis, emvalThis, emvalArgs);               \
	}                                                                                                                                             \
	::tc::jst::callback_detail::CUniqueDetachableJsFunction<ReturnType Arguments> FieldName{&FieldName##_tc_js_wrapper, this};              \
	ReturnType FieldName##_tc_js_impl Arguments noexcept

// Use if member function has been forward declared with TC_JS_MEMBER_FUNCTION
#define TC_JS_MEMBER_FUNCTION_DEF(ClassName, FieldName, ReturnType, Arguments) \
	ReturnType ClassName::FieldName##_tc_js_impl Arguments noexcept

	// ---------------------------------------- Lambda wrapper callback ----------------------------------------
	// This callback wraps lambda (or any callable) so it can be immediately passed as function<...>.
	// It takes ownership of the lambda. Whenever the wrapper is destroyed, corresponding JS function is turned
	// into no-op.
	//
	// Heap-allocated "fire-and-forget" callbacks are explicitly out of scope for the library: there are typically
	// no guarantees on when they're called and we want the user to think about that carefully and make sure all
	// proper cancellations are in place (e.g. by using member callback tied to the object which initiates the request).
	namespace callback_detail {
		namespace no_adl {
			// Helper class, should always be passed as a const value to avoid
			// function(function&&) binding to it and moving out.
			template<typename Fn>
			struct lambda_impl final : CUniqueDetachableJsFunction<boost::callable_traits::function_type_t<Fn>> {
				using function_type = boost::callable_traits::function_type_t<Fn>;

				static_assert(!std::is_reference<Fn>::value);
				static_assert(boost::callable_traits::is_noexcept<Fn>::value, "Callbacks for JS should be noexcept");

				template<typename FnSrc>
				lambda_impl(FnSrc&& fn) noexcept
					: CUniqueDetachableJsFunction<function_type>(&FnWrapper, this), m_fn(std::forward<FnSrc>(fn)) {}

				// Explicitly disable two-way conversion enabled by IsEmvalWrapper below.
				lambda_impl(emscripten::val) = delete;

			  private:
				Fn m_fn;

				static emscripten::val FnWrapper(void* pThis, emscripten::val const& emvalThis, emscripten::val const& emvalArgs) noexcept {
					return callback_detail::CCallableWrapper<boost::callable_traits::args_t<Fn, tc::type::list>>()(
						tc::void_cast<lambda_impl>(pThis)->m_fn,
						emvalThis,
						emvalArgs);
				}
			};
			template<typename Fn>
			lambda_impl(Fn) -> lambda_impl<Fn>;
		} // namespace no_adl
		using no_adl::lambda_impl;
	} // namespace callback_detail

	template<typename Fn>
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wignored-qualifiers" // https://bugs.llvm.org/show_bug.cgi?id=43709 and https://github.com/emscripten-core/emscripten/issues/11123
	auto const lambda(Fn&& fn) noexcept { // Returning const is important to avoid moving from the temporary.
#pragma clang diagnostic pop
		return callback_detail::lambda_impl(std::forward<Fn>(fn));
	}
} // namespace tc::jst
