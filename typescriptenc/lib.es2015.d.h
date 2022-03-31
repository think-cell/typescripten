#include <boost/hana/assert.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/map.hpp>
#include <boost/hana/pair.hpp>
#include <boost/hana/type.hpp>
#include <boost/hana/string.hpp>
#include <boost/hana/hash.hpp>
#include <boost/hana/at_key.hpp>
using namespace boost::hana::literals;
#pragma push_macro("assert")
#undef assert
namespace tc::js_defs {
} // namespace tc::js_defs
namespace tc::jst {
} // namespace tc::jst
namespace tc::js_defs {
	using namespace jst; // no ADL
	template<typename T>
	struct _impl_js_jArray;
	template<typename T>
	using _js_jArray = ref<_impl_js_jArray<T>>;
	struct _impl_js_jArrayBuffer;
	using _js_jArrayBuffer = ref<_impl_js_jArrayBuffer>;
	struct _impl_js_jArrayBufferConstructor;
	using _js_jArrayBufferConstructor = ref<_impl_js_jArrayBufferConstructor>;
	struct _impl_js_jArrayBufferTypes;
	using _js_jArrayBufferTypes = ref<_impl_js_jArrayBufferTypes>;
	struct _impl_js_jArrayBufferView;
	using _js_jArrayBufferView = ref<_impl_js_jArrayBufferView>;
	struct _impl_js_jArrayConstructor;
	using _js_jArrayConstructor = ref<_impl_js_jArrayConstructor>;
	template<typename T>
	struct _impl_js_jArrayLike;
	template<typename T>
	using _js_jArrayLike = ref<_impl_js_jArrayLike<T>>;
	struct _impl_js_jBoolean;
	using _js_jBoolean = ref<_impl_js_jBoolean>;
	struct _impl_js_jBooleanConstructor;
	using _js_jBooleanConstructor = ref<_impl_js_jBooleanConstructor>;
	struct _impl_js_jFunction;
	using _js_jFunction = ref<_impl_js_jFunction>;
	struct _impl_js_jCallableFunction;
	using _js_jCallableFunction = ref<_impl_js_jCallableFunction>;
	template<typename T>
	struct _impl_js_jConcatArray;
	template<typename T>
	using _js_jConcatArray = ref<_impl_js_jConcatArray<T>>;
	struct _impl_js_jDataView;
	using _js_jDataView = ref<_impl_js_jDataView>;
	struct _impl_js_jDataViewConstructor;
	using _js_jDataViewConstructor = ref<_impl_js_jDataViewConstructor>;
	struct _impl_js_jDate;
	using _js_jDate = ref<_impl_js_jDate>;
	struct _impl_js_jDateConstructor;
	using _js_jDateConstructor = ref<_impl_js_jDateConstructor>;
	struct _impl_js_jError;
	using _js_jError = ref<_impl_js_jError>;
	struct _impl_js_jErrorConstructor;
	using _js_jErrorConstructor = ref<_impl_js_jErrorConstructor>;
	struct _impl_js_jEvalError;
	using _js_jEvalError = ref<_impl_js_jEvalError>;
	struct _impl_js_jEvalErrorConstructor;
	using _js_jEvalErrorConstructor = ref<_impl_js_jEvalErrorConstructor>;
	struct _impl_js_jFloat32Array;
	using _js_jFloat32Array = ref<_impl_js_jFloat32Array>;
	struct _impl_js_jFloat32ArrayConstructor;
	using _js_jFloat32ArrayConstructor = ref<_impl_js_jFloat32ArrayConstructor>;
	struct _impl_js_jFloat64Array;
	using _js_jFloat64Array = ref<_impl_js_jFloat64Array>;
	struct _impl_js_jFloat64ArrayConstructor;
	using _js_jFloat64ArrayConstructor = ref<_impl_js_jFloat64ArrayConstructor>;
	struct _impl_js_jFunctionConstructor;
	using _js_jFunctionConstructor = ref<_impl_js_jFunctionConstructor>;
	template<typename T, typename TReturn, typename TNext>
	struct _impl_js_jIterator;
	template<typename T, typename TReturn, typename TNext>
	using _js_jIterator = ref<_impl_js_jIterator<T, TReturn, TNext>>;
	template<typename T, typename TReturn, typename TNext>
	struct _impl_js_jGenerator;
	template<typename T, typename TReturn, typename TNext>
	using _js_jGenerator = ref<_impl_js_jGenerator<T, TReturn, TNext>>;
	struct _impl_js_jGeneratorFunction;
	using _js_jGeneratorFunction = ref<_impl_js_jGeneratorFunction>;
	struct _impl_js_jGeneratorFunctionConstructor;
	using _js_jGeneratorFunctionConstructor = ref<_impl_js_jGeneratorFunctionConstructor>;
	struct _impl_js_jIArguments;
	using _js_jIArguments = ref<_impl_js_jIArguments>;
	struct _impl_js_jImportMeta;
	using _js_jImportMeta = ref<_impl_js_jImportMeta>;
	struct _impl_js_jInt16Array;
	using _js_jInt16Array = ref<_impl_js_jInt16Array>;
	struct _impl_js_jInt16ArrayConstructor;
	using _js_jInt16ArrayConstructor = ref<_impl_js_jInt16ArrayConstructor>;
	struct _impl_js_jInt32Array;
	using _js_jInt32Array = ref<_impl_js_jInt32Array>;
	struct _impl_js_jInt32ArrayConstructor;
	using _js_jInt32ArrayConstructor = ref<_impl_js_jInt32ArrayConstructor>;
	struct _impl_js_jInt8Array;
	using _js_jInt8Array = ref<_impl_js_jInt8Array>;
	struct _impl_js_jInt8ArrayConstructor;
	using _js_jInt8ArrayConstructor = ref<_impl_js_jInt8ArrayConstructor>;
	struct _impl_js_jIntl;
	using _js_jIntl = ref<_impl_js_jIntl>;
	struct _impl_js_jIntl_dCollator;
	using _js_jIntl_dCollator = ref<_impl_js_jIntl_dCollator>;
	struct _impl_js_jIntl_dCollatorOptions;
	using _js_jIntl_dCollatorOptions = ref<_impl_js_jIntl_dCollatorOptions>;
	struct _impl_js_jIntl_dDateTimeFormat;
	using _js_jIntl_dDateTimeFormat = ref<_impl_js_jIntl_dDateTimeFormat>;
	struct _impl_js_jIntl_dDateTimeFormatOptions;
	using _js_jIntl_dDateTimeFormatOptions = ref<_impl_js_jIntl_dDateTimeFormatOptions>;
	struct _impl_js_jIntl_dDateTimeFormatPart;
	using _js_jIntl_dDateTimeFormatPart = ref<_impl_js_jIntl_dDateTimeFormatPart>;
	struct _impl_js_jIntl_dNumberFormat;
	using _js_jIntl_dNumberFormat = ref<_impl_js_jIntl_dNumberFormat>;
	struct _impl_js_jIntl_dNumberFormatOptions;
	using _js_jIntl_dNumberFormatOptions = ref<_impl_js_jIntl_dNumberFormatOptions>;
	struct _impl_js_jIntl_dNumberFormatPart;
	using _js_jIntl_dNumberFormatPart = ref<_impl_js_jIntl_dNumberFormatPart>;
	struct _impl_js_jIntl_dPluralRules;
	using _js_jIntl_dPluralRules = ref<_impl_js_jIntl_dPluralRules>;
	struct _impl_js_jIntl_dPluralRulesOptions;
	using _js_jIntl_dPluralRulesOptions = ref<_impl_js_jIntl_dPluralRulesOptions>;
	struct _impl_js_jIntl_dResolvedCollatorOptions;
	using _js_jIntl_dResolvedCollatorOptions = ref<_impl_js_jIntl_dResolvedCollatorOptions>;
	struct _impl_js_jIntl_dResolvedDateTimeFormatOptions;
	using _js_jIntl_dResolvedDateTimeFormatOptions = ref<_impl_js_jIntl_dResolvedDateTimeFormatOptions>;
	struct _impl_js_jIntl_dResolvedNumberFormatOptions;
	using _js_jIntl_dResolvedNumberFormatOptions = ref<_impl_js_jIntl_dResolvedNumberFormatOptions>;
	struct _impl_js_jIntl_dResolvedPluralRulesOptions;
	using _js_jIntl_dResolvedPluralRulesOptions = ref<_impl_js_jIntl_dResolvedPluralRulesOptions>;
	template<typename T>
	struct _impl_js_jIterable;
	template<typename T>
	using _js_jIterable = ref<_impl_js_jIterable<T>>;
	template<typename T>
	struct _impl_js_jIterableIterator;
	template<typename T>
	using _js_jIterableIterator = ref<_impl_js_jIterableIterator<T>>;
	template<typename TReturn>
	struct _impl_js_jIteratorReturnResult;
	template<typename TReturn>
	using _js_jIteratorReturnResult = ref<_impl_js_jIteratorReturnResult<TReturn>>;
	template<typename TYield>
	struct _impl_js_jIteratorYieldResult;
	template<typename TYield>
	using _js_jIteratorYieldResult = ref<_impl_js_jIteratorYieldResult<TYield>>;
	struct _impl_js_jJSON;
	using _js_jJSON = ref<_impl_js_jJSON>;
	template<typename K, typename V>
	struct _impl_js_jMap;
	template<typename K, typename V>
	using _js_jMap = ref<_impl_js_jMap<K, V>>;
	struct _impl_js_jMapConstructor;
	using _js_jMapConstructor = ref<_impl_js_jMapConstructor>;
	struct _impl_js_jMath;
	using _js_jMath = ref<_impl_js_jMath>;
	struct _impl_js_jNewableFunction;
	using _js_jNewableFunction = ref<_impl_js_jNewableFunction>;
	struct _impl_js_jNumber;
	using _js_jNumber = ref<_impl_js_jNumber>;
	struct _impl_js_jNumberConstructor;
	using _js_jNumberConstructor = ref<_impl_js_jNumberConstructor>;
	struct _impl_js_jObject;
	using _js_jObject = ref<_impl_js_jObject>;
	struct _impl_js_jObjectConstructor;
	using _js_jObjectConstructor = ref<_impl_js_jObjectConstructor>;
	struct _impl_js_jPromiseConstructor;
	using _js_jPromiseConstructor = ref<_impl_js_jPromiseConstructor>;
	template<typename T>
	struct _impl_js_jPromiseLike;
	template<typename T>
	using _js_jPromiseLike = ref<_impl_js_jPromiseLike<T>>;
	struct _impl_js_jPropertyDescriptor;
	using _js_jPropertyDescriptor = ref<_impl_js_jPropertyDescriptor>;
	struct _impl_js_jPropertyDescriptorMap;
	using _js_jPropertyDescriptorMap = ref<_impl_js_jPropertyDescriptorMap>;
	struct _impl_js_jProxyConstructor;
	using _js_jProxyConstructor = ref<_impl_js_jProxyConstructor>;
	template<typename T>
	struct _impl_js_jProxyHandler;
	template<typename T>
	using _js_jProxyHandler = ref<_impl_js_jProxyHandler<T>>;
	struct _impl_js_jRangeError;
	using _js_jRangeError = ref<_impl_js_jRangeError>;
	struct _impl_js_jRangeErrorConstructor;
	using _js_jRangeErrorConstructor = ref<_impl_js_jRangeErrorConstructor>;
	template<typename T>
	struct _impl_js_jReadonlyArray;
	template<typename T>
	using _js_jReadonlyArray = ref<_impl_js_jReadonlyArray<T>>;
	template<typename K, typename V>
	struct _impl_js_jReadonlyMap;
	template<typename K, typename V>
	using _js_jReadonlyMap = ref<_impl_js_jReadonlyMap<K, V>>;
	template<typename T>
	struct _impl_js_jReadonlySet;
	template<typename T>
	using _js_jReadonlySet = ref<_impl_js_jReadonlySet<T>>;
	struct _impl_js_jReferenceError;
	using _js_jReferenceError = ref<_impl_js_jReferenceError>;
	struct _impl_js_jReferenceErrorConstructor;
	using _js_jReferenceErrorConstructor = ref<_impl_js_jReferenceErrorConstructor>;
	struct _impl_js_jReflect;
	using _js_jReflect = ref<_impl_js_jReflect>;
	struct _impl_js_jRegExp;
	using _js_jRegExp = ref<_impl_js_jRegExp>;
	struct _impl_js_jRegExpConstructor;
	using _js_jRegExpConstructor = ref<_impl_js_jRegExpConstructor>;
	struct _impl_js_jRegExpExecArray;
	using _js_jRegExpExecArray = ref<_impl_js_jRegExpExecArray>;
	struct _impl_js_jRegExpMatchArray;
	using _js_jRegExpMatchArray = ref<_impl_js_jRegExpMatchArray>;
	template<typename T>
	struct _impl_js_jSet;
	template<typename T>
	using _js_jSet = ref<_impl_js_jSet<T>>;
	struct _impl_js_jSetConstructor;
	using _js_jSetConstructor = ref<_impl_js_jSetConstructor>;
	struct _impl_js_jString;
	using _js_jString = ref<_impl_js_jString>;
	struct _impl_js_jStringConstructor;
	using _js_jStringConstructor = ref<_impl_js_jStringConstructor>;
	struct _impl_js_jSymbol;
	using _js_jSymbol = ref<_impl_js_jSymbol>;
	struct _impl_js_jSymbolConstructor;
	using _js_jSymbolConstructor = ref<_impl_js_jSymbolConstructor>;
	struct _impl_js_jSyntaxError;
	using _js_jSyntaxError = ref<_impl_js_jSyntaxError>;
	struct _impl_js_jSyntaxErrorConstructor;
	using _js_jSyntaxErrorConstructor = ref<_impl_js_jSyntaxErrorConstructor>;
	struct _impl_js_jTemplateStringsArray;
	using _js_jTemplateStringsArray = ref<_impl_js_jTemplateStringsArray>;
	template<typename T>
	struct _impl_js_jThisType;
	template<typename T>
	using _js_jThisType = ref<_impl_js_jThisType<T>>;
	struct _impl_js_jTypeError;
	using _js_jTypeError = ref<_impl_js_jTypeError>;
	struct _impl_js_jTypeErrorConstructor;
	using _js_jTypeErrorConstructor = ref<_impl_js_jTypeErrorConstructor>;
	template<typename T>
	struct _impl_js_jTypedPropertyDescriptor;
	template<typename T>
	using _js_jTypedPropertyDescriptor = ref<_impl_js_jTypedPropertyDescriptor<T>>;
	struct _impl_js_jURIError;
	using _js_jURIError = ref<_impl_js_jURIError>;
	struct _impl_js_jURIErrorConstructor;
	using _js_jURIErrorConstructor = ref<_impl_js_jURIErrorConstructor>;
	struct _impl_js_jUint16Array;
	using _js_jUint16Array = ref<_impl_js_jUint16Array>;
	struct _impl_js_jUint16ArrayConstructor;
	using _js_jUint16ArrayConstructor = ref<_impl_js_jUint16ArrayConstructor>;
	struct _impl_js_jUint32Array;
	using _js_jUint32Array = ref<_impl_js_jUint32Array>;
	struct _impl_js_jUint32ArrayConstructor;
	using _js_jUint32ArrayConstructor = ref<_impl_js_jUint32ArrayConstructor>;
	struct _impl_js_jUint8Array;
	using _js_jUint8Array = ref<_impl_js_jUint8Array>;
	struct _impl_js_jUint8ArrayConstructor;
	using _js_jUint8ArrayConstructor = ref<_impl_js_jUint8ArrayConstructor>;
	struct _impl_js_jUint8ClampedArray;
	using _js_jUint8ClampedArray = ref<_impl_js_jUint8ClampedArray>;
	struct _impl_js_jUint8ClampedArrayConstructor;
	using _js_jUint8ClampedArrayConstructor = ref<_impl_js_jUint8ClampedArrayConstructor>;
	template<typename K, typename V>
	struct _impl_js_jWeakMap;
	template<typename K, typename V>
	using _js_jWeakMap = ref<_impl_js_jWeakMap<K, V>>;
	struct _impl_js_jWeakMapConstructor;
	using _js_jWeakMapConstructor = ref<_impl_js_jWeakMapConstructor>;
	template<typename T>
	struct _impl_js_jWeakSet;
	template<typename T>
	using _js_jWeakSet = ref<_impl_js_jWeakSet<T>>;
	struct _impl_js_jWeakSetConstructor;
	using _js_jWeakSetConstructor = ref<_impl_js_jWeakSetConstructor>;
	using _js_jArrayBufferLike = tc::js::any /*UnionWithUnsupportedSubtype=[_js_jArrayBuffer,tc::js::any]*/;
	template<typename S>
	using _js_jCapitalize = tc::js::any;
	template<typename T, typename TReturn>
	using _js_jIteratorResult = tc::jst::union_t<_js_jIteratorReturnResult<TReturn>, _js_jIteratorYieldResult<T>>;
	template<typename S>
	using _js_jLowercase = tc::js::any;
	template<typename T, typename K>
	using _js_jOmit = tc::js::any /*UnknownClassOrInterface=Pick*/;
	using _js_jParameterDecorator = tc::jst::function<void(_js_jObject, tc::js::any /*UnionWithUnsupportedSubtype=[tc::js::string,tc::js::any]*/, double)>;
	template<typename T>
	using _js_jPartial = tc::js::any /*UnknownObjectType=Partial<T> objectFlags=32*/;
	template<typename T, typename K>
	using _js_jPick = tc::js::any /*UnknownObjectType=Pick<T, K> objectFlags=3145760*/;
	using _js_jPromiseConstructorLike = tc::js::any /*AnonymousTypeWithLiteralType=PromiseConstructorLike[__new]*/;
	using _js_jPropertyDecorator = tc::jst::function<void(_js_jObject, tc::js::any /*UnionWithUnsupportedSubtype=[tc::js::string,tc::js::any]*/)>;
	template<typename T>
	using _js_jReadonly = tc::js::any /*UnknownObjectType=Readonly<T> objectFlags=32*/;
	template<typename T>
	using _js_jRequired = tc::js::any /*UnknownObjectType=Required<T> objectFlags=32*/;
	template<typename S>
	using _js_jUncapitalize = tc::js::any;
	template<typename S>
	using _js_jUppercase = tc::js::any;
	template<typename T>
	struct _impl_js_jArray : virtual object_base {
		struct _tcjs_definitions {
			static constexpr auto keyof = boost::hana::make_map(
				boost::hana::make_pair("length"_s, boost::hana::type_c<double>)
			);
		};
		auto length() noexcept;
		void length(double v) noexcept;
		static auto _tcjs_construct(tc::jst::union_t<double, tc::js::undefined> arrayLength = tc::js::undefined()) noexcept;
		static auto _tcjs_construct(double arrayLength) noexcept;
		static auto _tcjs_construct(tc::jst::variadic_arg_t, T items) noexcept;
		auto concat(tc::jst::variadic_arg_t, _js_jConcatArray<T> items) noexcept;
		auto concat(tc::jst::variadic_arg_t, tc::jst::union_t<T, _js_jConcatArray<T>> items) noexcept;
		auto copyWithin(double target, double start, tc::jst::union_t<double, tc::js::undefined> end = tc::js::undefined()) noexcept;
		auto entries() noexcept;
		template<typename S>
		auto every(tc::jst::function<bool /*false*/(T, double, _js_jArray<T>)> predicate, tc::js::any thisArg) noexcept;
		auto every(tc::jst::function<tc::js::any(T, double, _js_jArray<T>)> predicate, tc::js::any thisArg) noexcept;
		auto fill(T value, tc::jst::union_t<double, tc::js::undefined> start = tc::js::undefined(), tc::jst::union_t<double, tc::js::undefined> end = tc::js::undefined()) noexcept;
		template<typename S>
		auto filter(tc::jst::function<bool /*false*/(T, double, _js_jArray<T>)> predicate, tc::js::any thisArg) noexcept;
		auto filter(tc::jst::function<tc::js::any(T, double, _js_jArray<T>)> predicate, tc::js::any thisArg) noexcept;
		template<typename S>
		auto find(tc::jst::function<bool /*false*/(T, double, _js_jArray<T>)> predicate, tc::js::any thisArg) noexcept;
		auto find(tc::jst::function<tc::js::any(T, double, _js_jArray<T>)> predicate, tc::js::any thisArg) noexcept;
		auto findIndex(tc::jst::function<tc::js::any(T, double, _js_jArray<T>)> predicate, tc::js::any thisArg) noexcept;
		auto forEach(tc::jst::function<void(T, double, _js_jArray<T>)> callbackfn, tc::js::any thisArg) noexcept;
		auto includes(T searchElement, tc::jst::union_t<double, tc::js::undefined> fromIndex = tc::js::undefined()) noexcept;
		auto indexOf(T searchElement, tc::jst::union_t<double, tc::js::undefined> fromIndex = tc::js::undefined()) noexcept;
		auto join(tc::jst::union_t<tc::js::string, tc::js::undefined> separator = tc::js::undefined()) noexcept;
		auto keys() noexcept;
		auto lastIndexOf(T searchElement, tc::jst::union_t<double, tc::js::undefined> fromIndex = tc::js::undefined()) noexcept;
		template<typename U>
		auto map(tc::jst::function<U(T, double, _js_jArray<T>)> callbackfn, tc::js::any thisArg) noexcept;
		auto operator[](double n) noexcept;
		auto pop() noexcept;
		auto push(tc::jst::variadic_arg_t, T items) noexcept;
		auto reduce(tc::jst::function<T(T, T, double, _js_jArray<T>)> callbackfn) noexcept;
		auto reduce(tc::jst::function<T(T, T, double, _js_jArray<T>)> callbackfn, T initialValue) noexcept;
		template<typename U>
		auto reduce(tc::jst::function<U(U, T, double, _js_jArray<T>)> callbackfn, U initialValue) noexcept;
		auto reduceRight(tc::jst::function<T(T, T, double, _js_jArray<T>)> callbackfn) noexcept;
		auto reduceRight(tc::jst::function<T(T, T, double, _js_jArray<T>)> callbackfn, T initialValue) noexcept;
		template<typename U>
		auto reduceRight(tc::jst::function<U(U, T, double, _js_jArray<T>)> callbackfn, U initialValue) noexcept;
		auto reverse() noexcept;
		auto shift() noexcept;
		auto slice(tc::jst::union_t<double, tc::js::undefined> start = tc::js::undefined(), tc::jst::union_t<double, tc::js::undefined> end = tc::js::undefined()) noexcept;
		auto some(tc::jst::function<tc::js::any(T, double, _js_jArray<T>)> predicate, tc::js::any thisArg) noexcept;
		auto sort(tc::jst::union_t<tc::js::undefined, tc::jst::function<double(T, T)>> compareFn = tc::js::undefined()) noexcept;
		auto splice(double start, double deleteCount, tc::jst::variadic_arg_t, T items) noexcept;
		auto splice(double start, tc::jst::union_t<double, tc::js::undefined> deleteCount = tc::js::undefined()) noexcept;
		auto toLocaleString() noexcept;
		auto toString() noexcept;
		auto unshift(tc::jst::variadic_arg_t, T items) noexcept;
		auto values() noexcept;
	};
	struct _impl_js_jArrayBuffer : virtual object_base {
		struct _tcjs_definitions {
			static constexpr auto keyof = boost::hana::make_map(
				boost::hana::make_pair("byteLength"_s, boost::hana::type_c<double>)
			);
		};
		auto byteLength() noexcept;
		static auto _tcjs_construct(double byteLength) noexcept;
		auto slice(double begin, tc::jst::union_t<double, tc::js::undefined> end = tc::js::undefined()) noexcept;
	};
	struct _impl_js_jArrayBufferConstructor : virtual object_base {
		struct _tcjs_definitions {
			static constexpr auto keyof = boost::hana::make_map(
				boost::hana::make_pair("prototype"_s, boost::hana::type_c<_js_jArrayBuffer>)
			);
		};
		auto prototype() noexcept;
		auto isView(tc::js::any arg) noexcept;
	};
	struct _impl_js_jArrayBufferTypes : virtual object_base {
		struct _tcjs_definitions {
			static constexpr auto keyof = boost::hana::make_map(
				boost::hana::make_pair("SharedArrayBuffer"_s, boost::hana::type_c<tc::js::any /*UnknownClassOrInterface=SharedArrayBuffer*/>),
				boost::hana::make_pair("ArrayBuffer"_s, boost::hana::type_c<_js_jArrayBuffer>)
			);
		};
		auto SharedArrayBuffer() noexcept;
		void SharedArrayBuffer(tc::js::any /*UnknownClassOrInterface=SharedArrayBuffer*/ v) noexcept;
		auto ArrayBuffer() noexcept;
		void ArrayBuffer(_js_jArrayBuffer v) noexcept;
	};
	struct _impl_js_jArrayBufferView : virtual object_base {
		struct _tcjs_definitions {
			static constexpr auto keyof = boost::hana::make_map(
				boost::hana::make_pair("buffer"_s, boost::hana::type_c<_js_jArrayBufferLike>),
				boost::hana::make_pair("byteLength"_s, boost::hana::type_c<double>),
				boost::hana::make_pair("byteOffset"_s, boost::hana::type_c<double>)
			);
		};
		auto buffer() noexcept;
		void buffer(_js_jArrayBufferLike v) noexcept;
		auto byteLength() noexcept;
		void byteLength(double v) noexcept;
		auto byteOffset() noexcept;
		void byteOffset(double v) noexcept;
	};
	struct _impl_js_jArrayConstructor : virtual object_base {
		struct _tcjs_definitions {
			static constexpr auto keyof = boost::hana::make_map(
				boost::hana::make_pair("prototype"_s, boost::hana::type_c<_js_jArray<tc::js::any>>)
			);
		};
		auto prototype() noexcept;
		template<typename T>
		auto from(_js_jArrayLike<T> arrayLike) noexcept;
		template<typename T, typename U>
		auto from(_js_jArrayLike<T> arrayLike, tc::jst::function<U(T, double)> mapfn, tc::js::any thisArg) noexcept;
		template<typename T>
		auto from(tc::jst::union_t<_js_jArrayLike<T>, _js_jIterable<T>> iterable) noexcept;
		template<typename T, typename U>
		auto from(tc::jst::union_t<_js_jArrayLike<T>, _js_jIterable<T>> iterable, tc::jst::function<U(T, double)> mapfn, tc::js::any thisArg) noexcept;
		auto isArray(tc::js::any arg) noexcept;
		template<typename T>
		auto of(tc::jst::variadic_arg_t, T items) noexcept;
	};
	template<typename T>
	struct _impl_js_jArrayLike : virtual object_base {
		struct _tcjs_definitions {
			static constexpr auto keyof = boost::hana::make_map(
				boost::hana::make_pair("length"_s, boost::hana::type_c<double>)
			);
		};
		auto length() noexcept;
		auto operator[](double n) noexcept;
	};
	struct _impl_js_jBoolean : virtual object_base {
		struct _tcjs_definitions {
			static constexpr auto keyof = boost::hana::make_map(

			);
		};
		static auto _tcjs_construct(tc::js::any value) noexcept;
		auto valueOf() noexcept;
	};
	struct _impl_js_jBooleanConstructor : virtual object_base {
		struct _tcjs_definitions {
			static constexpr auto keyof = boost::hana::make_map(
				boost::hana::make_pair("prototype"_s, boost::hana::type_c<_js_jBoolean>)
			);
		};
		auto prototype() noexcept;
	};
	struct _impl_js_jFunction : virtual object_base {
		struct _tcjs_definitions {
			static constexpr auto keyof = boost::hana::make_map(
				boost::hana::make_pair("prototype"_s, boost::hana::type_c<tc::js::any>),
				boost::hana::make_pair("length"_s, boost::hana::type_c<double>),
				boost::hana::make_pair("arguments"_s, boost::hana::type_c<tc::js::any>),
				boost::hana::make_pair("caller"_s, boost::hana::type_c<_js_jFunction>),
				boost::hana::make_pair("name"_s, boost::hana::type_c<tc::js::string>)
			);
		};
		auto prototype() noexcept;
		void prototype(tc::js::any v) noexcept;
		auto length() noexcept;
		auto arguments() noexcept;
		void arguments(tc::js::any v) noexcept;
		auto caller() noexcept;
		void caller(_js_jFunction v) noexcept;
		auto name() noexcept;
		static auto _tcjs_construct(tc::jst::variadic_arg_t, tc::js::string args) noexcept;
		auto apply(tc::js::any thisArg, tc::js::any argArray) noexcept;
		auto bind(tc::js::any thisArg, tc::jst::variadic_arg_t, tc::js::any argArray) noexcept;
		auto call(tc::js::any thisArg, tc::jst::variadic_arg_t, tc::js::any argArray) noexcept;
		auto toString() noexcept;
	};
	struct _impl_js_jCallableFunction : virtual _impl_js_jFunction {
		struct _tcjs_definitions {
			static constexpr auto keyof = boost::hana::make_map(

			);
		};
		template<typename T, typename R>
		auto apply(T thisArg) noexcept;
		template<typename T, typename A, typename R>
		auto apply(T thisArg, A args) noexcept;
		template<typename T, typename A0, typename A, typename R>
		auto bind(T thisArg, A0 arg0) noexcept;
		template<typename T, typename A0, typename A1, typename A, typename R>
		auto bind(T thisArg, A0 arg0, A1 arg1) noexcept;
		template<typename T, typename A0, typename A1, typename A2, typename A, typename R>
		auto bind(T thisArg, A0 arg0, A1 arg1, A2 arg2) noexcept;
		template<typename T, typename A0, typename A1, typename A2, typename A3, typename A, typename R>
		auto bind(T thisArg, A0 arg0, A1 arg1, A2 arg2, A3 arg3) noexcept;
		template<typename T, typename AX, typename R>
		auto bind(T thisArg, tc::jst::variadic_arg_t, AX args) noexcept;
		template<typename T>
		auto bind(tc::js::any /* unsupported type flags =16777216: ThisParameterType<T>*/ thisArg) noexcept;
		template<typename T, typename A, typename R>
		auto call(T thisArg, A args) noexcept;
	};
	template<typename T>
	struct _impl_js_jConcatArray : virtual object_base {
		struct _tcjs_definitions {
			static constexpr auto keyof = boost::hana::make_map(
				boost::hana::make_pair("length"_s, boost::hana::type_c<double>)
			);
		};
		auto length() noexcept;
		auto join(tc::jst::union_t<tc::js::string, tc::js::undefined> separator = tc::js::undefined()) noexcept;
		auto operator[](double n) noexcept;
		auto slice(tc::jst::union_t<double, tc::js::undefined> start = tc::js::undefined(), tc::jst::union_t<double, tc::js::undefined> end = tc::js::undefined()) noexcept;
	};
	struct _impl_js_jDataView : virtual object_base {
		struct _tcjs_definitions {
			static constexpr auto keyof = boost::hana::make_map(
				boost::hana::make_pair("buffer"_s, boost::hana::type_c<_js_jArrayBuffer>),
				boost::hana::make_pair("byteLength"_s, boost::hana::type_c<double>),
				boost::hana::make_pair("byteOffset"_s, boost::hana::type_c<double>)
			);
		};
		auto buffer() noexcept;
		auto byteLength() noexcept;
		auto byteOffset() noexcept;
		static auto _tcjs_construct(_js_jArrayBufferLike buffer, tc::jst::union_t<double, tc::js::undefined> byteOffset = tc::js::undefined(), tc::jst::union_t<double, tc::js::undefined> byteLength = tc::js::undefined()) noexcept;
		auto getBigInt64(double byteOffset, tc::jst::union_t<bool /*false*/, tc::js::undefined> littleEndian = tc::js::undefined()) noexcept;
		auto getBigUint64(double byteOffset, tc::jst::union_t<bool /*false*/, tc::js::undefined> littleEndian = tc::js::undefined()) noexcept;
		auto getFloat32(double byteOffset, tc::jst::union_t<bool /*false*/, tc::js::undefined> littleEndian = tc::js::undefined()) noexcept;
		auto getFloat64(double byteOffset, tc::jst::union_t<bool /*false*/, tc::js::undefined> littleEndian = tc::js::undefined()) noexcept;
		auto getInt16(double byteOffset, tc::jst::union_t<bool /*false*/, tc::js::undefined> littleEndian = tc::js::undefined()) noexcept;
		auto getInt32(double byteOffset, tc::jst::union_t<bool /*false*/, tc::js::undefined> littleEndian = tc::js::undefined()) noexcept;
		auto getInt8(double byteOffset) noexcept;
		auto getUint16(double byteOffset, tc::jst::union_t<bool /*false*/, tc::js::undefined> littleEndian = tc::js::undefined()) noexcept;
		auto getUint32(double byteOffset, tc::jst::union_t<bool /*false*/, tc::js::undefined> littleEndian = tc::js::undefined()) noexcept;
		auto getUint8(double byteOffset) noexcept;
		auto setBigInt64(double byteOffset, tc::js::any /* unsupported type flags =64: bigint*/ value, tc::jst::union_t<bool /*false*/, tc::js::undefined> littleEndian = tc::js::undefined()) noexcept;
		auto setBigUint64(double byteOffset, tc::js::any /* unsupported type flags =64: bigint*/ value, tc::jst::union_t<bool /*false*/, tc::js::undefined> littleEndian = tc::js::undefined()) noexcept;
		auto setFloat32(double byteOffset, double value, tc::jst::union_t<bool /*false*/, tc::js::undefined> littleEndian = tc::js::undefined()) noexcept;
		auto setFloat64(double byteOffset, double value, tc::jst::union_t<bool /*false*/, tc::js::undefined> littleEndian = tc::js::undefined()) noexcept;
		auto setInt16(double byteOffset, double value, tc::jst::union_t<bool /*false*/, tc::js::undefined> littleEndian = tc::js::undefined()) noexcept;
		auto setInt32(double byteOffset, double value, tc::jst::union_t<bool /*false*/, tc::js::undefined> littleEndian = tc::js::undefined()) noexcept;
		auto setInt8(double byteOffset, double value) noexcept;
		auto setUint16(double byteOffset, double value, tc::jst::union_t<bool /*false*/, tc::js::undefined> littleEndian = tc::js::undefined()) noexcept;
		auto setUint32(double byteOffset, double value, tc::jst::union_t<bool /*false*/, tc::js::undefined> littleEndian = tc::js::undefined()) noexcept;
		auto setUint8(double byteOffset, double value) noexcept;
	};
	struct _impl_js_jDataViewConstructor : virtual object_base {
		struct _tcjs_definitions {
			static constexpr auto keyof = boost::hana::make_map(
				boost::hana::make_pair("prototype"_s, boost::hana::type_c<_js_jDataView>)
			);
		};
		auto prototype() noexcept;
	};
	struct _impl_js_jDate : virtual object_base {
		struct _tcjs_definitions {
			static constexpr auto keyof = boost::hana::make_map(

			);
		};
		static auto _tcjs_construct() noexcept;
		static auto _tcjs_construct(tc::jst::union_t<double, tc::js::string> value) noexcept;
		static auto _tcjs_construct(double year, double month, tc::jst::union_t<double, tc::js::undefined> date = tc::js::undefined(), tc::jst::union_t<double, tc::js::undefined> hours = tc::js::undefined(), tc::jst::union_t<double, tc::js::undefined> minutes = tc::js::undefined(), tc::jst::union_t<double, tc::js::undefined> seconds = tc::js::undefined(), tc::jst::union_t<double, tc::js::undefined> ms = tc::js::undefined()) noexcept;
		static auto _tcjs_construct(tc::jst::union_t<_js_jDate, double, tc::js::string> value) noexcept;
		auto getDate() noexcept;
		auto getDay() noexcept;
		auto getFullYear() noexcept;
		auto getHours() noexcept;
		auto getMilliseconds() noexcept;
		auto getMinutes() noexcept;
		auto getMonth() noexcept;
		auto getSeconds() noexcept;
		auto getTime() noexcept;
		auto getTimezoneOffset() noexcept;
		auto getUTCDate() noexcept;
		auto getUTCDay() noexcept;
		auto getUTCFullYear() noexcept;
		auto getUTCHours() noexcept;
		auto getUTCMilliseconds() noexcept;
		auto getUTCMinutes() noexcept;
		auto getUTCMonth() noexcept;
		auto getUTCSeconds() noexcept;
		auto setDate(double date) noexcept;
		auto setFullYear(double year, tc::jst::union_t<double, tc::js::undefined> month = tc::js::undefined(), tc::jst::union_t<double, tc::js::undefined> date = tc::js::undefined()) noexcept;
		auto setHours(double hours, tc::jst::union_t<double, tc::js::undefined> min = tc::js::undefined(), tc::jst::union_t<double, tc::js::undefined> sec = tc::js::undefined(), tc::jst::union_t<double, tc::js::undefined> ms = tc::js::undefined()) noexcept;
		auto setMilliseconds(double ms) noexcept;
		auto setMinutes(double min, tc::jst::union_t<double, tc::js::undefined> sec = tc::js::undefined(), tc::jst::union_t<double, tc::js::undefined> ms = tc::js::undefined()) noexcept;
		auto setMonth(double month, tc::jst::union_t<double, tc::js::undefined> date = tc::js::undefined()) noexcept;
		auto setSeconds(double sec, tc::jst::union_t<double, tc::js::undefined> ms = tc::js::undefined()) noexcept;
		auto setTime(double time) noexcept;
		auto setUTCDate(double date) noexcept;
		auto setUTCFullYear(double year, tc::jst::union_t<double, tc::js::undefined> month = tc::js::undefined(), tc::jst::union_t<double, tc::js::undefined> date = tc::js::undefined()) noexcept;
		auto setUTCHours(double hours, tc::jst::union_t<double, tc::js::undefined> min = tc::js::undefined(), tc::jst::union_t<double, tc::js::undefined> sec = tc::js::undefined(), tc::jst::union_t<double, tc::js::undefined> ms = tc::js::undefined()) noexcept;
		auto setUTCMilliseconds(double ms) noexcept;
		auto setUTCMinutes(double min, tc::jst::union_t<double, tc::js::undefined> sec = tc::js::undefined(), tc::jst::union_t<double, tc::js::undefined> ms = tc::js::undefined()) noexcept;
		auto setUTCMonth(double month, tc::jst::union_t<double, tc::js::undefined> date = tc::js::undefined()) noexcept;
		auto setUTCSeconds(double sec, tc::jst::union_t<double, tc::js::undefined> ms = tc::js::undefined()) noexcept;
		auto toDateString() noexcept;
		auto toISOString() noexcept;
		auto toJSON(tc::js::any key) noexcept;
		auto toLocaleDateString() noexcept;
		auto toLocaleDateString(tc::jst::union_t<_js_jArray<tc::js::string>, tc::js::string, tc::js::undefined> locales = tc::js::undefined(), tc::jst::union_t<_js_jIntl_dDateTimeFormatOptions, tc::js::undefined> options = tc::js::undefined()) noexcept;
		auto toLocaleString() noexcept;
		auto toLocaleString(tc::jst::union_t<_js_jArray<tc::js::string>, tc::js::string, tc::js::undefined> locales = tc::js::undefined(), tc::jst::union_t<_js_jIntl_dDateTimeFormatOptions, tc::js::undefined> options = tc::js::undefined()) noexcept;
		auto toLocaleTimeString() noexcept;
		auto toLocaleTimeString(tc::jst::union_t<_js_jArray<tc::js::string>, tc::js::string, tc::js::undefined> locales = tc::js::undefined(), tc::jst::union_t<_js_jIntl_dDateTimeFormatOptions, tc::js::undefined> options = tc::js::undefined()) noexcept;
		auto toString() noexcept;
		auto toTimeString() noexcept;
		auto toUTCString() noexcept;
		auto valueOf() noexcept;
	};
	struct _impl_js_jDateConstructor : virtual object_base {
		struct _tcjs_definitions {
			static constexpr auto keyof = boost::hana::make_map(
				boost::hana::make_pair("prototype"_s, boost::hana::type_c<_js_jDate>)
			);
		};
		auto prototype() noexcept;
		auto UTC(double year, double month, tc::jst::union_t<double, tc::js::undefined> date = tc::js::undefined(), tc::jst::union_t<double, tc::js::undefined> hours = tc::js::undefined(), tc::jst::union_t<double, tc::js::undefined> minutes = tc::js::undefined(), tc::jst::union_t<double, tc::js::undefined> seconds = tc::js::undefined(), tc::jst::union_t<double, tc::js::undefined> ms = tc::js::undefined()) noexcept;
		auto now() noexcept;
		auto parse(tc::js::string s) noexcept;
	};
	struct _impl_js_jError : virtual object_base {
		struct _tcjs_definitions {
			static constexpr auto keyof = boost::hana::make_map(
				boost::hana::make_pair("name"_s, boost::hana::type_c<tc::js::string>),
				boost::hana::make_pair("message"_s, boost::hana::type_c<tc::js::string>),
				boost::hana::make_pair("stack"_s, boost::hana::type_c<tc::jst::union_t<tc::js::string, tc::js::undefined>>)
			);
		};
		auto name() noexcept;
		void name(tc::js::string v) noexcept;
		auto message() noexcept;
		void message(tc::js::string v) noexcept;
		auto stack() noexcept;
		void stack(tc::jst::union_t<tc::js::string, tc::js::undefined> v) noexcept;
		static auto _tcjs_construct(tc::jst::union_t<tc::js::string, tc::js::undefined> message = tc::js::undefined()) noexcept;
	};
	struct _impl_js_jErrorConstructor : virtual object_base {
		struct _tcjs_definitions {
			static constexpr auto keyof = boost::hana::make_map(
				boost::hana::make_pair("prototype"_s, boost::hana::type_c<_js_jError>),
				boost::hana::make_pair("prepareStackTrace"_s, boost::hana::type_c<tc::jst::union_t<tc::js::undefined, tc::jst::function<tc::js::any(_js_jError, _js_jArray<tc::js::any /*UnknownClassOrInterface=NodeJS.CallSite*/>)>>>),
				boost::hana::make_pair("stackTraceLimit"_s, boost::hana::type_c<double>)
			);
		};
		auto prototype() noexcept;
		auto prepareStackTrace() noexcept;
		void prepareStackTrace(tc::jst::union_t<tc::js::undefined, tc::jst::function<tc::js::any(_js_jError, _js_jArray<tc::js::any /*UnknownClassOrInterface=NodeJS.CallSite*/>)>> v) noexcept;
		auto stackTraceLimit() noexcept;
		void stackTraceLimit(double v) noexcept;
		auto captureStackTrace(tc::js::any /* unsupported type flags =67108864: object*/ targetObject, tc::jst::union_t<_js_jFunction, tc::js::undefined> constructorOpt = tc::js::undefined()) noexcept;
	};
	struct _impl_js_jEvalError : virtual _impl_js_jError {
		struct _tcjs_definitions {
			static constexpr auto keyof = boost::hana::make_map(

			);
		};
		static auto _tcjs_construct(tc::jst::union_t<tc::js::string, tc::js::undefined> message = tc::js::undefined()) noexcept;
	};
	struct _impl_js_jEvalErrorConstructor : virtual _impl_js_jErrorConstructor {
		struct _tcjs_definitions {
			static constexpr auto keyof = boost::hana::make_map(
				boost::hana::make_pair("prototype"_s, boost::hana::type_c<_js_jEvalError>)
			);
		};
		auto prototype() noexcept;
	};
	struct _impl_js_jFloat32Array : virtual object_base {
		struct _tcjs_definitions {
			static constexpr auto keyof = boost::hana::make_map(
				boost::hana::make_pair("BYTES_PER_ELEMENT"_s, boost::hana::type_c<double>),
				boost::hana::make_pair("buffer"_s, boost::hana::type_c<_js_jArrayBufferLike>),
				boost::hana::make_pair("byteLength"_s, boost::hana::type_c<double>),
				boost::hana::make_pair("byteOffset"_s, boost::hana::type_c<double>),
				boost::hana::make_pair("length"_s, boost::hana::type_c<double>)
			);
		};
		auto BYTES_PER_ELEMENT() noexcept;
		auto buffer() noexcept;
		auto byteLength() noexcept;
		auto byteOffset() noexcept;
		auto length() noexcept;
		static auto _tcjs_construct() noexcept;
		static auto _tcjs_construct(double length) noexcept;
		static auto _tcjs_construct(tc::js::any /*UnionWithUnsupportedSubtype=[_js_jArrayBuffer,tc::js::any,_js_jArrayLike<double>]*/ array) noexcept;
		static auto _tcjs_construct(_js_jArrayBufferLike buffer, tc::jst::union_t<double, tc::js::undefined> byteOffset = tc::js::undefined(), tc::jst::union_t<double, tc::js::undefined> length = tc::js::undefined()) noexcept;
		static auto _tcjs_construct(_js_jIterable<double> elements) noexcept;
		auto copyWithin(double target, double start, tc::jst::union_t<double, tc::js::undefined> end = tc::js::undefined()) noexcept;
		auto entries() noexcept;
		auto every(tc::jst::function<tc::js::any(double, double, _js_jFloat32Array)> predicate, tc::js::any thisArg) noexcept;
		auto fill(double value, tc::jst::union_t<double, tc::js::undefined> start = tc::js::undefined(), tc::jst::union_t<double, tc::js::undefined> end = tc::js::undefined()) noexcept;
		auto filter(tc::jst::function<tc::js::any(double, double, _js_jFloat32Array)> predicate, tc::js::any thisArg) noexcept;
		auto find(tc::jst::function<bool /*false*/(double, double, _js_jFloat32Array)> predicate, tc::js::any thisArg) noexcept;
		auto findIndex(tc::jst::function<bool /*false*/(double, double, _js_jFloat32Array)> predicate, tc::js::any thisArg) noexcept;
		auto forEach(tc::jst::function<void(double, double, _js_jFloat32Array)> callbackfn, tc::js::any thisArg) noexcept;
		auto includes(double searchElement, tc::jst::union_t<double, tc::js::undefined> fromIndex = tc::js::undefined()) noexcept;
		auto indexOf(double searchElement, tc::jst::union_t<double, tc::js::undefined> fromIndex = tc::js::undefined()) noexcept;
		auto join(tc::jst::union_t<tc::js::string, tc::js::undefined> separator = tc::js::undefined()) noexcept;
		auto keys() noexcept;
		auto lastIndexOf(double searchElement, tc::jst::union_t<double, tc::js::undefined> fromIndex = tc::js::undefined()) noexcept;
		auto map(tc::jst::function<double(double, double, _js_jFloat32Array)> callbackfn, tc::js::any thisArg) noexcept;
		auto operator[](double index) noexcept;
		template<typename U>
		auto reduce(tc::jst::function<U(U, double, double, _js_jFloat32Array)> callbackfn, U initialValue) noexcept;
		auto reduce(tc::jst::function<double(double, double, double, _js_jFloat32Array)> callbackfn) noexcept;
		auto reduce(tc::jst::function<double(double, double, double, _js_jFloat32Array)> callbackfn, double initialValue) noexcept;
		template<typename U>
		auto reduceRight(tc::jst::function<U(U, double, double, _js_jFloat32Array)> callbackfn, U initialValue) noexcept;
		auto reduceRight(tc::jst::function<double(double, double, double, _js_jFloat32Array)> callbackfn) noexcept;
		auto reduceRight(tc::jst::function<double(double, double, double, _js_jFloat32Array)> callbackfn, double initialValue) noexcept;
		auto reverse() noexcept;
		auto set(_js_jArrayLike<double> array, tc::jst::union_t<double, tc::js::undefined> offset = tc::js::undefined()) noexcept;
		auto slice(tc::jst::union_t<double, tc::js::undefined> start = tc::js::undefined(), tc::jst::union_t<double, tc::js::undefined> end = tc::js::undefined()) noexcept;
		auto some(tc::jst::function<tc::js::any(double, double, _js_jFloat32Array)> predicate, tc::js::any thisArg) noexcept;
		auto sort(tc::jst::union_t<tc::js::undefined, tc::jst::function<double(double, double)>> compareFn = tc::js::undefined()) noexcept;
		auto subarray(tc::jst::union_t<double, tc::js::undefined> begin = tc::js::undefined(), tc::jst::union_t<double, tc::js::undefined> end = tc::js::undefined()) noexcept;
		auto toLocaleString() noexcept;
		auto toString() noexcept;
		auto valueOf() noexcept;
		auto values() noexcept;
	};
	struct _impl_js_jFloat32ArrayConstructor : virtual object_base {
		struct _tcjs_definitions {
			static constexpr auto keyof = boost::hana::make_map(
				boost::hana::make_pair("prototype"_s, boost::hana::type_c<_js_jFloat32Array>),
				boost::hana::make_pair("BYTES_PER_ELEMENT"_s, boost::hana::type_c<double>)
			);
		};
		auto prototype() noexcept;
		auto BYTES_PER_ELEMENT() noexcept;
		template<typename T>
		auto from(_js_jArrayLike<T> arrayLike, tc::jst::function<double(T, double)> mapfn, tc::js::any thisArg) noexcept;
		auto from(_js_jArrayLike<double> arrayLike) noexcept;
		auto from(_js_jIterable<double> arrayLike, tc::jst::union_t<tc::js::undefined, tc::jst::function<double(double, double)>> mapfn, tc::js::any thisArg) noexcept;
		auto of(tc::jst::variadic_arg_t, double items) noexcept;
	};
	struct _impl_js_jFloat64Array : virtual object_base {
		struct _tcjs_definitions {
			static constexpr auto keyof = boost::hana::make_map(
				boost::hana::make_pair("BYTES_PER_ELEMENT"_s, boost::hana::type_c<double>),
				boost::hana::make_pair("buffer"_s, boost::hana::type_c<_js_jArrayBufferLike>),
				boost::hana::make_pair("byteLength"_s, boost::hana::type_c<double>),
				boost::hana::make_pair("byteOffset"_s, boost::hana::type_c<double>),
				boost::hana::make_pair("length"_s, boost::hana::type_c<double>)
			);
		};
		auto BYTES_PER_ELEMENT() noexcept;
		auto buffer() noexcept;
		auto byteLength() noexcept;
		auto byteOffset() noexcept;
		auto length() noexcept;
		static auto _tcjs_construct() noexcept;
		static auto _tcjs_construct(double length) noexcept;
		static auto _tcjs_construct(tc::js::any /*UnionWithUnsupportedSubtype=[_js_jArrayBuffer,tc::js::any,_js_jArrayLike<double>]*/ array) noexcept;
		static auto _tcjs_construct(_js_jArrayBufferLike buffer, tc::jst::union_t<double, tc::js::undefined> byteOffset = tc::js::undefined(), tc::jst::union_t<double, tc::js::undefined> length = tc::js::undefined()) noexcept;
		static auto _tcjs_construct(_js_jIterable<double> elements) noexcept;
		auto copyWithin(double target, double start, tc::jst::union_t<double, tc::js::undefined> end = tc::js::undefined()) noexcept;
		auto entries() noexcept;
		auto every(tc::jst::function<tc::js::any(double, double, _js_jFloat64Array)> predicate, tc::js::any thisArg) noexcept;
		auto fill(double value, tc::jst::union_t<double, tc::js::undefined> start = tc::js::undefined(), tc::jst::union_t<double, tc::js::undefined> end = tc::js::undefined()) noexcept;
		auto filter(tc::jst::function<tc::js::any(double, double, _js_jFloat64Array)> predicate, tc::js::any thisArg) noexcept;
		auto find(tc::jst::function<bool /*false*/(double, double, _js_jFloat64Array)> predicate, tc::js::any thisArg) noexcept;
		auto findIndex(tc::jst::function<bool /*false*/(double, double, _js_jFloat64Array)> predicate, tc::js::any thisArg) noexcept;
		auto forEach(tc::jst::function<void(double, double, _js_jFloat64Array)> callbackfn, tc::js::any thisArg) noexcept;
		auto includes(double searchElement, tc::jst::union_t<double, tc::js::undefined> fromIndex = tc::js::undefined()) noexcept;
		auto indexOf(double searchElement, tc::jst::union_t<double, tc::js::undefined> fromIndex = tc::js::undefined()) noexcept;
		auto join(tc::jst::union_t<tc::js::string, tc::js::undefined> separator = tc::js::undefined()) noexcept;
		auto keys() noexcept;
		auto lastIndexOf(double searchElement, tc::jst::union_t<double, tc::js::undefined> fromIndex = tc::js::undefined()) noexcept;
		auto map(tc::jst::function<double(double, double, _js_jFloat64Array)> callbackfn, tc::js::any thisArg) noexcept;
		auto operator[](double index) noexcept;
		template<typename U>
		auto reduce(tc::jst::function<U(U, double, double, _js_jFloat64Array)> callbackfn, U initialValue) noexcept;
		auto reduce(tc::jst::function<double(double, double, double, _js_jFloat64Array)> callbackfn) noexcept;
		auto reduce(tc::jst::function<double(double, double, double, _js_jFloat64Array)> callbackfn, double initialValue) noexcept;
		template<typename U>
		auto reduceRight(tc::jst::function<U(U, double, double, _js_jFloat64Array)> callbackfn, U initialValue) noexcept;
		auto reduceRight(tc::jst::function<double(double, double, double, _js_jFloat64Array)> callbackfn) noexcept;
		auto reduceRight(tc::jst::function<double(double, double, double, _js_jFloat64Array)> callbackfn, double initialValue) noexcept;
		auto reverse() noexcept;
		auto set(_js_jArrayLike<double> array, tc::jst::union_t<double, tc::js::undefined> offset = tc::js::undefined()) noexcept;
		auto slice(tc::jst::union_t<double, tc::js::undefined> start = tc::js::undefined(), tc::jst::union_t<double, tc::js::undefined> end = tc::js::undefined()) noexcept;
		auto some(tc::jst::function<tc::js::any(double, double, _js_jFloat64Array)> predicate, tc::js::any thisArg) noexcept;
		auto sort(tc::jst::union_t<tc::js::undefined, tc::jst::function<double(double, double)>> compareFn = tc::js::undefined()) noexcept;
		auto subarray(tc::jst::union_t<double, tc::js::undefined> begin = tc::js::undefined(), tc::jst::union_t<double, tc::js::undefined> end = tc::js::undefined()) noexcept;
		auto toString() noexcept;
		auto valueOf() noexcept;
		auto values() noexcept;
	};
	struct _impl_js_jFloat64ArrayConstructor : virtual object_base {
		struct _tcjs_definitions {
			static constexpr auto keyof = boost::hana::make_map(
				boost::hana::make_pair("prototype"_s, boost::hana::type_c<_js_jFloat64Array>),
				boost::hana::make_pair("BYTES_PER_ELEMENT"_s, boost::hana::type_c<double>)
			);
		};
		auto prototype() noexcept;
		auto BYTES_PER_ELEMENT() noexcept;
		template<typename T>
		auto from(_js_jArrayLike<T> arrayLike, tc::jst::function<double(T, double)> mapfn, tc::js::any thisArg) noexcept;
		auto from(_js_jArrayLike<double> arrayLike) noexcept;
		auto from(_js_jIterable<double> arrayLike, tc::jst::union_t<tc::js::undefined, tc::jst::function<double(double, double)>> mapfn, tc::js::any thisArg) noexcept;
		auto of(tc::jst::variadic_arg_t, double items) noexcept;
	};
	struct _impl_js_jFunctionConstructor : virtual object_base {
		struct _tcjs_definitions {
			static constexpr auto keyof = boost::hana::make_map(
				boost::hana::make_pair("prototype"_s, boost::hana::type_c<_js_jFunction>)
			);
		};
		auto prototype() noexcept;
	};
	template<typename T, typename TReturn, typename TNext>
	struct _impl_js_jIterator : virtual object_base {
		struct _tcjs_definitions {
			static constexpr auto keyof = boost::hana::make_map(

			);
		};
		auto next(tc::jst::union_t<std::tuple<>, std::tuple<TNext>> args) noexcept;
	};
	template<typename T, typename TReturn, typename TNext>
	struct _impl_js_jGenerator : virtual _impl_js_jIterator<T, TReturn, TNext> {
		struct _tcjs_definitions {
			static constexpr auto keyof = boost::hana::make_map(

			);
		};
		auto next(tc::jst::union_t<std::tuple<>, std::tuple<TNext>> args) noexcept;
		auto return_(TReturn value) noexcept;
		auto throw_(tc::js::any e) noexcept;
	};
	struct _impl_js_jGeneratorFunction : virtual object_base {
		struct _tcjs_definitions {
			static constexpr auto keyof = boost::hana::make_map(
				boost::hana::make_pair("length"_s, boost::hana::type_c<double>),
				boost::hana::make_pair("name"_s, boost::hana::type_c<tc::js::string>),
				boost::hana::make_pair("prototype"_s, boost::hana::type_c<_js_jGenerator<tc::js::any, tc::js::any, tc::js::any>>)
			);
		};
		auto length() noexcept;
		auto name() noexcept;
		auto prototype() noexcept;
	};
	struct _impl_js_jGeneratorFunctionConstructor : virtual object_base {
		struct _tcjs_definitions {
			static constexpr auto keyof = boost::hana::make_map(
				boost::hana::make_pair("length"_s, boost::hana::type_c<double>),
				boost::hana::make_pair("name"_s, boost::hana::type_c<tc::js::string>),
				boost::hana::make_pair("prototype"_s, boost::hana::type_c<_js_jGeneratorFunction>)
			);
		};
		auto length() noexcept;
		auto name() noexcept;
		auto prototype() noexcept;
	};
	struct _impl_js_jIArguments : virtual object_base {
		struct _tcjs_definitions {
			static constexpr auto keyof = boost::hana::make_map(
				boost::hana::make_pair("length"_s, boost::hana::type_c<double>),
				boost::hana::make_pair("callee"_s, boost::hana::type_c<_js_jFunction>)
			);
		};
		auto length() noexcept;
		void length(double v) noexcept;
		auto callee() noexcept;
		void callee(_js_jFunction v) noexcept;
		auto operator[](double index) noexcept;
	};
	struct _impl_js_jImportMeta : virtual object_base {
		struct _tcjs_definitions {
			static constexpr auto keyof = boost::hana::make_map(
				boost::hana::make_pair("url"_s, boost::hana::type_c<tc::js::string>)
			);
		};
		auto url() noexcept;
		void url(tc::js::string v) noexcept;
	};
	struct _impl_js_jInt16Array : virtual object_base {
		struct _tcjs_definitions {
			static constexpr auto keyof = boost::hana::make_map(
				boost::hana::make_pair("BYTES_PER_ELEMENT"_s, boost::hana::type_c<double>),
				boost::hana::make_pair("buffer"_s, boost::hana::type_c<_js_jArrayBufferLike>),
				boost::hana::make_pair("byteLength"_s, boost::hana::type_c<double>),
				boost::hana::make_pair("byteOffset"_s, boost::hana::type_c<double>),
				boost::hana::make_pair("length"_s, boost::hana::type_c<double>)
			);
		};
		auto BYTES_PER_ELEMENT() noexcept;
		auto buffer() noexcept;
		auto byteLength() noexcept;
		auto byteOffset() noexcept;
		auto length() noexcept;
		static auto _tcjs_construct() noexcept;
		static auto _tcjs_construct(double length) noexcept;
		static auto _tcjs_construct(tc::js::any /*UnionWithUnsupportedSubtype=[_js_jArrayBuffer,tc::js::any,_js_jArrayLike<double>]*/ array) noexcept;
		static auto _tcjs_construct(_js_jArrayBufferLike buffer, tc::jst::union_t<double, tc::js::undefined> byteOffset = tc::js::undefined(), tc::jst::union_t<double, tc::js::undefined> length = tc::js::undefined()) noexcept;
		static auto _tcjs_construct(_js_jIterable<double> elements) noexcept;
		auto copyWithin(double target, double start, tc::jst::union_t<double, tc::js::undefined> end = tc::js::undefined()) noexcept;
		auto entries() noexcept;
		auto every(tc::jst::function<tc::js::any(double, double, _js_jInt16Array)> predicate, tc::js::any thisArg) noexcept;
		auto fill(double value, tc::jst::union_t<double, tc::js::undefined> start = tc::js::undefined(), tc::jst::union_t<double, tc::js::undefined> end = tc::js::undefined()) noexcept;
		auto filter(tc::jst::function<tc::js::any(double, double, _js_jInt16Array)> predicate, tc::js::any thisArg) noexcept;
		auto find(tc::jst::function<bool /*false*/(double, double, _js_jInt16Array)> predicate, tc::js::any thisArg) noexcept;
		auto findIndex(tc::jst::function<bool /*false*/(double, double, _js_jInt16Array)> predicate, tc::js::any thisArg) noexcept;
		auto forEach(tc::jst::function<void(double, double, _js_jInt16Array)> callbackfn, tc::js::any thisArg) noexcept;
		auto includes(double searchElement, tc::jst::union_t<double, tc::js::undefined> fromIndex = tc::js::undefined()) noexcept;
		auto indexOf(double searchElement, tc::jst::union_t<double, tc::js::undefined> fromIndex = tc::js::undefined()) noexcept;
		auto join(tc::jst::union_t<tc::js::string, tc::js::undefined> separator = tc::js::undefined()) noexcept;
		auto keys() noexcept;
		auto lastIndexOf(double searchElement, tc::jst::union_t<double, tc::js::undefined> fromIndex = tc::js::undefined()) noexcept;
		auto map(tc::jst::function<double(double, double, _js_jInt16Array)> callbackfn, tc::js::any thisArg) noexcept;
		auto operator[](double index) noexcept;
		template<typename U>
		auto reduce(tc::jst::function<U(U, double, double, _js_jInt16Array)> callbackfn, U initialValue) noexcept;
		auto reduce(tc::jst::function<double(double, double, double, _js_jInt16Array)> callbackfn) noexcept;
		auto reduce(tc::jst::function<double(double, double, double, _js_jInt16Array)> callbackfn, double initialValue) noexcept;
		template<typename U>
		auto reduceRight(tc::jst::function<U(U, double, double, _js_jInt16Array)> callbackfn, U initialValue) noexcept;
		auto reduceRight(tc::jst::function<double(double, double, double, _js_jInt16Array)> callbackfn) noexcept;
		auto reduceRight(tc::jst::function<double(double, double, double, _js_jInt16Array)> callbackfn, double initialValue) noexcept;
		auto reverse() noexcept;
		auto set(_js_jArrayLike<double> array, tc::jst::union_t<double, tc::js::undefined> offset = tc::js::undefined()) noexcept;
		auto slice(tc::jst::union_t<double, tc::js::undefined> start = tc::js::undefined(), tc::jst::union_t<double, tc::js::undefined> end = tc::js::undefined()) noexcept;
		auto some(tc::jst::function<tc::js::any(double, double, _js_jInt16Array)> predicate, tc::js::any thisArg) noexcept;
		auto sort(tc::jst::union_t<tc::js::undefined, tc::jst::function<double(double, double)>> compareFn = tc::js::undefined()) noexcept;
		auto subarray(tc::jst::union_t<double, tc::js::undefined> begin = tc::js::undefined(), tc::jst::union_t<double, tc::js::undefined> end = tc::js::undefined()) noexcept;
		auto toLocaleString() noexcept;
		auto toString() noexcept;
		auto valueOf() noexcept;
		auto values() noexcept;
	};
	struct _impl_js_jInt16ArrayConstructor : virtual object_base {
		struct _tcjs_definitions {
			static constexpr auto keyof = boost::hana::make_map(
				boost::hana::make_pair("prototype"_s, boost::hana::type_c<_js_jInt16Array>),
				boost::hana::make_pair("BYTES_PER_ELEMENT"_s, boost::hana::type_c<double>)
			);
		};
		auto prototype() noexcept;
		auto BYTES_PER_ELEMENT() noexcept;
		template<typename T>
		auto from(_js_jArrayLike<T> arrayLike, tc::jst::function<double(T, double)> mapfn, tc::js::any thisArg) noexcept;
		auto from(_js_jArrayLike<double> arrayLike) noexcept;
		auto from(_js_jIterable<double> arrayLike, tc::jst::union_t<tc::js::undefined, tc::jst::function<double(double, double)>> mapfn, tc::js::any thisArg) noexcept;
		auto of(tc::jst::variadic_arg_t, double items) noexcept;
	};
	struct _impl_js_jInt32Array : virtual object_base {
		struct _tcjs_definitions {
			static constexpr auto keyof = boost::hana::make_map(
				boost::hana::make_pair("BYTES_PER_ELEMENT"_s, boost::hana::type_c<double>),
				boost::hana::make_pair("buffer"_s, boost::hana::type_c<_js_jArrayBufferLike>),
				boost::hana::make_pair("byteLength"_s, boost::hana::type_c<double>),
				boost::hana::make_pair("byteOffset"_s, boost::hana::type_c<double>),
				boost::hana::make_pair("length"_s, boost::hana::type_c<double>)
			);
		};
		auto BYTES_PER_ELEMENT() noexcept;
		auto buffer() noexcept;
		auto byteLength() noexcept;
		auto byteOffset() noexcept;
		auto length() noexcept;
		static auto _tcjs_construct() noexcept;
		static auto _tcjs_construct(double length) noexcept;
		static auto _tcjs_construct(tc::js::any /*UnionWithUnsupportedSubtype=[_js_jArrayBuffer,tc::js::any,_js_jArrayLike<double>]*/ array) noexcept;
		static auto _tcjs_construct(_js_jArrayBufferLike buffer, tc::jst::union_t<double, tc::js::undefined> byteOffset = tc::js::undefined(), tc::jst::union_t<double, tc::js::undefined> length = tc::js::undefined()) noexcept;
		static auto _tcjs_construct(_js_jIterable<double> elements) noexcept;
		auto copyWithin(double target, double start, tc::jst::union_t<double, tc::js::undefined> end = tc::js::undefined()) noexcept;
		auto entries() noexcept;
		auto every(tc::jst::function<tc::js::any(double, double, _js_jInt32Array)> predicate, tc::js::any thisArg) noexcept;
		auto fill(double value, tc::jst::union_t<double, tc::js::undefined> start = tc::js::undefined(), tc::jst::union_t<double, tc::js::undefined> end = tc::js::undefined()) noexcept;
		auto filter(tc::jst::function<tc::js::any(double, double, _js_jInt32Array)> predicate, tc::js::any thisArg) noexcept;
		auto find(tc::jst::function<bool /*false*/(double, double, _js_jInt32Array)> predicate, tc::js::any thisArg) noexcept;
		auto findIndex(tc::jst::function<bool /*false*/(double, double, _js_jInt32Array)> predicate, tc::js::any thisArg) noexcept;
		auto forEach(tc::jst::function<void(double, double, _js_jInt32Array)> callbackfn, tc::js::any thisArg) noexcept;
		auto includes(double searchElement, tc::jst::union_t<double, tc::js::undefined> fromIndex = tc::js::undefined()) noexcept;
		auto indexOf(double searchElement, tc::jst::union_t<double, tc::js::undefined> fromIndex = tc::js::undefined()) noexcept;
		auto join(tc::jst::union_t<tc::js::string, tc::js::undefined> separator = tc::js::undefined()) noexcept;
		auto keys() noexcept;
		auto lastIndexOf(double searchElement, tc::jst::union_t<double, tc::js::undefined> fromIndex = tc::js::undefined()) noexcept;
		auto map(tc::jst::function<double(double, double, _js_jInt32Array)> callbackfn, tc::js::any thisArg) noexcept;
		auto operator[](double index) noexcept;
		template<typename U>
		auto reduce(tc::jst::function<U(U, double, double, _js_jInt32Array)> callbackfn, U initialValue) noexcept;
		auto reduce(tc::jst::function<double(double, double, double, _js_jInt32Array)> callbackfn) noexcept;
		auto reduce(tc::jst::function<double(double, double, double, _js_jInt32Array)> callbackfn, double initialValue) noexcept;
		template<typename U>
		auto reduceRight(tc::jst::function<U(U, double, double, _js_jInt32Array)> callbackfn, U initialValue) noexcept;
		auto reduceRight(tc::jst::function<double(double, double, double, _js_jInt32Array)> callbackfn) noexcept;
		auto reduceRight(tc::jst::function<double(double, double, double, _js_jInt32Array)> callbackfn, double initialValue) noexcept;
		auto reverse() noexcept;
		auto set(_js_jArrayLike<double> array, tc::jst::union_t<double, tc::js::undefined> offset = tc::js::undefined()) noexcept;
		auto slice(tc::jst::union_t<double, tc::js::undefined> start = tc::js::undefined(), tc::jst::union_t<double, tc::js::undefined> end = tc::js::undefined()) noexcept;
		auto some(tc::jst::function<tc::js::any(double, double, _js_jInt32Array)> predicate, tc::js::any thisArg) noexcept;
		auto sort(tc::jst::union_t<tc::js::undefined, tc::jst::function<double(double, double)>> compareFn = tc::js::undefined()) noexcept;
		auto subarray(tc::jst::union_t<double, tc::js::undefined> begin = tc::js::undefined(), tc::jst::union_t<double, tc::js::undefined> end = tc::js::undefined()) noexcept;
		auto toLocaleString() noexcept;
		auto toString() noexcept;
		auto valueOf() noexcept;
		auto values() noexcept;
	};
	struct _impl_js_jInt32ArrayConstructor : virtual object_base {
		struct _tcjs_definitions {
			static constexpr auto keyof = boost::hana::make_map(
				boost::hana::make_pair("prototype"_s, boost::hana::type_c<_js_jInt32Array>),
				boost::hana::make_pair("BYTES_PER_ELEMENT"_s, boost::hana::type_c<double>)
			);
		};
		auto prototype() noexcept;
		auto BYTES_PER_ELEMENT() noexcept;
		template<typename T>
		auto from(_js_jArrayLike<T> arrayLike, tc::jst::function<double(T, double)> mapfn, tc::js::any thisArg) noexcept;
		auto from(_js_jArrayLike<double> arrayLike) noexcept;
		auto from(_js_jIterable<double> arrayLike, tc::jst::union_t<tc::js::undefined, tc::jst::function<double(double, double)>> mapfn, tc::js::any thisArg) noexcept;
		auto of(tc::jst::variadic_arg_t, double items) noexcept;
	};
	struct _impl_js_jInt8Array : virtual object_base {
		struct _tcjs_definitions {
			static constexpr auto keyof = boost::hana::make_map(
				boost::hana::make_pair("BYTES_PER_ELEMENT"_s, boost::hana::type_c<double>),
				boost::hana::make_pair("buffer"_s, boost::hana::type_c<_js_jArrayBufferLike>),
				boost::hana::make_pair("byteLength"_s, boost::hana::type_c<double>),
				boost::hana::make_pair("byteOffset"_s, boost::hana::type_c<double>),
				boost::hana::make_pair("length"_s, boost::hana::type_c<double>)
			);
		};
		auto BYTES_PER_ELEMENT() noexcept;
		auto buffer() noexcept;
		auto byteLength() noexcept;
		auto byteOffset() noexcept;
		auto length() noexcept;
		static auto _tcjs_construct() noexcept;
		static auto _tcjs_construct(double length) noexcept;
		static auto _tcjs_construct(tc::js::any /*UnionWithUnsupportedSubtype=[_js_jArrayBuffer,tc::js::any,_js_jArrayLike<double>]*/ array) noexcept;
		static auto _tcjs_construct(_js_jArrayBufferLike buffer, tc::jst::union_t<double, tc::js::undefined> byteOffset = tc::js::undefined(), tc::jst::union_t<double, tc::js::undefined> length = tc::js::undefined()) noexcept;
		static auto _tcjs_construct(_js_jIterable<double> elements) noexcept;
		auto copyWithin(double target, double start, tc::jst::union_t<double, tc::js::undefined> end = tc::js::undefined()) noexcept;
		auto entries() noexcept;
		auto every(tc::jst::function<tc::js::any(double, double, _js_jInt8Array)> predicate, tc::js::any thisArg) noexcept;
		auto fill(double value, tc::jst::union_t<double, tc::js::undefined> start = tc::js::undefined(), tc::jst::union_t<double, tc::js::undefined> end = tc::js::undefined()) noexcept;
		auto filter(tc::jst::function<tc::js::any(double, double, _js_jInt8Array)> predicate, tc::js::any thisArg) noexcept;
		auto find(tc::jst::function<bool /*false*/(double, double, _js_jInt8Array)> predicate, tc::js::any thisArg) noexcept;
		auto findIndex(tc::jst::function<bool /*false*/(double, double, _js_jInt8Array)> predicate, tc::js::any thisArg) noexcept;
		auto forEach(tc::jst::function<void(double, double, _js_jInt8Array)> callbackfn, tc::js::any thisArg) noexcept;
		auto includes(double searchElement, tc::jst::union_t<double, tc::js::undefined> fromIndex = tc::js::undefined()) noexcept;
		auto indexOf(double searchElement, tc::jst::union_t<double, tc::js::undefined> fromIndex = tc::js::undefined()) noexcept;
		auto join(tc::jst::union_t<tc::js::string, tc::js::undefined> separator = tc::js::undefined()) noexcept;
		auto keys() noexcept;
		auto lastIndexOf(double searchElement, tc::jst::union_t<double, tc::js::undefined> fromIndex = tc::js::undefined()) noexcept;
		auto map(tc::jst::function<double(double, double, _js_jInt8Array)> callbackfn, tc::js::any thisArg) noexcept;
		auto operator[](double index) noexcept;
		template<typename U>
		auto reduce(tc::jst::function<U(U, double, double, _js_jInt8Array)> callbackfn, U initialValue) noexcept;
		auto reduce(tc::jst::function<double(double, double, double, _js_jInt8Array)> callbackfn) noexcept;
		auto reduce(tc::jst::function<double(double, double, double, _js_jInt8Array)> callbackfn, double initialValue) noexcept;
		template<typename U>
		auto reduceRight(tc::jst::function<U(U, double, double, _js_jInt8Array)> callbackfn, U initialValue) noexcept;
		auto reduceRight(tc::jst::function<double(double, double, double, _js_jInt8Array)> callbackfn) noexcept;
		auto reduceRight(tc::jst::function<double(double, double, double, _js_jInt8Array)> callbackfn, double initialValue) noexcept;
		auto reverse() noexcept;
		auto set(_js_jArrayLike<double> array, tc::jst::union_t<double, tc::js::undefined> offset = tc::js::undefined()) noexcept;
		auto slice(tc::jst::union_t<double, tc::js::undefined> start = tc::js::undefined(), tc::jst::union_t<double, tc::js::undefined> end = tc::js::undefined()) noexcept;
		auto some(tc::jst::function<tc::js::any(double, double, _js_jInt8Array)> predicate, tc::js::any thisArg) noexcept;
		auto sort(tc::jst::union_t<tc::js::undefined, tc::jst::function<double(double, double)>> compareFn = tc::js::undefined()) noexcept;
		auto subarray(tc::jst::union_t<double, tc::js::undefined> begin = tc::js::undefined(), tc::jst::union_t<double, tc::js::undefined> end = tc::js::undefined()) noexcept;
		auto toLocaleString() noexcept;
		auto toString() noexcept;
		auto valueOf() noexcept;
		auto values() noexcept;
	};
	struct _impl_js_jInt8ArrayConstructor : virtual object_base {
		struct _tcjs_definitions {
			static constexpr auto keyof = boost::hana::make_map(
				boost::hana::make_pair("prototype"_s, boost::hana::type_c<_js_jInt8Array>),
				boost::hana::make_pair("BYTES_PER_ELEMENT"_s, boost::hana::type_c<double>)
			);
		};
		auto prototype() noexcept;
		auto BYTES_PER_ELEMENT() noexcept;
		template<typename T>
		auto from(_js_jArrayLike<T> arrayLike, tc::jst::function<double(T, double)> mapfn, tc::js::any thisArg) noexcept;
		auto from(_js_jArrayLike<double> arrayLike) noexcept;
		auto from(_js_jIterable<double> arrayLike, tc::jst::union_t<tc::js::undefined, tc::jst::function<double(double, double)>> mapfn, tc::js::any thisArg) noexcept;
		auto of(tc::jst::variadic_arg_t, double items) noexcept;
	};
	struct _impl_js_jIntl : virtual object_base {
		struct _tcjs_definitions {
			static constexpr auto keyof = boost::hana::make_map(

			);
			using DateTimeFormatPart = _js_jIntl_dDateTimeFormatPart;
			using DateTimeFormat = _js_jIntl_dDateTimeFormat;
			using PluralRulesOptions = _js_jIntl_dPluralRulesOptions;
			using ResolvedPluralRulesOptions = _js_jIntl_dResolvedPluralRulesOptions;
			using PluralRules = _js_jIntl_dPluralRules;
			using NumberFormat = _js_jIntl_dNumberFormat;
			using NumberFormatPart = _js_jIntl_dNumberFormatPart;
			using CollatorOptions = _js_jIntl_dCollatorOptions;
			using ResolvedCollatorOptions = _js_jIntl_dResolvedCollatorOptions;
			using Collator = _js_jIntl_dCollator;
			using NumberFormatOptions = _js_jIntl_dNumberFormatOptions;
			using ResolvedNumberFormatOptions = _js_jIntl_dResolvedNumberFormatOptions;
			using DateTimeFormatOptions = _js_jIntl_dDateTimeFormatOptions;
			using ResolvedDateTimeFormatOptions = _js_jIntl_dResolvedDateTimeFormatOptions;
		};
	};
	struct _impl_js_jIntl_dCollator : virtual object_base {
		struct _tcjs_definitions {
			static constexpr auto keyof = boost::hana::make_map(

			);
		};
		static auto _tcjs_construct(tc::jst::union_t<_js_jArray<tc::js::string>, tc::js::string, tc::js::undefined> locales = tc::js::undefined(), tc::jst::union_t<_js_jIntl_dCollatorOptions, tc::js::undefined> options = tc::js::undefined()) noexcept;
		auto compare(tc::js::string x, tc::js::string y) noexcept;
		auto resolvedOptions() noexcept;
	};
	struct _impl_js_jIntl_dCollatorOptions : virtual object_base {
		struct _tcjs_definitions {
			static constexpr auto keyof = boost::hana::make_map(
				boost::hana::make_pair("usage"_s, boost::hana::type_c<tc::jst::union_t<tc::js::string, tc::js::undefined>>),
				boost::hana::make_pair("localeMatcher"_s, boost::hana::type_c<tc::jst::union_t<tc::js::string, tc::js::undefined>>),
				boost::hana::make_pair("numeric"_s, boost::hana::type_c<tc::jst::union_t<bool /*false*/, tc::js::undefined>>),
				boost::hana::make_pair("caseFirst"_s, boost::hana::type_c<tc::jst::union_t<tc::js::string, tc::js::undefined>>),
				boost::hana::make_pair("sensitivity"_s, boost::hana::type_c<tc::jst::union_t<tc::js::string, tc::js::undefined>>),
				boost::hana::make_pair("ignorePunctuation"_s, boost::hana::type_c<tc::jst::union_t<bool /*false*/, tc::js::undefined>>)
			);
		};
		auto usage() noexcept;
		void usage(tc::jst::union_t<tc::js::string, tc::js::undefined> v) noexcept;
		auto localeMatcher() noexcept;
		void localeMatcher(tc::jst::union_t<tc::js::string, tc::js::undefined> v) noexcept;
		auto numeric() noexcept;
		void numeric(tc::jst::union_t<bool /*false*/, tc::js::undefined> v) noexcept;
		auto caseFirst() noexcept;
		void caseFirst(tc::jst::union_t<tc::js::string, tc::js::undefined> v) noexcept;
		auto sensitivity() noexcept;
		void sensitivity(tc::jst::union_t<tc::js::string, tc::js::undefined> v) noexcept;
		auto ignorePunctuation() noexcept;
		void ignorePunctuation(tc::jst::union_t<bool /*false*/, tc::js::undefined> v) noexcept;
		static auto _tcjs_construct() noexcept;
	};
	struct _impl_js_jIntl_dDateTimeFormat : virtual object_base {
		struct _tcjs_definitions {
			static constexpr auto keyof = boost::hana::make_map(

			);
		};
		static auto _tcjs_construct(tc::jst::union_t<_js_jArray<tc::js::string>, tc::js::string, tc::js::undefined> locales = tc::js::undefined(), tc::jst::union_t<_js_jIntl_dDateTimeFormatOptions, tc::js::undefined> options = tc::js::undefined()) noexcept;
		auto format(tc::jst::union_t<_js_jDate, double, tc::js::undefined> date = tc::js::undefined()) noexcept;
		auto formatToParts(tc::jst::union_t<_js_jDate, double, tc::js::undefined> date = tc::js::undefined()) noexcept;
		auto resolvedOptions() noexcept;
	};
	struct _impl_js_jIntl_dDateTimeFormatOptions : virtual object_base {
		struct _tcjs_definitions {
			static constexpr auto keyof = boost::hana::make_map(
				boost::hana::make_pair("localeMatcher"_s, boost::hana::type_c<tc::jst::union_t<tc::js::string /*"lookup"*/, tc::js::undefined>>),
				boost::hana::make_pair("weekday"_s, boost::hana::type_c<tc::jst::union_t<tc::js::string /*"long"*/, tc::js::undefined>>),
				boost::hana::make_pair("era"_s, boost::hana::type_c<tc::jst::union_t<tc::js::string /*"long"*/, tc::js::undefined>>),
				boost::hana::make_pair("year"_s, boost::hana::type_c<tc::jst::union_t<tc::js::string /*"numeric"*/, tc::js::undefined>>),
				boost::hana::make_pair("month"_s, boost::hana::type_c<tc::jst::union_t<tc::js::string /*"long"*/, tc::js::undefined>>),
				boost::hana::make_pair("day"_s, boost::hana::type_c<tc::jst::union_t<tc::js::string /*"numeric"*/, tc::js::undefined>>),
				boost::hana::make_pair("hour"_s, boost::hana::type_c<tc::jst::union_t<tc::js::string /*"numeric"*/, tc::js::undefined>>),
				boost::hana::make_pair("minute"_s, boost::hana::type_c<tc::jst::union_t<tc::js::string /*"numeric"*/, tc::js::undefined>>),
				boost::hana::make_pair("second"_s, boost::hana::type_c<tc::jst::union_t<tc::js::string /*"numeric"*/, tc::js::undefined>>),
				boost::hana::make_pair("timeZoneName"_s, boost::hana::type_c<tc::jst::union_t<tc::js::string /*"long"*/, tc::js::undefined>>),
				boost::hana::make_pair("formatMatcher"_s, boost::hana::type_c<tc::jst::union_t<tc::js::string /*"best fit"*/, tc::js::undefined>>),
				boost::hana::make_pair("hour12"_s, boost::hana::type_c<tc::jst::union_t<bool /*false*/, tc::js::undefined>>),
				boost::hana::make_pair("timeZone"_s, boost::hana::type_c<tc::jst::union_t<tc::js::string, tc::js::undefined>>)
			);
		};
		auto localeMatcher() noexcept;
		void localeMatcher(tc::jst::union_t<tc::js::string /*"lookup"*/, tc::js::undefined> v) noexcept;
		auto weekday() noexcept;
		void weekday(tc::jst::union_t<tc::js::string /*"long"*/, tc::js::undefined> v) noexcept;
		auto era() noexcept;
		void era(tc::jst::union_t<tc::js::string /*"long"*/, tc::js::undefined> v) noexcept;
		auto year() noexcept;
		void year(tc::jst::union_t<tc::js::string /*"numeric"*/, tc::js::undefined> v) noexcept;
		auto month() noexcept;
		void month(tc::jst::union_t<tc::js::string /*"long"*/, tc::js::undefined> v) noexcept;
		auto day() noexcept;
		void day(tc::jst::union_t<tc::js::string /*"numeric"*/, tc::js::undefined> v) noexcept;
		auto hour() noexcept;
		void hour(tc::jst::union_t<tc::js::string /*"numeric"*/, tc::js::undefined> v) noexcept;
		auto minute() noexcept;
		void minute(tc::jst::union_t<tc::js::string /*"numeric"*/, tc::js::undefined> v) noexcept;
		auto second() noexcept;
		void second(tc::jst::union_t<tc::js::string /*"numeric"*/, tc::js::undefined> v) noexcept;
		auto timeZoneName() noexcept;
		void timeZoneName(tc::jst::union_t<tc::js::string /*"long"*/, tc::js::undefined> v) noexcept;
		auto formatMatcher() noexcept;
		void formatMatcher(tc::jst::union_t<tc::js::string /*"best fit"*/, tc::js::undefined> v) noexcept;
		auto hour12() noexcept;
		void hour12(tc::jst::union_t<bool /*false*/, tc::js::undefined> v) noexcept;
		auto timeZone() noexcept;
		void timeZone(tc::jst::union_t<tc::js::string, tc::js::undefined> v) noexcept;
		static auto _tcjs_construct() noexcept;
	};
	struct _impl_js_jIntl_dDateTimeFormatPart : virtual object_base {
		struct _tcjs_definitions {
			static constexpr auto keyof = boost::hana::make_map(
				boost::hana::make_pair("type"_s, boost::hana::type_c<tc::js::string /*"day"*/>),
				boost::hana::make_pair("value"_s, boost::hana::type_c<tc::js::string>)
			);
		};
		auto type() noexcept;
		void type(tc::js::string /*"day"*/ v) noexcept;
		auto value() noexcept;
		void value(tc::js::string v) noexcept;
	};
	struct _impl_js_jIntl_dNumberFormat : virtual object_base {
		struct _tcjs_definitions {
			static constexpr auto keyof = boost::hana::make_map(

			);
		};
		static auto _tcjs_construct(tc::jst::union_t<_js_jArray<tc::js::string>, tc::js::string, tc::js::undefined> locales = tc::js::undefined(), tc::jst::union_t<_js_jIntl_dNumberFormatOptions, tc::js::undefined> options = tc::js::undefined()) noexcept;
		auto format(double value) noexcept;
		auto format(tc::js::any /*UnionWithUnsupportedSubtype=[double,tc::js::any]*/ value) noexcept;
		auto formatToParts(tc::js::any /*UnionWithUnsupportedSubtype=[tc::js::undefined,double,tc::js::any]*/ number = tc::js::undefined()) noexcept;
		auto resolvedOptions() noexcept;
	};
	struct _impl_js_jIntl_dNumberFormatOptions : virtual object_base {
		struct _tcjs_definitions {
			static constexpr auto keyof = boost::hana::make_map(
				boost::hana::make_pair("localeMatcher"_s, boost::hana::type_c<tc::jst::union_t<tc::js::string, tc::js::undefined>>),
				boost::hana::make_pair("style"_s, boost::hana::type_c<tc::jst::union_t<tc::js::string, tc::js::undefined>>),
				boost::hana::make_pair("currency"_s, boost::hana::type_c<tc::jst::union_t<tc::js::string, tc::js::undefined>>),
				boost::hana::make_pair("currencyDisplay"_s, boost::hana::type_c<tc::jst::union_t<tc::js::string, tc::js::undefined>>),
				boost::hana::make_pair("currencySign"_s, boost::hana::type_c<tc::jst::union_t<tc::js::string, tc::js::undefined>>),
				boost::hana::make_pair("useGrouping"_s, boost::hana::type_c<tc::jst::union_t<bool /*false*/, tc::js::undefined>>),
				boost::hana::make_pair("minimumIntegerDigits"_s, boost::hana::type_c<tc::jst::union_t<double, tc::js::undefined>>),
				boost::hana::make_pair("minimumFractionDigits"_s, boost::hana::type_c<tc::jst::union_t<double, tc::js::undefined>>),
				boost::hana::make_pair("maximumFractionDigits"_s, boost::hana::type_c<tc::jst::union_t<double, tc::js::undefined>>),
				boost::hana::make_pair("minimumSignificantDigits"_s, boost::hana::type_c<tc::jst::union_t<double, tc::js::undefined>>),
				boost::hana::make_pair("maximumSignificantDigits"_s, boost::hana::type_c<tc::jst::union_t<double, tc::js::undefined>>)
			);
		};
		auto localeMatcher() noexcept;
		void localeMatcher(tc::jst::union_t<tc::js::string, tc::js::undefined> v) noexcept;
		auto style() noexcept;
		void style(tc::jst::union_t<tc::js::string, tc::js::undefined> v) noexcept;
		auto currency() noexcept;
		void currency(tc::jst::union_t<tc::js::string, tc::js::undefined> v) noexcept;
		auto currencyDisplay() noexcept;
		void currencyDisplay(tc::jst::union_t<tc::js::string, tc::js::undefined> v) noexcept;
		auto currencySign() noexcept;
		void currencySign(tc::jst::union_t<tc::js::string, tc::js::undefined> v) noexcept;
		auto useGrouping() noexcept;
		void useGrouping(tc::jst::union_t<bool /*false*/, tc::js::undefined> v) noexcept;
		auto minimumIntegerDigits() noexcept;
		void minimumIntegerDigits(tc::jst::union_t<double, tc::js::undefined> v) noexcept;
		auto minimumFractionDigits() noexcept;
		void minimumFractionDigits(tc::jst::union_t<double, tc::js::undefined> v) noexcept;
		auto maximumFractionDigits() noexcept;
		void maximumFractionDigits(tc::jst::union_t<double, tc::js::undefined> v) noexcept;
		auto minimumSignificantDigits() noexcept;
		void minimumSignificantDigits(tc::jst::union_t<double, tc::js::undefined> v) noexcept;
		auto maximumSignificantDigits() noexcept;
		void maximumSignificantDigits(tc::jst::union_t<double, tc::js::undefined> v) noexcept;
		static auto _tcjs_construct() noexcept;
	};
	struct _impl_js_jIntl_dNumberFormatPart : virtual object_base {
		struct _tcjs_definitions {
			static constexpr auto keyof = boost::hana::make_map(
				boost::hana::make_pair("type"_s, boost::hana::type_c<tc::js::string /*"literal"*/>),
				boost::hana::make_pair("value"_s, boost::hana::type_c<tc::js::string>)
			);
		};
		auto type() noexcept;
		void type(tc::js::string /*"literal"*/ v) noexcept;
		auto value() noexcept;
		void value(tc::js::string v) noexcept;
	};
	struct _impl_js_jIntl_dPluralRules : virtual object_base {
		struct _tcjs_definitions {
			static constexpr auto keyof = boost::hana::make_map(

			);
		};
		static auto _tcjs_construct(tc::jst::union_t<_js_jArray<tc::js::string>, tc::js::string, tc::js::undefined> locales = tc::js::undefined(), tc::jst::union_t<_js_jIntl_dPluralRulesOptions, tc::js::undefined> options = tc::js::undefined()) noexcept;
		auto resolvedOptions() noexcept;
		auto select(double n) noexcept;
	};
	struct _impl_js_jIntl_dPluralRulesOptions : virtual object_base {
		struct _tcjs_definitions {
			static constexpr auto keyof = boost::hana::make_map(
				boost::hana::make_pair("localeMatcher"_s, boost::hana::type_c<tc::jst::union_t<tc::js::string /*"lookup"*/, tc::js::undefined>>),
				boost::hana::make_pair("type"_s, boost::hana::type_c<tc::jst::union_t<tc::js::string /*"cardinal"*/, tc::js::undefined>>),
				boost::hana::make_pair("minimumIntegerDigits"_s, boost::hana::type_c<tc::jst::union_t<double, tc::js::undefined>>),
				boost::hana::make_pair("minimumFractionDigits"_s, boost::hana::type_c<tc::jst::union_t<double, tc::js::undefined>>),
				boost::hana::make_pair("maximumFractionDigits"_s, boost::hana::type_c<tc::jst::union_t<double, tc::js::undefined>>),
				boost::hana::make_pair("minimumSignificantDigits"_s, boost::hana::type_c<tc::jst::union_t<double, tc::js::undefined>>),
				boost::hana::make_pair("maximumSignificantDigits"_s, boost::hana::type_c<tc::jst::union_t<double, tc::js::undefined>>)
			);
		};
		auto localeMatcher() noexcept;
		void localeMatcher(tc::jst::union_t<tc::js::string /*"lookup"*/, tc::js::undefined> v) noexcept;
		auto type() noexcept;
		void type(tc::jst::union_t<tc::js::string /*"cardinal"*/, tc::js::undefined> v) noexcept;
		auto minimumIntegerDigits() noexcept;
		void minimumIntegerDigits(tc::jst::union_t<double, tc::js::undefined> v) noexcept;
		auto minimumFractionDigits() noexcept;
		void minimumFractionDigits(tc::jst::union_t<double, tc::js::undefined> v) noexcept;
		auto maximumFractionDigits() noexcept;
		void maximumFractionDigits(tc::jst::union_t<double, tc::js::undefined> v) noexcept;
		auto minimumSignificantDigits() noexcept;
		void minimumSignificantDigits(tc::jst::union_t<double, tc::js::undefined> v) noexcept;
		auto maximumSignificantDigits() noexcept;
		void maximumSignificantDigits(tc::jst::union_t<double, tc::js::undefined> v) noexcept;
		static auto _tcjs_construct() noexcept;
	};
	struct _impl_js_jIntl_dResolvedCollatorOptions : virtual object_base {
		struct _tcjs_definitions {
			static constexpr auto keyof = boost::hana::make_map(
				boost::hana::make_pair("locale"_s, boost::hana::type_c<tc::js::string>),
				boost::hana::make_pair("usage"_s, boost::hana::type_c<tc::js::string>),
				boost::hana::make_pair("sensitivity"_s, boost::hana::type_c<tc::js::string>),
				boost::hana::make_pair("ignorePunctuation"_s, boost::hana::type_c<bool /*false*/>),
				boost::hana::make_pair("collation"_s, boost::hana::type_c<tc::js::string>),
				boost::hana::make_pair("caseFirst"_s, boost::hana::type_c<tc::js::string>),
				boost::hana::make_pair("numeric"_s, boost::hana::type_c<bool /*false*/>)
			);
		};
		auto locale() noexcept;
		void locale(tc::js::string v) noexcept;
		auto usage() noexcept;
		void usage(tc::js::string v) noexcept;
		auto sensitivity() noexcept;
		void sensitivity(tc::js::string v) noexcept;
		auto ignorePunctuation() noexcept;
		void ignorePunctuation(bool /*false*/ v) noexcept;
		auto collation() noexcept;
		void collation(tc::js::string v) noexcept;
		auto caseFirst() noexcept;
		void caseFirst(tc::js::string v) noexcept;
		auto numeric() noexcept;
		void numeric(bool /*false*/ v) noexcept;
	};
	struct _impl_js_jIntl_dResolvedDateTimeFormatOptions : virtual object_base {
		struct _tcjs_definitions {
			static constexpr auto keyof = boost::hana::make_map(
				boost::hana::make_pair("locale"_s, boost::hana::type_c<tc::js::string>),
				boost::hana::make_pair("calendar"_s, boost::hana::type_c<tc::js::string>),
				boost::hana::make_pair("numberingSystem"_s, boost::hana::type_c<tc::js::string>),
				boost::hana::make_pair("timeZone"_s, boost::hana::type_c<tc::js::string>),
				boost::hana::make_pair("hour12"_s, boost::hana::type_c<tc::jst::union_t<bool /*false*/, tc::js::undefined>>),
				boost::hana::make_pair("weekday"_s, boost::hana::type_c<tc::jst::union_t<tc::js::string, tc::js::undefined>>),
				boost::hana::make_pair("era"_s, boost::hana::type_c<tc::jst::union_t<tc::js::string, tc::js::undefined>>),
				boost::hana::make_pair("year"_s, boost::hana::type_c<tc::jst::union_t<tc::js::string, tc::js::undefined>>),
				boost::hana::make_pair("month"_s, boost::hana::type_c<tc::jst::union_t<tc::js::string, tc::js::undefined>>),
				boost::hana::make_pair("day"_s, boost::hana::type_c<tc::jst::union_t<tc::js::string, tc::js::undefined>>),
				boost::hana::make_pair("hour"_s, boost::hana::type_c<tc::jst::union_t<tc::js::string, tc::js::undefined>>),
				boost::hana::make_pair("minute"_s, boost::hana::type_c<tc::jst::union_t<tc::js::string, tc::js::undefined>>),
				boost::hana::make_pair("second"_s, boost::hana::type_c<tc::jst::union_t<tc::js::string, tc::js::undefined>>),
				boost::hana::make_pair("timeZoneName"_s, boost::hana::type_c<tc::jst::union_t<tc::js::string, tc::js::undefined>>)
			);
		};
		auto locale() noexcept;
		void locale(tc::js::string v) noexcept;
		auto calendar() noexcept;
		void calendar(tc::js::string v) noexcept;
		auto numberingSystem() noexcept;
		void numberingSystem(tc::js::string v) noexcept;
		auto timeZone() noexcept;
		void timeZone(tc::js::string v) noexcept;
		auto hour12() noexcept;
		void hour12(tc::jst::union_t<bool /*false*/, tc::js::undefined> v) noexcept;
		auto weekday() noexcept;
		void weekday(tc::jst::union_t<tc::js::string, tc::js::undefined> v) noexcept;
		auto era() noexcept;
		void era(tc::jst::union_t<tc::js::string, tc::js::undefined> v) noexcept;
		auto year() noexcept;
		void year(tc::jst::union_t<tc::js::string, tc::js::undefined> v) noexcept;
		auto month() noexcept;
		void month(tc::jst::union_t<tc::js::string, tc::js::undefined> v) noexcept;
		auto day() noexcept;
		void day(tc::jst::union_t<tc::js::string, tc::js::undefined> v) noexcept;
		auto hour() noexcept;
		void hour(tc::jst::union_t<tc::js::string, tc::js::undefined> v) noexcept;
		auto minute() noexcept;
		void minute(tc::jst::union_t<tc::js::string, tc::js::undefined> v) noexcept;
		auto second() noexcept;
		void second(tc::jst::union_t<tc::js::string, tc::js::undefined> v) noexcept;
		auto timeZoneName() noexcept;
		void timeZoneName(tc::jst::union_t<tc::js::string, tc::js::undefined> v) noexcept;
	};
	struct _impl_js_jIntl_dResolvedNumberFormatOptions : virtual object_base {
		struct _tcjs_definitions {
			static constexpr auto keyof = boost::hana::make_map(
				boost::hana::make_pair("locale"_s, boost::hana::type_c<tc::js::string>),
				boost::hana::make_pair("numberingSystem"_s, boost::hana::type_c<tc::js::string>),
				boost::hana::make_pair("style"_s, boost::hana::type_c<tc::js::string>),
				boost::hana::make_pair("currency"_s, boost::hana::type_c<tc::jst::union_t<tc::js::string, tc::js::undefined>>),
				boost::hana::make_pair("currencyDisplay"_s, boost::hana::type_c<tc::jst::union_t<tc::js::string, tc::js::undefined>>),
				boost::hana::make_pair("minimumIntegerDigits"_s, boost::hana::type_c<double>),
				boost::hana::make_pair("minimumFractionDigits"_s, boost::hana::type_c<double>),
				boost::hana::make_pair("maximumFractionDigits"_s, boost::hana::type_c<double>),
				boost::hana::make_pair("minimumSignificantDigits"_s, boost::hana::type_c<tc::jst::union_t<double, tc::js::undefined>>),
				boost::hana::make_pair("maximumSignificantDigits"_s, boost::hana::type_c<tc::jst::union_t<double, tc::js::undefined>>),
				boost::hana::make_pair("useGrouping"_s, boost::hana::type_c<bool /*false*/>)
			);
		};
		auto locale() noexcept;
		void locale(tc::js::string v) noexcept;
		auto numberingSystem() noexcept;
		void numberingSystem(tc::js::string v) noexcept;
		auto style() noexcept;
		void style(tc::js::string v) noexcept;
		auto currency() noexcept;
		void currency(tc::jst::union_t<tc::js::string, tc::js::undefined> v) noexcept;
		auto currencyDisplay() noexcept;
		void currencyDisplay(tc::jst::union_t<tc::js::string, tc::js::undefined> v) noexcept;
		auto minimumIntegerDigits() noexcept;
		void minimumIntegerDigits(double v) noexcept;
		auto minimumFractionDigits() noexcept;
		void minimumFractionDigits(double v) noexcept;
		auto maximumFractionDigits() noexcept;
		void maximumFractionDigits(double v) noexcept;
		auto minimumSignificantDigits() noexcept;
		void minimumSignificantDigits(tc::jst::union_t<double, tc::js::undefined> v) noexcept;
		auto maximumSignificantDigits() noexcept;
		void maximumSignificantDigits(tc::jst::union_t<double, tc::js::undefined> v) noexcept;
		auto useGrouping() noexcept;
		void useGrouping(bool /*false*/ v) noexcept;
	};
	struct _impl_js_jIntl_dResolvedPluralRulesOptions : virtual object_base {
		struct _tcjs_definitions {
			static constexpr auto keyof = boost::hana::make_map(
				boost::hana::make_pair("locale"_s, boost::hana::type_c<tc::js::string>),
				boost::hana::make_pair("pluralCategories"_s, boost::hana::type_c<_js_jArray<tc::js::string /*"zero"*/>>),
				boost::hana::make_pair("type"_s, boost::hana::type_c<tc::js::string /*"cardinal"*/>),
				boost::hana::make_pair("minimumIntegerDigits"_s, boost::hana::type_c<double>),
				boost::hana::make_pair("minimumFractionDigits"_s, boost::hana::type_c<double>),
				boost::hana::make_pair("maximumFractionDigits"_s, boost::hana::type_c<double>),
				boost::hana::make_pair("minimumSignificantDigits"_s, boost::hana::type_c<tc::jst::union_t<double, tc::js::undefined>>),
				boost::hana::make_pair("maximumSignificantDigits"_s, boost::hana::type_c<tc::jst::union_t<double, tc::js::undefined>>)
			);
		};
		auto locale() noexcept;
		void locale(tc::js::string v) noexcept;
		auto pluralCategories() noexcept;
		void pluralCategories(_js_jArray<tc::js::string /*"zero"*/> v) noexcept;
		auto type() noexcept;
		void type(tc::js::string /*"cardinal"*/ v) noexcept;
		auto minimumIntegerDigits() noexcept;
		void minimumIntegerDigits(double v) noexcept;
		auto minimumFractionDigits() noexcept;
		void minimumFractionDigits(double v) noexcept;
		auto maximumFractionDigits() noexcept;
		void maximumFractionDigits(double v) noexcept;
		auto minimumSignificantDigits() noexcept;
		void minimumSignificantDigits(tc::jst::union_t<double, tc::js::undefined> v) noexcept;
		auto maximumSignificantDigits() noexcept;
		void maximumSignificantDigits(tc::jst::union_t<double, tc::js::undefined> v) noexcept;
	};
	template<typename T>
	struct _impl_js_jIterable : virtual object_base {
		struct _tcjs_definitions {
			static constexpr auto keyof = boost::hana::make_map(

			);
		};
	};
	template<typename T>
	struct _impl_js_jIterableIterator : virtual _impl_js_jIterator<T, tc::js::any, tc::js::undefined> {
		struct _tcjs_definitions {
			static constexpr auto keyof = boost::hana::make_map(

			);
		};
	};
	template<typename TReturn>
	struct _impl_js_jIteratorReturnResult : virtual object_base {
		struct _tcjs_definitions {
			static constexpr auto keyof = boost::hana::make_map(
				boost::hana::make_pair("done"_s, boost::hana::type_c<bool /*true*/>),
				boost::hana::make_pair("value"_s, boost::hana::type_c<TReturn>)
			);
		};
		auto done() noexcept;
		void done(bool /*true*/ v) noexcept;
		auto value() noexcept;
		void value(TReturn v) noexcept;
	};
	template<typename TYield>
	struct _impl_js_jIteratorYieldResult : virtual object_base {
		struct _tcjs_definitions {
			static constexpr auto keyof = boost::hana::make_map(
				boost::hana::make_pair("done"_s, boost::hana::type_c<tc::jst::union_t<bool /*false*/, tc::js::undefined>>),
				boost::hana::make_pair("value"_s, boost::hana::type_c<TYield>)
			);
		};
		auto done() noexcept;
		void done(tc::jst::union_t<bool /*false*/, tc::js::undefined> v) noexcept;
		auto value() noexcept;
		void value(TYield v) noexcept;
	};
	struct _impl_js_jJSON : virtual object_base {
		struct _tcjs_definitions {
			static constexpr auto keyof = boost::hana::make_map(

			);
		};
		auto parse(tc::js::string text, tc::jst::union_t<tc::js::undefined, tc::jst::function<tc::js::any(tc::js::string, tc::js::any)>> reviver = tc::js::undefined()) noexcept;
		auto stringify(tc::js::any value, tc::jst::union_t<_js_jArray<tc::jst::union_t<double, tc::js::string>>, tc::js::null, tc::js::undefined> replacer = tc::js::undefined(), tc::jst::union_t<double, tc::js::string, tc::js::undefined> space = tc::js::undefined()) noexcept;
		auto stringify(tc::js::any value, tc::jst::union_t<tc::js::undefined, tc::jst::function<tc::js::any(tc::js::string, tc::js::any)>> replacer = tc::js::undefined(), tc::jst::union_t<double, tc::js::string, tc::js::undefined> space = tc::js::undefined()) noexcept;
	};
	template<typename K, typename V>
	struct _impl_js_jMap : virtual object_base {
		struct _tcjs_definitions {
			static constexpr auto keyof = boost::hana::make_map(
				boost::hana::make_pair("size"_s, boost::hana::type_c<double>)
			);
		};
		auto size() noexcept;
		static auto _tcjs_construct() noexcept;
		static auto _tcjs_construct(tc::jst::union_t<_js_jReadonlyArray<std::tuple<K, V>>, tc::js::null, tc::js::undefined> entries = tc::js::undefined()) noexcept;
		static auto _tcjs_construct(_js_jIterable<std::tuple<K, V>> iterable) noexcept;
		auto clear() noexcept;
		auto delete_(K key) noexcept;
		auto entries() noexcept;
		auto forEach(tc::jst::function<void(V, K, _js_jMap<K, V>)> callbackfn, tc::js::any thisArg) noexcept;
		auto get(K key) noexcept;
		auto has(K key) noexcept;
		auto keys() noexcept;
		auto set(K key, V value) noexcept;
		auto values() noexcept;
	};
	struct _impl_js_jMapConstructor : virtual object_base {
		struct _tcjs_definitions {
			static constexpr auto keyof = boost::hana::make_map(
				boost::hana::make_pair("prototype"_s, boost::hana::type_c<_js_jMap<tc::js::any, tc::js::any>>)
			);
		};
		auto prototype() noexcept;
	};
	struct _impl_js_jMath : virtual object_base {
		struct _tcjs_definitions {
			static constexpr auto keyof = boost::hana::make_map(
				boost::hana::make_pair("E"_s, boost::hana::type_c<double>),
				boost::hana::make_pair("LN10"_s, boost::hana::type_c<double>),
				boost::hana::make_pair("LN2"_s, boost::hana::type_c<double>),
				boost::hana::make_pair("LOG2E"_s, boost::hana::type_c<double>),
				boost::hana::make_pair("LOG10E"_s, boost::hana::type_c<double>),
				boost::hana::make_pair("PI"_s, boost::hana::type_c<double>),
				boost::hana::make_pair("SQRT1_2"_s, boost::hana::type_c<double>),
				boost::hana::make_pair("SQRT2"_s, boost::hana::type_c<double>)
			);
		};
		auto E() noexcept;
		auto LN10() noexcept;
		auto LN2() noexcept;
		auto LOG2E() noexcept;
		auto LOG10E() noexcept;
		auto PI() noexcept;
		auto SQRT1_2() noexcept;
		auto SQRT2() noexcept;
		auto abs(double x) noexcept;
		auto acos(double x) noexcept;
		auto acosh(double x) noexcept;
		auto asin(double x) noexcept;
		auto asinh(double x) noexcept;
		auto atan(double x) noexcept;
		auto atan2(double y, double x) noexcept;
		auto atanh(double x) noexcept;
		auto cbrt(double x) noexcept;
		auto ceil(double x) noexcept;
		auto clz32(double x) noexcept;
		auto cos(double x) noexcept;
		auto cosh(double x) noexcept;
		auto exp(double x) noexcept;
		auto expm1(double x) noexcept;
		auto floor(double x) noexcept;
		auto fround(double x) noexcept;
		auto hypot(tc::jst::variadic_arg_t, double values) noexcept;
		auto imul(double x, double y) noexcept;
		auto log(double x) noexcept;
		auto log10(double x) noexcept;
		auto log1p(double x) noexcept;
		auto log2(double x) noexcept;
		auto max(tc::jst::variadic_arg_t, double values) noexcept;
		auto min(tc::jst::variadic_arg_t, double values) noexcept;
		auto pow(double x, double y) noexcept;
		auto random() noexcept;
		auto round(double x) noexcept;
		auto sign(double x) noexcept;
		auto sin(double x) noexcept;
		auto sinh(double x) noexcept;
		auto sqrt(double x) noexcept;
		auto tan(double x) noexcept;
		auto tanh(double x) noexcept;
		auto trunc(double x) noexcept;
	};
	struct _impl_js_jNewableFunction : virtual _impl_js_jFunction {
		struct _tcjs_definitions {
			static constexpr auto keyof = boost::hana::make_map(

			);
		};
		template<typename T>
		auto apply(T thisArg) noexcept;
		template<typename T, typename A>
		auto apply(T thisArg, A args) noexcept;
		template<typename T>
		auto bind(tc::js::any thisArg) noexcept;
		template<typename A0, typename A, typename R>
		auto bind(tc::js::any thisArg, A0 arg0) noexcept;
		template<typename A0, typename A1, typename A, typename R>
		auto bind(tc::js::any thisArg, A0 arg0, A1 arg1) noexcept;
		template<typename A0, typename A1, typename A2, typename A, typename R>
		auto bind(tc::js::any thisArg, A0 arg0, A1 arg1, A2 arg2) noexcept;
		template<typename A0, typename A1, typename A2, typename A3, typename A, typename R>
		auto bind(tc::js::any thisArg, A0 arg0, A1 arg1, A2 arg2, A3 arg3) noexcept;
		template<typename AX, typename R>
		auto bind(tc::js::any thisArg, tc::jst::variadic_arg_t, AX args) noexcept;
		template<typename T, typename A>
		auto call(T thisArg, A args) noexcept;
	};
	struct _impl_js_jNumber : virtual object_base {
		struct _tcjs_definitions {
			static constexpr auto keyof = boost::hana::make_map(

			);
		};
		static auto _tcjs_construct(tc::js::any value) noexcept;
		auto toExponential(tc::jst::union_t<double, tc::js::undefined> fractionDigits = tc::js::undefined()) noexcept;
		auto toFixed(tc::jst::union_t<double, tc::js::undefined> fractionDigits = tc::js::undefined()) noexcept;
		auto toLocaleString(tc::jst::union_t<_js_jArray<tc::js::string>, tc::js::string, tc::js::undefined> locales = tc::js::undefined(), tc::jst::union_t<_js_jIntl_dNumberFormatOptions, tc::js::undefined> options = tc::js::undefined()) noexcept;
		auto toPrecision(tc::jst::union_t<double, tc::js::undefined> precision = tc::js::undefined()) noexcept;
		auto toString(tc::jst::union_t<double, tc::js::undefined> radix = tc::js::undefined()) noexcept;
		auto valueOf() noexcept;
	};
	struct _impl_js_jNumberConstructor : virtual object_base {
		struct _tcjs_definitions {
			static constexpr auto keyof = boost::hana::make_map(
				boost::hana::make_pair("prototype"_s, boost::hana::type_c<_js_jNumber>),
				boost::hana::make_pair("MAX_VALUE"_s, boost::hana::type_c<double>),
				boost::hana::make_pair("MIN_VALUE"_s, boost::hana::type_c<double>),
				boost::hana::make_pair("NaN"_s, boost::hana::type_c<double>),
				boost::hana::make_pair("NEGATIVE_INFINITY"_s, boost::hana::type_c<double>),
				boost::hana::make_pair("POSITIVE_INFINITY"_s, boost::hana::type_c<double>),
				boost::hana::make_pair("EPSILON"_s, boost::hana::type_c<double>),
				boost::hana::make_pair("MAX_SAFE_INTEGER"_s, boost::hana::type_c<double>),
				boost::hana::make_pair("MIN_SAFE_INTEGER"_s, boost::hana::type_c<double>)
			);
		};
		auto prototype() noexcept;
		auto MAX_VALUE() noexcept;
		auto MIN_VALUE() noexcept;
		auto NaN() noexcept;
		auto NEGATIVE_INFINITY() noexcept;
		auto POSITIVE_INFINITY() noexcept;
		auto EPSILON() noexcept;
		auto MAX_SAFE_INTEGER() noexcept;
		auto MIN_SAFE_INTEGER() noexcept;
		auto isFinite(tc::js::any number) noexcept;
		auto isInteger(tc::js::any number) noexcept;
		auto isNaN(tc::js::any number) noexcept;
		auto isSafeInteger(tc::js::any number) noexcept;
		auto parseFloat(tc::js::string string) noexcept;
		auto parseInt(tc::js::string string, tc::jst::union_t<double, tc::js::undefined> radix = tc::js::undefined()) noexcept;
	};
	struct _impl_js_jObject : virtual object_base {
		struct _tcjs_definitions {
			static constexpr auto keyof = boost::hana::make_map(
				boost::hana::make_pair("constructor"_s, boost::hana::type_c<_js_jFunction>)
			);
		};
		auto constructor() noexcept;
		void constructor(_js_jFunction v) noexcept;
		static auto _tcjs_construct(tc::js::any value) noexcept;
		auto hasOwnProperty(tc::js::any /*UnionWithUnsupportedSubtype=[tc::js::string,double,tc::js::any]*/ v) noexcept;
		auto isPrototypeOf(_js_jObject v) noexcept;
		auto propertyIsEnumerable(tc::js::any /*UnionWithUnsupportedSubtype=[tc::js::string,double,tc::js::any]*/ v) noexcept;
		auto toLocaleString() noexcept;
		auto toString() noexcept;
		auto valueOf() noexcept;
	};
	struct _impl_js_jObjectConstructor : virtual object_base {
		struct _tcjs_definitions {
			static constexpr auto keyof = boost::hana::make_map(
				boost::hana::make_pair("prototype"_s, boost::hana::type_c<_js_jObject>)
			);
		};
		auto prototype() noexcept;
		template<typename T, typename U>
		auto assign(T target, U source) noexcept;
		template<typename T, typename U, typename V>
		auto assign(T target, U source1, V source2) noexcept;
		template<typename T, typename U, typename V, typename W>
		auto assign(T target, U source1, V source2, W source3) noexcept;
		auto assign(tc::js::any /* unsupported type flags =67108864: object*/ target, tc::jst::variadic_arg_t, tc::js::any sources) noexcept;
		auto create(tc::js::any /*UnionWithUnsupportedSubtype=[tc::js::null,tc::js::any]*/ o) noexcept;
		auto create(tc::js::any /*UnionWithUnsupportedSubtype=[tc::js::null,tc::js::any]*/ o, tc::js::any /* unsupported type flags =2097152: PropertyDescriptorMap & ThisType<any>*/ properties) noexcept;
		template<typename T>
		auto defineProperties(T o, tc::js::any /* unsupported type flags =2097152: PropertyDescriptorMap & ThisType<any>*/ properties) noexcept;
		template<typename T>
		auto defineProperty(T o, tc::js::any /*UnionWithUnsupportedSubtype=[tc::js::string,double,tc::js::any]*/ p, tc::js::any /* unsupported type flags =2097152: PropertyDescriptor & ThisType<any>*/ attributes) noexcept;
		template<typename T>
		auto entries(tc::js::any /*UnionWithUnsupportedSubtype=[tc::js::any,_js_jArrayLike<T>]*/ o) noexcept;
		template<typename T>
		auto freeze(T f) noexcept;
		template<typename T>
		auto freeze(_js_jArray<T> a) noexcept;
		auto getOwnPropertyDescriptor(tc::js::any o, tc::js::any /*UnionWithUnsupportedSubtype=[tc::js::string,double,tc::js::any]*/ p) noexcept;
		template<typename T>
		auto getOwnPropertyDescriptors(T o) noexcept;
		auto getOwnPropertyNames(tc::js::any o) noexcept;
		auto getOwnPropertySymbols(tc::js::any o) noexcept;
		auto getPrototypeOf(tc::js::any o) noexcept;
		auto is(tc::js::any value1, tc::js::any value2) noexcept;
		auto isExtensible(tc::js::any o) noexcept;
		auto isFrozen(tc::js::any o) noexcept;
		auto isSealed(tc::js::any o) noexcept;
		auto keys(tc::js::any /*AnonymousType={}*/ o) noexcept;
		template<typename T>
		auto preventExtensions(T o) noexcept;
		template<typename T>
		auto seal(T o) noexcept;
		auto setPrototypeOf(tc::js::any o, tc::js::any /*UnionWithUnsupportedSubtype=[tc::js::null,tc::js::any]*/ proto) noexcept;
		auto values(tc::js::any /*AnonymousType={}*/ o) noexcept;
	};
	struct _impl_js_jPromiseConstructor : virtual object_base {
		struct _tcjs_definitions {
			static constexpr auto keyof = boost::hana::make_map(
				boost::hana::make_pair("prototype"_s, boost::hana::type_c<_js_jPromise<tc::js::any>>)
			);
		};
		auto prototype() noexcept;
		template<typename T>
		auto all(_js_jIterable<tc::jst::union_t<T, _js_jPromiseLike<T>>> values) noexcept;
		template<typename T>
		auto all(_js_jReadonlyArray<tc::jst::union_t<T, _js_jPromiseLike<T>>> values) noexcept;
		template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10>
		auto all(std::tuple<tc::jst::union_t<T1, _js_jPromiseLike<T1>>, tc::jst::union_t<T2, _js_jPromiseLike<T2>>, tc::jst::union_t<T3, _js_jPromiseLike<T3>>, tc::jst::union_t<T4, _js_jPromiseLike<T4>>, tc::jst::union_t<T5, _js_jPromiseLike<T5>>, tc::jst::union_t<T6, _js_jPromiseLike<T6>>, tc::jst::union_t<T7, _js_jPromiseLike<T7>>, tc::jst::union_t<T8, _js_jPromiseLike<T8>>, tc::jst::union_t<T9, _js_jPromiseLike<T9>>, tc::jst::union_t<T10, _js_jPromiseLike<T10>>> values) noexcept;
		template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9>
		auto all(std::tuple<tc::jst::union_t<T1, _js_jPromiseLike<T1>>, tc::jst::union_t<T2, _js_jPromiseLike<T2>>, tc::jst::union_t<T3, _js_jPromiseLike<T3>>, tc::jst::union_t<T4, _js_jPromiseLike<T4>>, tc::jst::union_t<T5, _js_jPromiseLike<T5>>, tc::jst::union_t<T6, _js_jPromiseLike<T6>>, tc::jst::union_t<T7, _js_jPromiseLike<T7>>, tc::jst::union_t<T8, _js_jPromiseLike<T8>>, tc::jst::union_t<T9, _js_jPromiseLike<T9>>> values) noexcept;
		template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
		auto all(std::tuple<tc::jst::union_t<T1, _js_jPromiseLike<T1>>, tc::jst::union_t<T2, _js_jPromiseLike<T2>>, tc::jst::union_t<T3, _js_jPromiseLike<T3>>, tc::jst::union_t<T4, _js_jPromiseLike<T4>>, tc::jst::union_t<T5, _js_jPromiseLike<T5>>, tc::jst::union_t<T6, _js_jPromiseLike<T6>>, tc::jst::union_t<T7, _js_jPromiseLike<T7>>, tc::jst::union_t<T8, _js_jPromiseLike<T8>>> values) noexcept;
		template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
		auto all(std::tuple<tc::jst::union_t<T1, _js_jPromiseLike<T1>>, tc::jst::union_t<T2, _js_jPromiseLike<T2>>, tc::jst::union_t<T3, _js_jPromiseLike<T3>>, tc::jst::union_t<T4, _js_jPromiseLike<T4>>, tc::jst::union_t<T5, _js_jPromiseLike<T5>>, tc::jst::union_t<T6, _js_jPromiseLike<T6>>, tc::jst::union_t<T7, _js_jPromiseLike<T7>>> values) noexcept;
		template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
		auto all(std::tuple<tc::jst::union_t<T1, _js_jPromiseLike<T1>>, tc::jst::union_t<T2, _js_jPromiseLike<T2>>, tc::jst::union_t<T3, _js_jPromiseLike<T3>>, tc::jst::union_t<T4, _js_jPromiseLike<T4>>, tc::jst::union_t<T5, _js_jPromiseLike<T5>>, tc::jst::union_t<T6, _js_jPromiseLike<T6>>> values) noexcept;
		template<typename T1, typename T2, typename T3, typename T4, typename T5>
		auto all(std::tuple<tc::jst::union_t<T1, _js_jPromiseLike<T1>>, tc::jst::union_t<T2, _js_jPromiseLike<T2>>, tc::jst::union_t<T3, _js_jPromiseLike<T3>>, tc::jst::union_t<T4, _js_jPromiseLike<T4>>, tc::jst::union_t<T5, _js_jPromiseLike<T5>>> values) noexcept;
		template<typename T1, typename T2, typename T3, typename T4>
		auto all(std::tuple<tc::jst::union_t<T1, _js_jPromiseLike<T1>>, tc::jst::union_t<T2, _js_jPromiseLike<T2>>, tc::jst::union_t<T3, _js_jPromiseLike<T3>>, tc::jst::union_t<T4, _js_jPromiseLike<T4>>> values) noexcept;
		template<typename T1, typename T2, typename T3>
		auto all(std::tuple<tc::jst::union_t<T1, _js_jPromiseLike<T1>>, tc::jst::union_t<T2, _js_jPromiseLike<T2>>, tc::jst::union_t<T3, _js_jPromiseLike<T3>>> values) noexcept;
		template<typename T1, typename T2>
		auto all(std::tuple<tc::jst::union_t<T1, _js_jPromiseLike<T1>>, tc::jst::union_t<T2, _js_jPromiseLike<T2>>> values) noexcept;
		template<typename T>
		auto race(_js_jIterable<T> values) noexcept;
		template<typename T>
		auto race(_js_jIterable<tc::jst::union_t<T, _js_jPromiseLike<T>>> values) noexcept;
		template<typename T>
		auto race(_js_jReadonlyArray<T> values) noexcept;
		template<typename T>
		auto reject(tc::js::any reason) noexcept;
		auto resolve() noexcept;
		template<typename T>
		auto resolve(tc::jst::union_t<T, _js_jPromiseLike<T>> value) noexcept;
	};
	template<typename T>
	struct _impl_js_jPromiseLike : virtual object_base {
		struct _tcjs_definitions {
			static constexpr auto keyof = boost::hana::make_map(

			);
		};
		template<typename TResult1, typename TResult2>
		auto then(tc::jst::union_t<tc::js::null, tc::js::undefined, tc::jst::function<tc::jst::union_t<TResult1, _js_jPromiseLike<TResult1>>(T)>> onfulfilled = tc::js::undefined(), tc::jst::union_t<tc::js::null, tc::js::undefined, tc::jst::function<tc::jst::union_t<TResult2, _js_jPromiseLike<TResult2>>(tc::js::any)>> onrejected = tc::js::undefined()) noexcept;
	};
	struct _impl_js_jPropertyDescriptor : virtual object_base {
		struct _tcjs_definitions {
			static constexpr auto keyof = boost::hana::make_map(
				boost::hana::make_pair("configurable"_s, boost::hana::type_c<tc::jst::union_t<bool /*false*/, tc::js::undefined>>),
				boost::hana::make_pair("enumerable"_s, boost::hana::type_c<tc::jst::union_t<bool /*false*/, tc::js::undefined>>),
				boost::hana::make_pair("value"_s, boost::hana::type_c<tc::js::any>),
				boost::hana::make_pair("writable"_s, boost::hana::type_c<tc::jst::union_t<bool /*false*/, tc::js::undefined>>)
			);
		};
		auto configurable() noexcept;
		void configurable(tc::jst::union_t<bool /*false*/, tc::js::undefined> v) noexcept;
		auto enumerable() noexcept;
		void enumerable(tc::jst::union_t<bool /*false*/, tc::js::undefined> v) noexcept;
		auto value() noexcept;
		void value(tc::js::any v) noexcept;
		auto writable() noexcept;
		void writable(tc::jst::union_t<bool /*false*/, tc::js::undefined> v) noexcept;
	};
	struct _impl_js_jPropertyDescriptorMap : virtual object_base {
		struct _tcjs_definitions {
			static constexpr auto keyof = boost::hana::make_map(

			);
		};
		static auto _tcjs_construct() noexcept;
		auto operator[](tc::js::string s) noexcept;
	};
	struct _impl_js_jProxyConstructor : virtual object_base {
		struct _tcjs_definitions {
			static constexpr auto keyof = boost::hana::make_map(

			);
		};
		template<typename T>
		auto revocable(T target, _js_jProxyHandler<T> handler) noexcept;
	};
	template<typename T>
	struct _impl_js_jProxyHandler : virtual object_base {
		struct _tcjs_definitions {
			static constexpr auto keyof = boost::hana::make_map(

			);
		};
	};
	struct _impl_js_jRangeError : virtual _impl_js_jError {
		struct _tcjs_definitions {
			static constexpr auto keyof = boost::hana::make_map(

			);
		};
		static auto _tcjs_construct(tc::jst::union_t<tc::js::string, tc::js::undefined> message = tc::js::undefined()) noexcept;
	};
	struct _impl_js_jRangeErrorConstructor : virtual _impl_js_jErrorConstructor {
		struct _tcjs_definitions {
			static constexpr auto keyof = boost::hana::make_map(
				boost::hana::make_pair("prototype"_s, boost::hana::type_c<_js_jRangeError>)
			);
		};
		auto prototype() noexcept;
	};
	template<typename T>
	struct _impl_js_jReadonlyArray : virtual object_base {
		struct _tcjs_definitions {
			static constexpr auto keyof = boost::hana::make_map(
				boost::hana::make_pair("length"_s, boost::hana::type_c<double>)
			);
		};
		auto length() noexcept;
		auto concat(tc::jst::variadic_arg_t, _js_jConcatArray<T> items) noexcept;
		auto concat(tc::jst::variadic_arg_t, tc::jst::union_t<T, _js_jConcatArray<T>> items) noexcept;
		auto entries() noexcept;
		template<typename S>
		auto every(tc::jst::function<bool /*false*/(T, double, _js_jReadonlyArray<T>)> predicate, tc::js::any thisArg) noexcept;
		auto every(tc::jst::function<tc::js::any(T, double, _js_jReadonlyArray<T>)> predicate, tc::js::any thisArg) noexcept;
		template<typename S>
		auto filter(tc::jst::function<bool /*false*/(T, double, _js_jReadonlyArray<T>)> predicate, tc::js::any thisArg) noexcept;
		auto filter(tc::jst::function<tc::js::any(T, double, _js_jReadonlyArray<T>)> predicate, tc::js::any thisArg) noexcept;
		template<typename S>
		auto find(tc::jst::function<bool /*false*/(T, double, _js_jReadonlyArray<T>)> predicate, tc::js::any thisArg) noexcept;
		auto find(tc::jst::function<tc::js::any(T, double, _js_jReadonlyArray<T>)> predicate, tc::js::any thisArg) noexcept;
		auto findIndex(tc::jst::function<tc::js::any(T, double, _js_jReadonlyArray<T>)> predicate, tc::js::any thisArg) noexcept;
		auto forEach(tc::jst::function<void(T, double, _js_jReadonlyArray<T>)> callbackfn, tc::js::any thisArg) noexcept;
		auto includes(T searchElement, tc::jst::union_t<double, tc::js::undefined> fromIndex = tc::js::undefined()) noexcept;
		auto indexOf(T searchElement, tc::jst::union_t<double, tc::js::undefined> fromIndex = tc::js::undefined()) noexcept;
		auto join(tc::jst::union_t<tc::js::string, tc::js::undefined> separator = tc::js::undefined()) noexcept;
		auto keys() noexcept;
		auto lastIndexOf(T searchElement, tc::jst::union_t<double, tc::js::undefined> fromIndex = tc::js::undefined()) noexcept;
		template<typename U>
		auto map(tc::jst::function<U(T, double, _js_jReadonlyArray<T>)> callbackfn, tc::js::any thisArg) noexcept;
		auto operator[](double n) noexcept;
		auto reduce(tc::jst::function<T(T, T, double, _js_jReadonlyArray<T>)> callbackfn) noexcept;
		auto reduce(tc::jst::function<T(T, T, double, _js_jReadonlyArray<T>)> callbackfn, T initialValue) noexcept;
		template<typename U>
		auto reduce(tc::jst::function<U(U, T, double, _js_jReadonlyArray<T>)> callbackfn, U initialValue) noexcept;
		auto reduceRight(tc::jst::function<T(T, T, double, _js_jReadonlyArray<T>)> callbackfn) noexcept;
		auto reduceRight(tc::jst::function<T(T, T, double, _js_jReadonlyArray<T>)> callbackfn, T initialValue) noexcept;
		template<typename U>
		auto reduceRight(tc::jst::function<U(U, T, double, _js_jReadonlyArray<T>)> callbackfn, U initialValue) noexcept;
		auto slice(tc::jst::union_t<double, tc::js::undefined> start = tc::js::undefined(), tc::jst::union_t<double, tc::js::undefined> end = tc::js::undefined()) noexcept;
		auto some(tc::jst::function<tc::js::any(T, double, _js_jReadonlyArray<T>)> predicate, tc::js::any thisArg) noexcept;
		auto toLocaleString() noexcept;
		auto toString() noexcept;
		auto values() noexcept;
	};
	template<typename K, typename V>
	struct _impl_js_jReadonlyMap : virtual object_base {
		struct _tcjs_definitions {
			static constexpr auto keyof = boost::hana::make_map(
				boost::hana::make_pair("size"_s, boost::hana::type_c<double>)
			);
		};
		auto size() noexcept;
		auto entries() noexcept;
		auto forEach(tc::jst::function<void(V, K, _js_jReadonlyMap<K, V>)> callbackfn, tc::js::any thisArg) noexcept;
		auto get(K key) noexcept;
		auto has(K key) noexcept;
		auto keys() noexcept;
		auto values() noexcept;
	};
	template<typename T>
	struct _impl_js_jReadonlySet : virtual object_base {
		struct _tcjs_definitions {
			static constexpr auto keyof = boost::hana::make_map(
				boost::hana::make_pair("size"_s, boost::hana::type_c<double>)
			);
		};
		auto size() noexcept;
		auto entries() noexcept;
		auto forEach(tc::jst::function<void(T, T, _js_jReadonlySet<T>)> callbackfn, tc::js::any thisArg) noexcept;
		auto has(T value) noexcept;
		auto keys() noexcept;
		auto values() noexcept;
	};
	struct _impl_js_jReferenceError : virtual _impl_js_jError {
		struct _tcjs_definitions {
			static constexpr auto keyof = boost::hana::make_map(

			);
		};
		static auto _tcjs_construct(tc::jst::union_t<tc::js::string, tc::js::undefined> message = tc::js::undefined()) noexcept;
	};
	struct _impl_js_jReferenceErrorConstructor : virtual _impl_js_jErrorConstructor {
		struct _tcjs_definitions {
			static constexpr auto keyof = boost::hana::make_map(
				boost::hana::make_pair("prototype"_s, boost::hana::type_c<_js_jReferenceError>)
			);
		};
		auto prototype() noexcept;
	};
	struct _impl_js_jReflect : virtual object_base {
		struct _tcjs_definitions {
			static constexpr auto keyof = boost::hana::make_map(

			);
			static auto apply(_js_jFunction target, tc::js::any thisArgument, _js_jArrayLike<tc::js::any> argumentsList) noexcept;
			static auto construct(_js_jFunction target, _js_jArrayLike<tc::js::any> argumentsList, tc::jst::union_t<_js_jFunction, tc::js::undefined> newTarget = tc::js::undefined()) noexcept;
			static auto defineProperty(tc::js::any /* unsupported type flags =67108864: object*/ target, tc::js::any /*UnionWithUnsupportedSubtype=[tc::js::string,double,tc::js::any]*/ propertyKey, _js_jPropertyDescriptor attributes) noexcept;
			static auto deleteProperty(tc::js::any /* unsupported type flags =67108864: object*/ target, tc::js::any /*UnionWithUnsupportedSubtype=[tc::js::string,double,tc::js::any]*/ propertyKey) noexcept;
			static auto get(tc::js::any /* unsupported type flags =67108864: object*/ target, tc::js::any /*UnionWithUnsupportedSubtype=[tc::js::string,double,tc::js::any]*/ propertyKey, tc::js::any receiver) noexcept;
			static auto getOwnPropertyDescriptor(tc::js::any /* unsupported type flags =67108864: object*/ target, tc::js::any /*UnionWithUnsupportedSubtype=[tc::js::string,double,tc::js::any]*/ propertyKey) noexcept;
			static auto getPrototypeOf(tc::js::any /* unsupported type flags =67108864: object*/ target) noexcept;
			static auto has(tc::js::any /* unsupported type flags =67108864: object*/ target, tc::js::any /*UnionWithUnsupportedSubtype=[tc::js::string,double,tc::js::any]*/ propertyKey) noexcept;
			static auto isExtensible(tc::js::any /* unsupported type flags =67108864: object*/ target) noexcept;
			static auto ownKeys(tc::js::any /* unsupported type flags =67108864: object*/ target) noexcept;
			static auto preventExtensions(tc::js::any /* unsupported type flags =67108864: object*/ target) noexcept;
			static auto set(tc::js::any /* unsupported type flags =67108864: object*/ target, tc::js::any /*UnionWithUnsupportedSubtype=[tc::js::string,double,tc::js::any]*/ propertyKey, tc::js::any value, tc::js::any receiver) noexcept;
			static auto setPrototypeOf(tc::js::any /* unsupported type flags =67108864: object*/ target, tc::js::any /*UnionWithUnsupportedSubtype=[tc::js::null,tc::js::any]*/ proto) noexcept;
		};
	};
	struct _impl_js_jRegExp : virtual object_base {
		struct _tcjs_definitions {
			static constexpr auto keyof = boost::hana::make_map(
				boost::hana::make_pair("dotAll"_s, boost::hana::type_c<bool /*false*/>),
				boost::hana::make_pair("source"_s, boost::hana::type_c<tc::js::string>),
				boost::hana::make_pair("global"_s, boost::hana::type_c<bool /*false*/>),
				boost::hana::make_pair("ignoreCase"_s, boost::hana::type_c<bool /*false*/>),
				boost::hana::make_pair("multiline"_s, boost::hana::type_c<bool /*false*/>),
				boost::hana::make_pair("lastIndex"_s, boost::hana::type_c<double>),
				boost::hana::make_pair("flags"_s, boost::hana::type_c<tc::js::string>),
				boost::hana::make_pair("sticky"_s, boost::hana::type_c<bool /*false*/>),
				boost::hana::make_pair("unicode"_s, boost::hana::type_c<bool /*false*/>)
			);
		};
		auto dotAll() noexcept;
		auto source() noexcept;
		auto global() noexcept;
		auto ignoreCase() noexcept;
		auto multiline() noexcept;
		auto lastIndex() noexcept;
		void lastIndex(double v) noexcept;
		auto flags() noexcept;
		auto sticky() noexcept;
		auto unicode() noexcept;
		static auto _tcjs_construct(tc::jst::union_t<_js_jRegExp, tc::js::string> pattern) noexcept;
		static auto _tcjs_construct(tc::js::string pattern, tc::jst::union_t<tc::js::string, tc::js::undefined> flags = tc::js::undefined()) noexcept;
		static auto _tcjs_construct(tc::jst::union_t<_js_jRegExp, tc::js::string> pattern, tc::jst::union_t<tc::js::string, tc::js::undefined> flags = tc::js::undefined()) noexcept;
		auto compile() noexcept;
		auto exec(tc::js::string string) noexcept;
		auto test(tc::js::string string) noexcept;
	};
	struct _impl_js_jRegExpConstructor : virtual object_base {
		struct _tcjs_definitions {
			static constexpr auto keyof = boost::hana::make_map(
				boost::hana::make_pair("prototype"_s, boost::hana::type_c<_js_jRegExp>),
				boost::hana::make_pair("_1"_s, boost::hana::type_c<tc::js::string>),
				boost::hana::make_pair("_2"_s, boost::hana::type_c<tc::js::string>),
				boost::hana::make_pair("_3"_s, boost::hana::type_c<tc::js::string>),
				boost::hana::make_pair("_4"_s, boost::hana::type_c<tc::js::string>),
				boost::hana::make_pair("_5"_s, boost::hana::type_c<tc::js::string>),
				boost::hana::make_pair("_6"_s, boost::hana::type_c<tc::js::string>),
				boost::hana::make_pair("_7"_s, boost::hana::type_c<tc::js::string>),
				boost::hana::make_pair("_8"_s, boost::hana::type_c<tc::js::string>),
				boost::hana::make_pair("_9"_s, boost::hana::type_c<tc::js::string>),
				boost::hana::make_pair("lastMatch"_s, boost::hana::type_c<tc::js::string>)
			);
		};
		auto prototype() noexcept;
		auto _1() noexcept;
		void _1(tc::js::string v) noexcept;
		auto _2() noexcept;
		void _2(tc::js::string v) noexcept;
		auto _3() noexcept;
		void _3(tc::js::string v) noexcept;
		auto _4() noexcept;
		void _4(tc::js::string v) noexcept;
		auto _5() noexcept;
		void _5(tc::js::string v) noexcept;
		auto _6() noexcept;
		void _6(tc::js::string v) noexcept;
		auto _7() noexcept;
		void _7(tc::js::string v) noexcept;
		auto _8() noexcept;
		void _8(tc::js::string v) noexcept;
		auto _9() noexcept;
		void _9(tc::js::string v) noexcept;
		auto lastMatch() noexcept;
		void lastMatch(tc::js::string v) noexcept;
	};
	struct _impl_js_jRegExpExecArray : virtual _impl_js_jArray<tc::js::string> {
		struct _tcjs_definitions {
			static constexpr auto keyof = boost::hana::make_map(
				boost::hana::make_pair("groups"_s, boost::hana::type_c<tc::js::any /*UnionWithUnsupportedSubtype=[tc::js::undefined,tc::js::any]*/>),
				boost::hana::make_pair("index"_s, boost::hana::type_c<double>),
				boost::hana::make_pair("input"_s, boost::hana::type_c<tc::js::string>)
			);
		};
		auto groups() noexcept;
		void groups(tc::js::any /*UnionWithUnsupportedSubtype=[tc::js::undefined,tc::js::any]*/ v) noexcept;
		auto index() noexcept;
		void index(double v) noexcept;
		auto input() noexcept;
		void input(tc::js::string v) noexcept;
	};
	struct _impl_js_jRegExpMatchArray : virtual _impl_js_jArray<tc::js::string> {
		struct _tcjs_definitions {
			static constexpr auto keyof = boost::hana::make_map(
				boost::hana::make_pair("groups"_s, boost::hana::type_c<tc::js::any /*UnionWithUnsupportedSubtype=[tc::js::undefined,tc::js::any]*/>),
				boost::hana::make_pair("index"_s, boost::hana::type_c<tc::jst::union_t<double, tc::js::undefined>>),
				boost::hana::make_pair("input"_s, boost::hana::type_c<tc::jst::union_t<tc::js::string, tc::js::undefined>>)
			);
		};
		auto groups() noexcept;
		void groups(tc::js::any /*UnionWithUnsupportedSubtype=[tc::js::undefined,tc::js::any]*/ v) noexcept;
		auto index() noexcept;
		void index(tc::jst::union_t<double, tc::js::undefined> v) noexcept;
		auto input() noexcept;
		void input(tc::jst::union_t<tc::js::string, tc::js::undefined> v) noexcept;
	};
	template<typename T>
	struct _impl_js_jSet : virtual object_base {
		struct _tcjs_definitions {
			static constexpr auto keyof = boost::hana::make_map(
				boost::hana::make_pair("size"_s, boost::hana::type_c<double>)
			);
		};
		auto size() noexcept;
		static auto _tcjs_construct(tc::jst::union_t<_js_jReadonlyArray<T>, tc::js::null, tc::js::undefined> values = tc::js::undefined()) noexcept;
		static auto _tcjs_construct(tc::jst::union_t<_js_jIterable<T>, tc::js::null, tc::js::undefined> iterable = tc::js::undefined()) noexcept;
		auto add(T value) noexcept;
		auto clear() noexcept;
		auto delete_(T value) noexcept;
		auto entries() noexcept;
		auto forEach(tc::jst::function<void(T, T, _js_jSet<T>)> callbackfn, tc::js::any thisArg) noexcept;
		auto has(T value) noexcept;
		auto keys() noexcept;
		auto values() noexcept;
	};
	struct _impl_js_jSetConstructor : virtual object_base {
		struct _tcjs_definitions {
			static constexpr auto keyof = boost::hana::make_map(
				boost::hana::make_pair("prototype"_s, boost::hana::type_c<_js_jSet<tc::js::any>>)
			);
		};
		auto prototype() noexcept;
	};
	struct _impl_js_jString : virtual object_base {
		struct _tcjs_definitions {
			static constexpr auto keyof = boost::hana::make_map(
				boost::hana::make_pair("length"_s, boost::hana::type_c<double>)
			);
		};
		auto length() noexcept;
		static auto _tcjs_construct(tc::js::any value) noexcept;
		auto anchor(tc::js::string name) noexcept;
		auto big() noexcept;
		auto blink() noexcept;
		auto bold() noexcept;
		auto charAt(double pos) noexcept;
		auto charCodeAt(double index) noexcept;
		auto codePointAt(double pos) noexcept;
		auto concat(tc::jst::variadic_arg_t, tc::js::string strings) noexcept;
		auto endsWith(tc::js::string searchString, tc::jst::union_t<double, tc::js::undefined> endPosition = tc::js::undefined()) noexcept;
		auto fixed() noexcept;
		auto fontcolor(tc::js::string color) noexcept;
		auto fontsize(double size) noexcept;
		auto fontsize(tc::js::string size) noexcept;
		auto includes(tc::js::string searchString, tc::jst::union_t<double, tc::js::undefined> position = tc::js::undefined()) noexcept;
		auto indexOf(tc::js::string searchString, tc::jst::union_t<double, tc::js::undefined> position = tc::js::undefined()) noexcept;
		auto italics() noexcept;
		auto lastIndexOf(tc::js::string searchString, tc::jst::union_t<double, tc::js::undefined> position = tc::js::undefined()) noexcept;
		auto link(tc::js::string url) noexcept;
		auto localeCompare(tc::js::string that) noexcept;
		auto localeCompare(tc::js::string that, tc::jst::union_t<_js_jArray<tc::js::string>, tc::js::string, tc::js::undefined> locales = tc::js::undefined(), tc::jst::union_t<_js_jIntl_dCollatorOptions, tc::js::undefined> options = tc::js::undefined()) noexcept;
		auto match(tc::js::any /*AnonymousTypeWithLiteralType={ [Symbol.match](string: string): RegExpMatchArray | null; }[]*/ matcher) noexcept;
		auto match(tc::jst::union_t<_js_jRegExp, tc::js::string> regexp) noexcept;
		auto normalize(tc::js::string /*"NFC"*/ form) noexcept;
		auto normalize(tc::jst::union_t<tc::js::string, tc::js::undefined> form = tc::js::undefined()) noexcept;
		auto operator[](double index) noexcept;
		auto padEnd(double maxLength, tc::jst::union_t<tc::js::string, tc::js::undefined> fillString = tc::js::undefined()) noexcept;
		auto padStart(double maxLength, tc::jst::union_t<tc::js::string, tc::js::undefined> fillString = tc::js::undefined()) noexcept;
		auto repeat(double count) noexcept;
		auto replace(tc::js::any /*AnonymousTypeWithLiteralType={ [Symbol.replace](string: string, replaceValue: string): string; }[]*/ searchValue, tc::js::string replaceValue) noexcept;
		auto replace(tc::js::any /*AnonymousTypeWithLiteralType={ [Symbol.replace](string: string, replacer: (substring: string, ...args: any[]) => string): string; }[]*/ searchValue, tc::jst::function<tc::js::string(tc::js::string, _js_jArray<tc::js::any>)> replacer) noexcept;
		auto replace(tc::jst::union_t<_js_jRegExp, tc::js::string> searchValue, tc::js::string replaceValue) noexcept;
		auto replace(tc::jst::union_t<_js_jRegExp, tc::js::string> searchValue, tc::jst::function<tc::js::string(tc::js::string, _js_jArray<tc::js::any>)> replacer) noexcept;
		auto search(tc::js::any /*AnonymousTypeWithLiteralType={ [Symbol.search](string: string): number; }[]*/ searcher) noexcept;
		auto search(tc::jst::union_t<_js_jRegExp, tc::js::string> regexp) noexcept;
		auto slice(tc::jst::union_t<double, tc::js::undefined> start = tc::js::undefined(), tc::jst::union_t<double, tc::js::undefined> end = tc::js::undefined()) noexcept;
		auto small() noexcept;
		auto split(tc::js::any /*AnonymousTypeWithLiteralType={ [Symbol.split](string: string, limit?: number | undefined): string[]; }[]*/ splitter, tc::jst::union_t<double, tc::js::undefined> limit = tc::js::undefined()) noexcept;
		auto split(tc::jst::union_t<_js_jRegExp, tc::js::string> separator, tc::jst::union_t<double, tc::js::undefined> limit = tc::js::undefined()) noexcept;
		auto startsWith(tc::js::string searchString, tc::jst::union_t<double, tc::js::undefined> position = tc::js::undefined()) noexcept;
		auto strike() noexcept;
		auto sub() noexcept;
		auto substr(double from, tc::jst::union_t<double, tc::js::undefined> length = tc::js::undefined()) noexcept;
		auto substring(double start, tc::jst::union_t<double, tc::js::undefined> end = tc::js::undefined()) noexcept;
		auto sup() noexcept;
		auto toLocaleLowerCase(tc::jst::union_t<_js_jArray<tc::js::string>, tc::js::string, tc::js::undefined> locales = tc::js::undefined()) noexcept;
		auto toLocaleUpperCase(tc::jst::union_t<_js_jArray<tc::js::string>, tc::js::string, tc::js::undefined> locales = tc::js::undefined()) noexcept;
		auto toLowerCase() noexcept;
		auto toString() noexcept;
		auto toUpperCase() noexcept;
		auto trim() noexcept;
		auto trimLeft() noexcept;
		auto trimRight() noexcept;
		auto valueOf() noexcept;
	};
	struct _impl_js_jStringConstructor : virtual object_base {
		struct _tcjs_definitions {
			static constexpr auto keyof = boost::hana::make_map(
				boost::hana::make_pair("prototype"_s, boost::hana::type_c<_js_jString>)
			);
		};
		auto prototype() noexcept;
		auto fromCharCode(tc::jst::variadic_arg_t, double codes) noexcept;
		auto fromCodePoint(tc::jst::variadic_arg_t, double codePoints) noexcept;
		auto raw(tc::js::any /*AnonymousTypeWithLiteralType={ raw: readonly string[] | ArrayLike<string>; }[raw]*/ template_, tc::jst::variadic_arg_t, tc::js::any substitutions) noexcept;
	};
	struct _impl_js_jSymbol : virtual object_base {
		struct _tcjs_definitions {
			static constexpr auto keyof = boost::hana::make_map(

			);
		};
		auto toString() noexcept;
		auto valueOf() noexcept;
	};
	struct _impl_js_jSymbolConstructor : virtual object_base {
		struct _tcjs_definitions {
			static constexpr auto keyof = boost::hana::make_map(
				boost::hana::make_pair("prototype"_s, boost::hana::type_c<_js_jSymbol>),
				boost::hana::make_pair("asyncIterator"_s, boost::hana::type_c<tc::js::any /* unsupported type flags =8192: unique symbol*/>),
				boost::hana::make_pair("iterator"_s, boost::hana::type_c<tc::js::any /* unsupported type flags =8192: unique symbol*/>),
				boost::hana::make_pair("hasInstance"_s, boost::hana::type_c<tc::js::any /* unsupported type flags =8192: unique symbol*/>),
				boost::hana::make_pair("isConcatSpreadable"_s, boost::hana::type_c<tc::js::any /* unsupported type flags =8192: unique symbol*/>),
				boost::hana::make_pair("match"_s, boost::hana::type_c<tc::js::any /* unsupported type flags =8192: unique symbol*/>),
				boost::hana::make_pair("replace"_s, boost::hana::type_c<tc::js::any /* unsupported type flags =8192: unique symbol*/>),
				boost::hana::make_pair("search"_s, boost::hana::type_c<tc::js::any /* unsupported type flags =8192: unique symbol*/>),
				boost::hana::make_pair("species"_s, boost::hana::type_c<tc::js::any /* unsupported type flags =8192: unique symbol*/>),
				boost::hana::make_pair("split"_s, boost::hana::type_c<tc::js::any /* unsupported type flags =8192: unique symbol*/>),
				boost::hana::make_pair("toPrimitive"_s, boost::hana::type_c<tc::js::any /* unsupported type flags =8192: unique symbol*/>),
				boost::hana::make_pair("toStringTag"_s, boost::hana::type_c<tc::js::any /* unsupported type flags =8192: unique symbol*/>),
				boost::hana::make_pair("unscopables"_s, boost::hana::type_c<tc::js::any /* unsupported type flags =8192: unique symbol*/>)
			);
		};
		auto prototype() noexcept;
		auto asyncIterator() noexcept;
		auto iterator() noexcept;
		auto hasInstance() noexcept;
		auto isConcatSpreadable() noexcept;
		auto match() noexcept;
		auto replace() noexcept;
		auto search() noexcept;
		auto species() noexcept;
		auto split() noexcept;
		auto toPrimitive() noexcept;
		auto toStringTag() noexcept;
		auto unscopables() noexcept;
		auto for_(tc::js::string key) noexcept;
		auto keyFor(tc::js::any /* unsupported type flags =4096: symbol*/ sym) noexcept;
	};
	struct _impl_js_jSyntaxError : virtual _impl_js_jError {
		struct _tcjs_definitions {
			static constexpr auto keyof = boost::hana::make_map(

			);
		};
		static auto _tcjs_construct(tc::jst::union_t<tc::js::string, tc::js::undefined> message = tc::js::undefined()) noexcept;
	};
	struct _impl_js_jSyntaxErrorConstructor : virtual _impl_js_jErrorConstructor {
		struct _tcjs_definitions {
			static constexpr auto keyof = boost::hana::make_map(
				boost::hana::make_pair("prototype"_s, boost::hana::type_c<_js_jSyntaxError>)
			);
		};
		auto prototype() noexcept;
	};
	struct _impl_js_jTemplateStringsArray : virtual _impl_js_jReadonlyArray<tc::js::string> {
		struct _tcjs_definitions {
			static constexpr auto keyof = boost::hana::make_map(
				boost::hana::make_pair("raw"_s, boost::hana::type_c<_js_jReadonlyArray<tc::js::string>>)
			);
		};
		auto raw() noexcept;
	};
	template<typename T>
	struct _impl_js_jThisType : virtual object_base {
		struct _tcjs_definitions {
			static constexpr auto keyof = boost::hana::make_map(

			);
		};
		static auto _tcjs_construct() noexcept;
	};
	struct _impl_js_jTypeError : virtual _impl_js_jError {
		struct _tcjs_definitions {
			static constexpr auto keyof = boost::hana::make_map(

			);
		};
		static auto _tcjs_construct(tc::jst::union_t<tc::js::string, tc::js::undefined> message = tc::js::undefined()) noexcept;
	};
	struct _impl_js_jTypeErrorConstructor : virtual _impl_js_jErrorConstructor {
		struct _tcjs_definitions {
			static constexpr auto keyof = boost::hana::make_map(
				boost::hana::make_pair("prototype"_s, boost::hana::type_c<_js_jTypeError>)
			);
		};
		auto prototype() noexcept;
	};
	template<typename T>
	struct _impl_js_jTypedPropertyDescriptor : virtual object_base {
		struct _tcjs_definitions {
			static constexpr auto keyof = boost::hana::make_map(
				boost::hana::make_pair("enumerable"_s, boost::hana::type_c<tc::jst::union_t<bool /*false*/, tc::js::undefined>>),
				boost::hana::make_pair("configurable"_s, boost::hana::type_c<tc::jst::union_t<bool /*false*/, tc::js::undefined>>),
				boost::hana::make_pair("writable"_s, boost::hana::type_c<tc::jst::union_t<bool /*false*/, tc::js::undefined>>),
				boost::hana::make_pair("value"_s, boost::hana::type_c<tc::jst::union_t<T, tc::js::undefined>>),
				boost::hana::make_pair("get"_s, boost::hana::type_c<tc::jst::union_t<tc::js::undefined, tc::jst::function<T()>>>),
				boost::hana::make_pair("set"_s, boost::hana::type_c<tc::jst::union_t<tc::js::undefined, tc::jst::function<void(T)>>>)
			);
		};
		auto enumerable() noexcept;
		void enumerable(tc::jst::union_t<bool /*false*/, tc::js::undefined> v) noexcept;
		auto configurable() noexcept;
		void configurable(tc::jst::union_t<bool /*false*/, tc::js::undefined> v) noexcept;
		auto writable() noexcept;
		void writable(tc::jst::union_t<bool /*false*/, tc::js::undefined> v) noexcept;
		auto value() noexcept;
		void value(tc::jst::union_t<T, tc::js::undefined> v) noexcept;
		auto get() noexcept;
		void get(tc::jst::union_t<tc::js::undefined, tc::jst::function<T()>> v) noexcept;
		auto set() noexcept;
		void set(tc::jst::union_t<tc::js::undefined, tc::jst::function<void(T)>> v) noexcept;
		static auto _tcjs_construct() noexcept;
	};
	struct _impl_js_jURIError : virtual _impl_js_jError {
		struct _tcjs_definitions {
			static constexpr auto keyof = boost::hana::make_map(

			);
		};
		static auto _tcjs_construct(tc::jst::union_t<tc::js::string, tc::js::undefined> message = tc::js::undefined()) noexcept;
	};
	struct _impl_js_jURIErrorConstructor : virtual _impl_js_jErrorConstructor {
		struct _tcjs_definitions {
			static constexpr auto keyof = boost::hana::make_map(
				boost::hana::make_pair("prototype"_s, boost::hana::type_c<_js_jURIError>)
			);
		};
		auto prototype() noexcept;
	};
	struct _impl_js_jUint16Array : virtual object_base {
		struct _tcjs_definitions {
			static constexpr auto keyof = boost::hana::make_map(
				boost::hana::make_pair("BYTES_PER_ELEMENT"_s, boost::hana::type_c<double>),
				boost::hana::make_pair("buffer"_s, boost::hana::type_c<_js_jArrayBufferLike>),
				boost::hana::make_pair("byteLength"_s, boost::hana::type_c<double>),
				boost::hana::make_pair("byteOffset"_s, boost::hana::type_c<double>),
				boost::hana::make_pair("length"_s, boost::hana::type_c<double>)
			);
		};
		auto BYTES_PER_ELEMENT() noexcept;
		auto buffer() noexcept;
		auto byteLength() noexcept;
		auto byteOffset() noexcept;
		auto length() noexcept;
		static auto _tcjs_construct() noexcept;
		static auto _tcjs_construct(double length) noexcept;
		static auto _tcjs_construct(tc::js::any /*UnionWithUnsupportedSubtype=[_js_jArrayBuffer,tc::js::any,_js_jArrayLike<double>]*/ array) noexcept;
		static auto _tcjs_construct(_js_jArrayBufferLike buffer, tc::jst::union_t<double, tc::js::undefined> byteOffset = tc::js::undefined(), tc::jst::union_t<double, tc::js::undefined> length = tc::js::undefined()) noexcept;
		static auto _tcjs_construct(_js_jIterable<double> elements) noexcept;
		auto copyWithin(double target, double start, tc::jst::union_t<double, tc::js::undefined> end = tc::js::undefined()) noexcept;
		auto entries() noexcept;
		auto every(tc::jst::function<tc::js::any(double, double, _js_jUint16Array)> predicate, tc::js::any thisArg) noexcept;
		auto fill(double value, tc::jst::union_t<double, tc::js::undefined> start = tc::js::undefined(), tc::jst::union_t<double, tc::js::undefined> end = tc::js::undefined()) noexcept;
		auto filter(tc::jst::function<tc::js::any(double, double, _js_jUint16Array)> predicate, tc::js::any thisArg) noexcept;
		auto find(tc::jst::function<bool /*false*/(double, double, _js_jUint16Array)> predicate, tc::js::any thisArg) noexcept;
		auto findIndex(tc::jst::function<bool /*false*/(double, double, _js_jUint16Array)> predicate, tc::js::any thisArg) noexcept;
		auto forEach(tc::jst::function<void(double, double, _js_jUint16Array)> callbackfn, tc::js::any thisArg) noexcept;
		auto includes(double searchElement, tc::jst::union_t<double, tc::js::undefined> fromIndex = tc::js::undefined()) noexcept;
		auto indexOf(double searchElement, tc::jst::union_t<double, tc::js::undefined> fromIndex = tc::js::undefined()) noexcept;
		auto join(tc::jst::union_t<tc::js::string, tc::js::undefined> separator = tc::js::undefined()) noexcept;
		auto keys() noexcept;
		auto lastIndexOf(double searchElement, tc::jst::union_t<double, tc::js::undefined> fromIndex = tc::js::undefined()) noexcept;
		auto map(tc::jst::function<double(double, double, _js_jUint16Array)> callbackfn, tc::js::any thisArg) noexcept;
		auto operator[](double index) noexcept;
		template<typename U>
		auto reduce(tc::jst::function<U(U, double, double, _js_jUint16Array)> callbackfn, U initialValue) noexcept;
		auto reduce(tc::jst::function<double(double, double, double, _js_jUint16Array)> callbackfn) noexcept;
		auto reduce(tc::jst::function<double(double, double, double, _js_jUint16Array)> callbackfn, double initialValue) noexcept;
		template<typename U>
		auto reduceRight(tc::jst::function<U(U, double, double, _js_jUint16Array)> callbackfn, U initialValue) noexcept;
		auto reduceRight(tc::jst::function<double(double, double, double, _js_jUint16Array)> callbackfn) noexcept;
		auto reduceRight(tc::jst::function<double(double, double, double, _js_jUint16Array)> callbackfn, double initialValue) noexcept;
		auto reverse() noexcept;
		auto set(_js_jArrayLike<double> array, tc::jst::union_t<double, tc::js::undefined> offset = tc::js::undefined()) noexcept;
		auto slice(tc::jst::union_t<double, tc::js::undefined> start = tc::js::undefined(), tc::jst::union_t<double, tc::js::undefined> end = tc::js::undefined()) noexcept;
		auto some(tc::jst::function<tc::js::any(double, double, _js_jUint16Array)> predicate, tc::js::any thisArg) noexcept;
		auto sort(tc::jst::union_t<tc::js::undefined, tc::jst::function<double(double, double)>> compareFn = tc::js::undefined()) noexcept;
		auto subarray(tc::jst::union_t<double, tc::js::undefined> begin = tc::js::undefined(), tc::jst::union_t<double, tc::js::undefined> end = tc::js::undefined()) noexcept;
		auto toLocaleString() noexcept;
		auto toString() noexcept;
		auto valueOf() noexcept;
		auto values() noexcept;
	};
	struct _impl_js_jUint16ArrayConstructor : virtual object_base {
		struct _tcjs_definitions {
			static constexpr auto keyof = boost::hana::make_map(
				boost::hana::make_pair("prototype"_s, boost::hana::type_c<_js_jUint16Array>),
				boost::hana::make_pair("BYTES_PER_ELEMENT"_s, boost::hana::type_c<double>)
			);
		};
		auto prototype() noexcept;
		auto BYTES_PER_ELEMENT() noexcept;
		template<typename T>
		auto from(_js_jArrayLike<T> arrayLike, tc::jst::function<double(T, double)> mapfn, tc::js::any thisArg) noexcept;
		auto from(_js_jArrayLike<double> arrayLike) noexcept;
		auto from(_js_jIterable<double> arrayLike, tc::jst::union_t<tc::js::undefined, tc::jst::function<double(double, double)>> mapfn, tc::js::any thisArg) noexcept;
		auto of(tc::jst::variadic_arg_t, double items) noexcept;
	};
	struct _impl_js_jUint32Array : virtual object_base {
		struct _tcjs_definitions {
			static constexpr auto keyof = boost::hana::make_map(
				boost::hana::make_pair("BYTES_PER_ELEMENT"_s, boost::hana::type_c<double>),
				boost::hana::make_pair("buffer"_s, boost::hana::type_c<_js_jArrayBufferLike>),
				boost::hana::make_pair("byteLength"_s, boost::hana::type_c<double>),
				boost::hana::make_pair("byteOffset"_s, boost::hana::type_c<double>),
				boost::hana::make_pair("length"_s, boost::hana::type_c<double>)
			);
		};
		auto BYTES_PER_ELEMENT() noexcept;
		auto buffer() noexcept;
		auto byteLength() noexcept;
		auto byteOffset() noexcept;
		auto length() noexcept;
		static auto _tcjs_construct() noexcept;
		static auto _tcjs_construct(double length) noexcept;
		static auto _tcjs_construct(tc::js::any /*UnionWithUnsupportedSubtype=[_js_jArrayBuffer,tc::js::any,_js_jArrayLike<double>]*/ array) noexcept;
		static auto _tcjs_construct(_js_jArrayBufferLike buffer, tc::jst::union_t<double, tc::js::undefined> byteOffset = tc::js::undefined(), tc::jst::union_t<double, tc::js::undefined> length = tc::js::undefined()) noexcept;
		static auto _tcjs_construct(_js_jIterable<double> elements) noexcept;
		auto copyWithin(double target, double start, tc::jst::union_t<double, tc::js::undefined> end = tc::js::undefined()) noexcept;
		auto entries() noexcept;
		auto every(tc::jst::function<tc::js::any(double, double, _js_jUint32Array)> predicate, tc::js::any thisArg) noexcept;
		auto fill(double value, tc::jst::union_t<double, tc::js::undefined> start = tc::js::undefined(), tc::jst::union_t<double, tc::js::undefined> end = tc::js::undefined()) noexcept;
		auto filter(tc::jst::function<tc::js::any(double, double, _js_jUint32Array)> predicate, tc::js::any thisArg) noexcept;
		auto find(tc::jst::function<bool /*false*/(double, double, _js_jUint32Array)> predicate, tc::js::any thisArg) noexcept;
		auto findIndex(tc::jst::function<bool /*false*/(double, double, _js_jUint32Array)> predicate, tc::js::any thisArg) noexcept;
		auto forEach(tc::jst::function<void(double, double, _js_jUint32Array)> callbackfn, tc::js::any thisArg) noexcept;
		auto includes(double searchElement, tc::jst::union_t<double, tc::js::undefined> fromIndex = tc::js::undefined()) noexcept;
		auto indexOf(double searchElement, tc::jst::union_t<double, tc::js::undefined> fromIndex = tc::js::undefined()) noexcept;
		auto join(tc::jst::union_t<tc::js::string, tc::js::undefined> separator = tc::js::undefined()) noexcept;
		auto keys() noexcept;
		auto lastIndexOf(double searchElement, tc::jst::union_t<double, tc::js::undefined> fromIndex = tc::js::undefined()) noexcept;
		auto map(tc::jst::function<double(double, double, _js_jUint32Array)> callbackfn, tc::js::any thisArg) noexcept;
		auto operator[](double index) noexcept;
		template<typename U>
		auto reduce(tc::jst::function<U(U, double, double, _js_jUint32Array)> callbackfn, U initialValue) noexcept;
		auto reduce(tc::jst::function<double(double, double, double, _js_jUint32Array)> callbackfn) noexcept;
		auto reduce(tc::jst::function<double(double, double, double, _js_jUint32Array)> callbackfn, double initialValue) noexcept;
		template<typename U>
		auto reduceRight(tc::jst::function<U(U, double, double, _js_jUint32Array)> callbackfn, U initialValue) noexcept;
		auto reduceRight(tc::jst::function<double(double, double, double, _js_jUint32Array)> callbackfn) noexcept;
		auto reduceRight(tc::jst::function<double(double, double, double, _js_jUint32Array)> callbackfn, double initialValue) noexcept;
		auto reverse() noexcept;
		auto set(_js_jArrayLike<double> array, tc::jst::union_t<double, tc::js::undefined> offset = tc::js::undefined()) noexcept;
		auto slice(tc::jst::union_t<double, tc::js::undefined> start = tc::js::undefined(), tc::jst::union_t<double, tc::js::undefined> end = tc::js::undefined()) noexcept;
		auto some(tc::jst::function<tc::js::any(double, double, _js_jUint32Array)> predicate, tc::js::any thisArg) noexcept;
		auto sort(tc::jst::union_t<tc::js::undefined, tc::jst::function<double(double, double)>> compareFn = tc::js::undefined()) noexcept;
		auto subarray(tc::jst::union_t<double, tc::js::undefined> begin = tc::js::undefined(), tc::jst::union_t<double, tc::js::undefined> end = tc::js::undefined()) noexcept;
		auto toLocaleString() noexcept;
		auto toString() noexcept;
		auto valueOf() noexcept;
		auto values() noexcept;
	};
	struct _impl_js_jUint32ArrayConstructor : virtual object_base {
		struct _tcjs_definitions {
			static constexpr auto keyof = boost::hana::make_map(
				boost::hana::make_pair("prototype"_s, boost::hana::type_c<_js_jUint32Array>),
				boost::hana::make_pair("BYTES_PER_ELEMENT"_s, boost::hana::type_c<double>)
			);
		};
		auto prototype() noexcept;
		auto BYTES_PER_ELEMENT() noexcept;
		template<typename T>
		auto from(_js_jArrayLike<T> arrayLike, tc::jst::function<double(T, double)> mapfn, tc::js::any thisArg) noexcept;
		auto from(_js_jArrayLike<double> arrayLike) noexcept;
		auto from(_js_jIterable<double> arrayLike, tc::jst::union_t<tc::js::undefined, tc::jst::function<double(double, double)>> mapfn, tc::js::any thisArg) noexcept;
		auto of(tc::jst::variadic_arg_t, double items) noexcept;
	};
	struct _impl_js_jUint8Array : virtual object_base {
		struct _tcjs_definitions {
			static constexpr auto keyof = boost::hana::make_map(
				boost::hana::make_pair("BYTES_PER_ELEMENT"_s, boost::hana::type_c<double>),
				boost::hana::make_pair("buffer"_s, boost::hana::type_c<_js_jArrayBufferLike>),
				boost::hana::make_pair("byteLength"_s, boost::hana::type_c<double>),
				boost::hana::make_pair("byteOffset"_s, boost::hana::type_c<double>),
				boost::hana::make_pair("length"_s, boost::hana::type_c<double>)
			);
		};
		auto BYTES_PER_ELEMENT() noexcept;
		auto buffer() noexcept;
		auto byteLength() noexcept;
		auto byteOffset() noexcept;
		auto length() noexcept;
		static auto _tcjs_construct() noexcept;
		static auto _tcjs_construct(double length) noexcept;
		static auto _tcjs_construct(tc::js::any /*UnionWithUnsupportedSubtype=[_js_jArrayBuffer,tc::js::any,_js_jArrayLike<double>]*/ array) noexcept;
		static auto _tcjs_construct(_js_jArrayBufferLike buffer, tc::jst::union_t<double, tc::js::undefined> byteOffset = tc::js::undefined(), tc::jst::union_t<double, tc::js::undefined> length = tc::js::undefined()) noexcept;
		static auto _tcjs_construct(_js_jIterable<double> elements) noexcept;
		auto copyWithin(double target, double start, tc::jst::union_t<double, tc::js::undefined> end = tc::js::undefined()) noexcept;
		auto entries() noexcept;
		auto every(tc::jst::function<tc::js::any(double, double, _js_jUint8Array)> predicate, tc::js::any thisArg) noexcept;
		auto fill(double value, tc::jst::union_t<double, tc::js::undefined> start = tc::js::undefined(), tc::jst::union_t<double, tc::js::undefined> end = tc::js::undefined()) noexcept;
		auto filter(tc::jst::function<tc::js::any(double, double, _js_jUint8Array)> predicate, tc::js::any thisArg) noexcept;
		auto find(tc::jst::function<bool /*false*/(double, double, _js_jUint8Array)> predicate, tc::js::any thisArg) noexcept;
		auto findIndex(tc::jst::function<bool /*false*/(double, double, _js_jUint8Array)> predicate, tc::js::any thisArg) noexcept;
		auto forEach(tc::jst::function<void(double, double, _js_jUint8Array)> callbackfn, tc::js::any thisArg) noexcept;
		auto includes(double searchElement, tc::jst::union_t<double, tc::js::undefined> fromIndex = tc::js::undefined()) noexcept;
		auto indexOf(double searchElement, tc::jst::union_t<double, tc::js::undefined> fromIndex = tc::js::undefined()) noexcept;
		auto join(tc::jst::union_t<tc::js::string, tc::js::undefined> separator = tc::js::undefined()) noexcept;
		auto keys() noexcept;
		auto lastIndexOf(double searchElement, tc::jst::union_t<double, tc::js::undefined> fromIndex = tc::js::undefined()) noexcept;
		auto map(tc::jst::function<double(double, double, _js_jUint8Array)> callbackfn, tc::js::any thisArg) noexcept;
		auto operator[](double index) noexcept;
		template<typename U>
		auto reduce(tc::jst::function<U(U, double, double, _js_jUint8Array)> callbackfn, U initialValue) noexcept;
		auto reduce(tc::jst::function<double(double, double, double, _js_jUint8Array)> callbackfn) noexcept;
		auto reduce(tc::jst::function<double(double, double, double, _js_jUint8Array)> callbackfn, double initialValue) noexcept;
		template<typename U>
		auto reduceRight(tc::jst::function<U(U, double, double, _js_jUint8Array)> callbackfn, U initialValue) noexcept;
		auto reduceRight(tc::jst::function<double(double, double, double, _js_jUint8Array)> callbackfn) noexcept;
		auto reduceRight(tc::jst::function<double(double, double, double, _js_jUint8Array)> callbackfn, double initialValue) noexcept;
		auto reverse() noexcept;
		auto set(_js_jArrayLike<double> array, tc::jst::union_t<double, tc::js::undefined> offset = tc::js::undefined()) noexcept;
		auto slice(tc::jst::union_t<double, tc::js::undefined> start = tc::js::undefined(), tc::jst::union_t<double, tc::js::undefined> end = tc::js::undefined()) noexcept;
		auto some(tc::jst::function<tc::js::any(double, double, _js_jUint8Array)> predicate, tc::js::any thisArg) noexcept;
		auto sort(tc::jst::union_t<tc::js::undefined, tc::jst::function<double(double, double)>> compareFn = tc::js::undefined()) noexcept;
		auto subarray(tc::jst::union_t<double, tc::js::undefined> begin = tc::js::undefined(), tc::jst::union_t<double, tc::js::undefined> end = tc::js::undefined()) noexcept;
		auto toLocaleString() noexcept;
		auto toString() noexcept;
		auto valueOf() noexcept;
		auto values() noexcept;
	};
	struct _impl_js_jUint8ArrayConstructor : virtual object_base {
		struct _tcjs_definitions {
			static constexpr auto keyof = boost::hana::make_map(
				boost::hana::make_pair("prototype"_s, boost::hana::type_c<_js_jUint8Array>),
				boost::hana::make_pair("BYTES_PER_ELEMENT"_s, boost::hana::type_c<double>)
			);
		};
		auto prototype() noexcept;
		auto BYTES_PER_ELEMENT() noexcept;
		template<typename T>
		auto from(_js_jArrayLike<T> arrayLike, tc::jst::function<double(T, double)> mapfn, tc::js::any thisArg) noexcept;
		auto from(_js_jArrayLike<double> arrayLike) noexcept;
		auto from(_js_jIterable<double> arrayLike, tc::jst::union_t<tc::js::undefined, tc::jst::function<double(double, double)>> mapfn, tc::js::any thisArg) noexcept;
		auto of(tc::jst::variadic_arg_t, double items) noexcept;
	};
	struct _impl_js_jUint8ClampedArray : virtual object_base {
		struct _tcjs_definitions {
			static constexpr auto keyof = boost::hana::make_map(
				boost::hana::make_pair("BYTES_PER_ELEMENT"_s, boost::hana::type_c<double>),
				boost::hana::make_pair("buffer"_s, boost::hana::type_c<_js_jArrayBufferLike>),
				boost::hana::make_pair("byteLength"_s, boost::hana::type_c<double>),
				boost::hana::make_pair("byteOffset"_s, boost::hana::type_c<double>),
				boost::hana::make_pair("length"_s, boost::hana::type_c<double>)
			);
		};
		auto BYTES_PER_ELEMENT() noexcept;
		auto buffer() noexcept;
		auto byteLength() noexcept;
		auto byteOffset() noexcept;
		auto length() noexcept;
		static auto _tcjs_construct() noexcept;
		static auto _tcjs_construct(double length) noexcept;
		static auto _tcjs_construct(tc::js::any /*UnionWithUnsupportedSubtype=[_js_jArrayBuffer,tc::js::any,_js_jArrayLike<double>]*/ array) noexcept;
		static auto _tcjs_construct(_js_jArrayBufferLike buffer, tc::jst::union_t<double, tc::js::undefined> byteOffset = tc::js::undefined(), tc::jst::union_t<double, tc::js::undefined> length = tc::js::undefined()) noexcept;
		static auto _tcjs_construct(_js_jIterable<double> elements) noexcept;
		auto copyWithin(double target, double start, tc::jst::union_t<double, tc::js::undefined> end = tc::js::undefined()) noexcept;
		auto entries() noexcept;
		auto every(tc::jst::function<tc::js::any(double, double, _js_jUint8ClampedArray)> predicate, tc::js::any thisArg) noexcept;
		auto fill(double value, tc::jst::union_t<double, tc::js::undefined> start = tc::js::undefined(), tc::jst::union_t<double, tc::js::undefined> end = tc::js::undefined()) noexcept;
		auto filter(tc::jst::function<tc::js::any(double, double, _js_jUint8ClampedArray)> predicate, tc::js::any thisArg) noexcept;
		auto find(tc::jst::function<bool /*false*/(double, double, _js_jUint8ClampedArray)> predicate, tc::js::any thisArg) noexcept;
		auto findIndex(tc::jst::function<bool /*false*/(double, double, _js_jUint8ClampedArray)> predicate, tc::js::any thisArg) noexcept;
		auto forEach(tc::jst::function<void(double, double, _js_jUint8ClampedArray)> callbackfn, tc::js::any thisArg) noexcept;
		auto includes(double searchElement, tc::jst::union_t<double, tc::js::undefined> fromIndex = tc::js::undefined()) noexcept;
		auto indexOf(double searchElement, tc::jst::union_t<double, tc::js::undefined> fromIndex = tc::js::undefined()) noexcept;
		auto join(tc::jst::union_t<tc::js::string, tc::js::undefined> separator = tc::js::undefined()) noexcept;
		auto keys() noexcept;
		auto lastIndexOf(double searchElement, tc::jst::union_t<double, tc::js::undefined> fromIndex = tc::js::undefined()) noexcept;
		auto map(tc::jst::function<double(double, double, _js_jUint8ClampedArray)> callbackfn, tc::js::any thisArg) noexcept;
		auto operator[](double index) noexcept;
		template<typename U>
		auto reduce(tc::jst::function<U(U, double, double, _js_jUint8ClampedArray)> callbackfn, U initialValue) noexcept;
		auto reduce(tc::jst::function<double(double, double, double, _js_jUint8ClampedArray)> callbackfn) noexcept;
		auto reduce(tc::jst::function<double(double, double, double, _js_jUint8ClampedArray)> callbackfn, double initialValue) noexcept;
		template<typename U>
		auto reduceRight(tc::jst::function<U(U, double, double, _js_jUint8ClampedArray)> callbackfn, U initialValue) noexcept;
		auto reduceRight(tc::jst::function<double(double, double, double, _js_jUint8ClampedArray)> callbackfn) noexcept;
		auto reduceRight(tc::jst::function<double(double, double, double, _js_jUint8ClampedArray)> callbackfn, double initialValue) noexcept;
		auto reverse() noexcept;
		auto set(_js_jArrayLike<double> array, tc::jst::union_t<double, tc::js::undefined> offset = tc::js::undefined()) noexcept;
		auto slice(tc::jst::union_t<double, tc::js::undefined> start = tc::js::undefined(), tc::jst::union_t<double, tc::js::undefined> end = tc::js::undefined()) noexcept;
		auto some(tc::jst::function<tc::js::any(double, double, _js_jUint8ClampedArray)> predicate, tc::js::any thisArg) noexcept;
		auto sort(tc::jst::union_t<tc::js::undefined, tc::jst::function<double(double, double)>> compareFn = tc::js::undefined()) noexcept;
		auto subarray(tc::jst::union_t<double, tc::js::undefined> begin = tc::js::undefined(), tc::jst::union_t<double, tc::js::undefined> end = tc::js::undefined()) noexcept;
		auto toLocaleString() noexcept;
		auto toString() noexcept;
		auto valueOf() noexcept;
		auto values() noexcept;
	};
	struct _impl_js_jUint8ClampedArrayConstructor : virtual object_base {
		struct _tcjs_definitions {
			static constexpr auto keyof = boost::hana::make_map(
				boost::hana::make_pair("prototype"_s, boost::hana::type_c<_js_jUint8ClampedArray>),
				boost::hana::make_pair("BYTES_PER_ELEMENT"_s, boost::hana::type_c<double>)
			);
		};
		auto prototype() noexcept;
		auto BYTES_PER_ELEMENT() noexcept;
		template<typename T>
		auto from(_js_jArrayLike<T> arrayLike, tc::jst::function<double(T, double)> mapfn, tc::js::any thisArg) noexcept;
		auto from(_js_jArrayLike<double> arrayLike) noexcept;
		auto from(_js_jIterable<double> arrayLike, tc::jst::union_t<tc::js::undefined, tc::jst::function<double(double, double)>> mapfn, tc::js::any thisArg) noexcept;
		auto of(tc::jst::variadic_arg_t, double items) noexcept;
	};
	template<typename K, typename V>
	struct _impl_js_jWeakMap : virtual object_base {
		struct _tcjs_definitions {
			static constexpr auto keyof = boost::hana::make_map(

			);
		};
		static auto _tcjs_construct(tc::jst::union_t<_js_jReadonlyArray<std::tuple<K, V>>, tc::js::null, tc::js::undefined> entries = tc::js::undefined()) noexcept;
		static auto _tcjs_construct(_js_jIterable<std::tuple<K, V>> iterable) noexcept;
		auto delete_(K key) noexcept;
		auto get(K key) noexcept;
		auto has(K key) noexcept;
		auto set(K key, V value) noexcept;
	};
	struct _impl_js_jWeakMapConstructor : virtual object_base {
		struct _tcjs_definitions {
			static constexpr auto keyof = boost::hana::make_map(
				boost::hana::make_pair("prototype"_s, boost::hana::type_c<_js_jWeakMap<tc::js::any /* unsupported type flags =67108864: object*/, tc::js::any>>)
			);
		};
		auto prototype() noexcept;
	};
	template<typename T>
	struct _impl_js_jWeakSet : virtual object_base {
		struct _tcjs_definitions {
			static constexpr auto keyof = boost::hana::make_map(

			);
		};
		static auto _tcjs_construct(tc::jst::union_t<_js_jReadonlyArray<T>, tc::js::null, tc::js::undefined> values = tc::js::undefined()) noexcept;
		static auto _tcjs_construct(_js_jIterable<T> iterable) noexcept;
		auto add(T value) noexcept;
		auto delete_(T value) noexcept;
		auto has(T value) noexcept;
	};
	struct _impl_js_jWeakSetConstructor : virtual object_base {
		struct _tcjs_definitions {
			static constexpr auto keyof = boost::hana::make_map(
				boost::hana::make_pair("prototype"_s, boost::hana::type_c<_js_jWeakSet<tc::js::any /* unsupported type flags =67108864: object*/>>)
			);
		};
		auto prototype() noexcept;
	};
	template<typename T>
	inline auto _impl_js_jArray<T>::length() noexcept { return this->template _getProperty<double>("length"); }
	template<typename T>
	inline void _impl_js_jArray<T>::length(double v) noexcept { this->template _setProperty("length", v); }
	template<typename T>
	inline auto _impl_js_jArray<T>::_tcjs_construct(tc::jst::union_t<double, tc::js::undefined> arrayLength) noexcept {
		return _js_jArray<T>(emscripten::val::global("Array").new_(arrayLength));
	}
	template<typename T>
	inline auto _impl_js_jArray<T>::_tcjs_construct(double arrayLength) noexcept {
		return _js_jArray<T>(emscripten::val::global("Array").new_(arrayLength));
	}
	template<typename T>
	inline auto _impl_js_jArray<T>::_tcjs_construct(tc::jst::variadic_arg_t, T items) noexcept {
		return _js_jArray<T>(emscripten::val::global("Array").new_(items));
	}
	template<typename T>
	inline auto _impl_js_jArray<T>::concat(tc::jst::variadic_arg_t, _js_jConcatArray<T> items) noexcept {
		return this->template _call<_js_jArray<T>>("concat", items);
	}
	template<typename T>
	inline auto _impl_js_jArray<T>::concat(tc::jst::variadic_arg_t, tc::jst::union_t<T, _js_jConcatArray<T>> items) noexcept {
		return this->template _call<_js_jArray<T>>("concat", items);
	}
	template<typename T>
	inline auto _impl_js_jArray<T>::copyWithin(double target, double start, tc::jst::union_t<double, tc::js::undefined> end) noexcept {
		return this->template _call<_js_jArray>("copyWithin", target, start, end);
	}
	template<typename T>
	inline auto _impl_js_jArray<T>::entries() noexcept {
		return this->template _call<_js_jIterableIterator<std::tuple<double, T>>>("entries");
	}
	template<typename T>
	template<typename S>
	inline auto _impl_js_jArray<T>::every(tc::jst::function<bool /*false*/(T, double, _js_jArray<T>)> predicate, tc::js::any thisArg) noexcept {
		std::optional<_js_jArray<S>> result;
		if(this->template _call<bool /*false*/>("every", predicate, thisArg)) {
			result.emplace(_this<tc::js::any>());
		}
		return result;
	}
	template<typename T>
	inline auto _impl_js_jArray<T>::every(tc::jst::function<tc::js::any(T, double, _js_jArray<T>)> predicate, tc::js::any thisArg) noexcept {
		return this->template _call<bool /*false*/>("every", predicate, thisArg);
	}
	template<typename T>
	inline auto _impl_js_jArray<T>::fill(T value, tc::jst::union_t<double, tc::js::undefined> start, tc::jst::union_t<double, tc::js::undefined> end) noexcept {
		return this->template _call<_js_jArray>("fill", value, start, end);
	}
	template<typename T>
	template<typename S>
	inline auto _impl_js_jArray<T>::filter(tc::jst::function<bool /*false*/(T, double, _js_jArray<T>)> predicate, tc::js::any thisArg) noexcept {
		return this->template _call<_js_jArray<S>>("filter", predicate, thisArg);
	}
	template<typename T>
	inline auto _impl_js_jArray<T>::filter(tc::jst::function<tc::js::any(T, double, _js_jArray<T>)> predicate, tc::js::any thisArg) noexcept {
		return this->template _call<_js_jArray<T>>("filter", predicate, thisArg);
	}
	template<typename T>
	template<typename S>
	inline auto _impl_js_jArray<T>::find(tc::jst::function<bool /*false*/(T, double, _js_jArray<T>)> predicate, tc::js::any thisArg) noexcept {
		return this->template _call<tc::jst::union_t<S, tc::js::undefined>>("find", predicate, thisArg);
	}
	template<typename T>
	inline auto _impl_js_jArray<T>::find(tc::jst::function<tc::js::any(T, double, _js_jArray<T>)> predicate, tc::js::any thisArg) noexcept {
		return this->template _call<tc::jst::union_t<T, tc::js::undefined>>("find", predicate, thisArg);
	}
	template<typename T>
	inline auto _impl_js_jArray<T>::findIndex(tc::jst::function<tc::js::any(T, double, _js_jArray<T>)> predicate, tc::js::any thisArg) noexcept {
		return this->template _call<double>("findIndex", predicate, thisArg);
	}
	template<typename T>
	inline auto _impl_js_jArray<T>::forEach(tc::jst::function<void(T, double, _js_jArray<T>)> callbackfn, tc::js::any thisArg) noexcept {
		 this->template _call<void>("forEach", callbackfn, thisArg);
	}
	template<typename T>
	inline auto _impl_js_jArray<T>::includes(T searchElement, tc::jst::union_t<double, tc::js::undefined> fromIndex) noexcept {
		return this->template _call<bool /*false*/>("includes", searchElement, fromIndex);
	}
	template<typename T>
	inline auto _impl_js_jArray<T>::indexOf(T searchElement, tc::jst::union_t<double, tc::js::undefined> fromIndex) noexcept {
		return this->template _call<double>("indexOf", searchElement, fromIndex);
	}
	template<typename T>
	inline auto _impl_js_jArray<T>::join(tc::jst::union_t<tc::js::string, tc::js::undefined> separator) noexcept {
		return this->template _call<tc::js::string>("join", separator);
	}
	template<typename T>
	inline auto _impl_js_jArray<T>::keys() noexcept {
		return this->template _call<_js_jIterableIterator<double>>("keys");
	}
	template<typename T>
	inline auto _impl_js_jArray<T>::lastIndexOf(T searchElement, tc::jst::union_t<double, tc::js::undefined> fromIndex) noexcept {
		return this->template _call<double>("lastIndexOf", searchElement, fromIndex);
	}
	template<typename T>
	template<typename U>
	inline auto _impl_js_jArray<T>::map(tc::jst::function<U(T, double, _js_jArray<T>)> callbackfn, tc::js::any thisArg) noexcept {
		return this->template _call<_js_jArray<U>>("map", callbackfn, thisArg);
	}
	template<typename T>
	inline auto _impl_js_jArray<T>::operator[](double n) noexcept {
		return this->template _getProperty<T>(n);
	}
	template<typename T>
	inline auto _impl_js_jArray<T>::pop() noexcept {
		return this->template _call<tc::jst::union_t<T, tc::js::undefined>>("pop");
	}
	template<typename T>
	inline auto _impl_js_jArray<T>::push(tc::jst::variadic_arg_t, T items) noexcept {
		return this->template _call<double>("push", items);
	}
	template<typename T>
	inline auto _impl_js_jArray<T>::reduce(tc::jst::function<T(T, T, double, _js_jArray<T>)> callbackfn) noexcept {
		return this->template _call<T>("reduce", callbackfn);
	}
	template<typename T>
	inline auto _impl_js_jArray<T>::reduce(tc::jst::function<T(T, T, double, _js_jArray<T>)> callbackfn, T initialValue) noexcept {
		return this->template _call<T>("reduce", callbackfn, initialValue);
	}
	template<typename T>
	template<typename U>
	inline auto _impl_js_jArray<T>::reduce(tc::jst::function<U(U, T, double, _js_jArray<T>)> callbackfn, U initialValue) noexcept {
		return this->template _call<U>("reduce", callbackfn, initialValue);
	}
	template<typename T>
	inline auto _impl_js_jArray<T>::reduceRight(tc::jst::function<T(T, T, double, _js_jArray<T>)> callbackfn) noexcept {
		return this->template _call<T>("reduceRight", callbackfn);
	}
	template<typename T>
	inline auto _impl_js_jArray<T>::reduceRight(tc::jst::function<T(T, T, double, _js_jArray<T>)> callbackfn, T initialValue) noexcept {
		return this->template _call<T>("reduceRight", callbackfn, initialValue);
	}
	template<typename T>
	template<typename U>
	inline auto _impl_js_jArray<T>::reduceRight(tc::jst::function<U(U, T, double, _js_jArray<T>)> callbackfn, U initialValue) noexcept {
		return this->template _call<U>("reduceRight", callbackfn, initialValue);
	}
	template<typename T>
	inline auto _impl_js_jArray<T>::reverse() noexcept {
		return this->template _call<_js_jArray<T>>("reverse");
	}
	template<typename T>
	inline auto _impl_js_jArray<T>::shift() noexcept {
		return this->template _call<tc::jst::union_t<T, tc::js::undefined>>("shift");
	}
	template<typename T>
	inline auto _impl_js_jArray<T>::slice(tc::jst::union_t<double, tc::js::undefined> start, tc::jst::union_t<double, tc::js::undefined> end) noexcept {
		return this->template _call<_js_jArray<T>>("slice", start, end);
	}
	template<typename T>
	inline auto _impl_js_jArray<T>::some(tc::jst::function<tc::js::any(T, double, _js_jArray<T>)> predicate, tc::js::any thisArg) noexcept {
		return this->template _call<bool /*false*/>("some", predicate, thisArg);
	}
	template<typename T>
	inline auto _impl_js_jArray<T>::sort(tc::jst::union_t<tc::js::undefined, tc::jst::function<double(T, T)>> compareFn) noexcept {
		return this->template _call<_js_jArray>("sort", compareFn);
	}
	template<typename T>
	inline auto _impl_js_jArray<T>::splice(double start, double deleteCount, tc::jst::variadic_arg_t, T items) noexcept {
		return this->template _call<_js_jArray<T>>("splice", start, deleteCount, items);
	}
	template<typename T>
	inline auto _impl_js_jArray<T>::splice(double start, tc::jst::union_t<double, tc::js::undefined> deleteCount) noexcept {
		return this->template _call<_js_jArray<T>>("splice", start, deleteCount);
	}
	template<typename T>
	inline auto _impl_js_jArray<T>::toLocaleString() noexcept {
		return this->template _call<tc::js::string>("toLocaleString");
	}
	template<typename T>
	inline auto _impl_js_jArray<T>::toString() noexcept {
		return this->template _call<tc::js::string>("toString");
	}
	template<typename T>
	inline auto _impl_js_jArray<T>::unshift(tc::jst::variadic_arg_t, T items) noexcept {
		return this->template _call<double>("unshift", items);
	}
	template<typename T>
	inline auto _impl_js_jArray<T>::values() noexcept {
		return this->template _call<_js_jIterableIterator<T>>("values");
	}
	inline auto _impl_js_jArrayBuffer::byteLength() noexcept { return this->template _getProperty<double>("byteLength"); }
	inline auto _impl_js_jArrayBuffer::_tcjs_construct(double byteLength) noexcept {
		return _js_jArrayBuffer(emscripten::val::global("ArrayBuffer").new_(byteLength));
	}
	inline auto _impl_js_jArrayBuffer::slice(double begin, tc::jst::union_t<double, tc::js::undefined> end) noexcept {
		return this->template _call<_js_jArrayBuffer>("slice", begin, end);
	}
	inline auto _impl_js_jArrayBufferConstructor::prototype() noexcept { return this->template _getProperty<_js_jArrayBuffer>("prototype"); }
	inline auto _impl_js_jArrayBufferConstructor::isView(tc::js::any arg) noexcept {
		std::optional<_js_jArrayBufferView> result;
		if(this->template _call<bool /*false*/>("isView", arg)) {
			result.emplace(tc::js::any(arg));
		}
		return result;
	}
	inline auto _impl_js_jArrayBufferTypes::SharedArrayBuffer() noexcept { return this->template _getProperty<tc::js::any /*UnknownClassOrInterface=SharedArrayBuffer*/>("SharedArrayBuffer"); }
	inline void _impl_js_jArrayBufferTypes::SharedArrayBuffer(tc::js::any /*UnknownClassOrInterface=SharedArrayBuffer*/ v) noexcept { this->template _setProperty("SharedArrayBuffer", v); }
	inline auto _impl_js_jArrayBufferTypes::ArrayBuffer() noexcept { return this->template _getProperty<_js_jArrayBuffer>("ArrayBuffer"); }
	inline void _impl_js_jArrayBufferTypes::ArrayBuffer(_js_jArrayBuffer v) noexcept { this->template _setProperty("ArrayBuffer", v); }
	inline auto _impl_js_jArrayBufferView::buffer() noexcept { return this->template _getProperty<_js_jArrayBufferLike>("buffer"); }
	inline void _impl_js_jArrayBufferView::buffer(_js_jArrayBufferLike v) noexcept { this->template _setProperty("buffer", v); }
	inline auto _impl_js_jArrayBufferView::byteLength() noexcept { return this->template _getProperty<double>("byteLength"); }
	inline void _impl_js_jArrayBufferView::byteLength(double v) noexcept { this->template _setProperty("byteLength", v); }
	inline auto _impl_js_jArrayBufferView::byteOffset() noexcept { return this->template _getProperty<double>("byteOffset"); }
	inline void _impl_js_jArrayBufferView::byteOffset(double v) noexcept { this->template _setProperty("byteOffset", v); }
	inline auto _impl_js_jArrayConstructor::prototype() noexcept { return this->template _getProperty<_js_jArray<tc::js::any>>("prototype"); }
	template<typename T>
	inline auto _impl_js_jArrayConstructor::from(_js_jArrayLike<T> arrayLike) noexcept {
		return this->template _call<_js_jArray<T>>("from", arrayLike);
	}
	template<typename T, typename U>
	inline auto _impl_js_jArrayConstructor::from(_js_jArrayLike<T> arrayLike, tc::jst::function<U(T, double)> mapfn, tc::js::any thisArg) noexcept {
		return this->template _call<_js_jArray<U>>("from", arrayLike, mapfn, thisArg);
	}
	template<typename T>
	inline auto _impl_js_jArrayConstructor::from(tc::jst::union_t<_js_jArrayLike<T>, _js_jIterable<T>> iterable) noexcept {
		return this->template _call<_js_jArray<T>>("from", iterable);
	}
	template<typename T, typename U>
	inline auto _impl_js_jArrayConstructor::from(tc::jst::union_t<_js_jArrayLike<T>, _js_jIterable<T>> iterable, tc::jst::function<U(T, double)> mapfn, tc::js::any thisArg) noexcept {
		return this->template _call<_js_jArray<U>>("from", iterable, mapfn, thisArg);
	}
	inline auto _impl_js_jArrayConstructor::isArray(tc::js::any arg) noexcept {
		std::optional<_js_jArray<tc::js::any>> result;
		if(this->template _call<bool /*false*/>("isArray", arg)) {
			result.emplace(tc::js::any(arg));
		}
		return result;
	}
	template<typename T>
	inline auto _impl_js_jArrayConstructor::of(tc::jst::variadic_arg_t, T items) noexcept {
		return this->template _call<_js_jArray<T>>("of", items);
	}
	template<typename T>
	inline auto _impl_js_jArrayLike<T>::length() noexcept { return this->template _getProperty<double>("length"); }
	template<typename T>
	inline auto _impl_js_jArrayLike<T>::operator[](double n) noexcept {
		return this->template _getProperty<T>(n);
	}
	inline auto _impl_js_jBoolean::_tcjs_construct(tc::js::any value) noexcept {
		return _js_jBoolean(emscripten::val::global("Boolean").new_(value));
	}
	inline auto _impl_js_jBoolean::valueOf() noexcept {
		return this->template _call<bool /*false*/>("valueOf");
	}
	inline auto _impl_js_jBooleanConstructor::prototype() noexcept { return this->template _getProperty<_js_jBoolean>("prototype"); }
	inline auto _impl_js_jFunction::prototype() noexcept { return this->template _getProperty<tc::js::any>("prototype"); }
	inline void _impl_js_jFunction::prototype(tc::js::any v) noexcept { this->template _setProperty("prototype", v); }
	inline auto _impl_js_jFunction::length() noexcept { return this->template _getProperty<double>("length"); }
	inline auto _impl_js_jFunction::arguments() noexcept { return this->template _getProperty<tc::js::any>("arguments"); }
	inline void _impl_js_jFunction::arguments(tc::js::any v) noexcept { this->template _setProperty("arguments", v); }
	inline auto _impl_js_jFunction::caller() noexcept { return this->template _getProperty<_js_jFunction>("caller"); }
	inline void _impl_js_jFunction::caller(_js_jFunction v) noexcept { this->template _setProperty("caller", v); }
	inline auto _impl_js_jFunction::name() noexcept { return this->template _getProperty<tc::js::string>("name"); }
	inline auto _impl_js_jFunction::_tcjs_construct(tc::jst::variadic_arg_t, tc::js::string args) noexcept {
		return _js_jFunction(emscripten::val::global("Function").new_(args));
	}
	inline auto _impl_js_jFunction::apply(tc::js::any thisArg, tc::js::any argArray) noexcept {
		return this->template _call<tc::js::any>("apply", thisArg, argArray);
	}
	inline auto _impl_js_jFunction::bind(tc::js::any thisArg, tc::jst::variadic_arg_t, tc::js::any argArray) noexcept {
		return this->template _call<tc::js::any>("bind", thisArg, argArray);
	}
	inline auto _impl_js_jFunction::call(tc::js::any thisArg, tc::jst::variadic_arg_t, tc::js::any argArray) noexcept {
		return this->template _call<tc::js::any>("call", thisArg, argArray);
	}
	inline auto _impl_js_jFunction::toString() noexcept {
		return this->template _call<tc::js::string>("toString");
	}
	template<typename T, typename R>
	inline auto _impl_js_jCallableFunction::apply(T thisArg) noexcept {
		return this->template _call<R>("apply", thisArg);
	}
	template<typename T, typename A, typename R>
	inline auto _impl_js_jCallableFunction::apply(T thisArg, A args) noexcept {
		return this->template _call<R>("apply", thisArg, args);
	}
	template<typename T, typename A0, typename A, typename R>
	inline auto _impl_js_jCallableFunction::bind(T thisArg, A0 arg0) noexcept {
		return this->template _call<tc::jst::function<R(A)>>("bind", thisArg, arg0);
	}
	template<typename T, typename A0, typename A1, typename A, typename R>
	inline auto _impl_js_jCallableFunction::bind(T thisArg, A0 arg0, A1 arg1) noexcept {
		return this->template _call<tc::jst::function<R(A)>>("bind", thisArg, arg0, arg1);
	}
	template<typename T, typename A0, typename A1, typename A2, typename A, typename R>
	inline auto _impl_js_jCallableFunction::bind(T thisArg, A0 arg0, A1 arg1, A2 arg2) noexcept {
		return this->template _call<tc::jst::function<R(A)>>("bind", thisArg, arg0, arg1, arg2);
	}
	template<typename T, typename A0, typename A1, typename A2, typename A3, typename A, typename R>
	inline auto _impl_js_jCallableFunction::bind(T thisArg, A0 arg0, A1 arg1, A2 arg2, A3 arg3) noexcept {
		return this->template _call<tc::jst::function<R(A)>>("bind", thisArg, arg0, arg1, arg2, arg3);
	}
	template<typename T, typename AX, typename R>
	inline auto _impl_js_jCallableFunction::bind(T thisArg, tc::jst::variadic_arg_t, AX args) noexcept {
		return this->template _call<tc::jst::function<R(_js_jArray<AX>)>>("bind", thisArg, args);
	}
	template<typename T>
	inline auto _impl_js_jCallableFunction::bind(tc::js::any /* unsupported type flags =16777216: ThisParameterType<T>*/ thisArg) noexcept {
		return this->template _call<tc::js::any /* unsupported type flags =16777216: OmitThisParameter<T>*/>("bind", thisArg);
	}
	template<typename T, typename A, typename R>
	inline auto _impl_js_jCallableFunction::call(T thisArg, A args) noexcept {
		return this->template _call<R>("call", thisArg, args);
	}
	template<typename T>
	inline auto _impl_js_jConcatArray<T>::length() noexcept { return this->template _getProperty<double>("length"); }
	template<typename T>
	inline auto _impl_js_jConcatArray<T>::join(tc::jst::union_t<tc::js::string, tc::js::undefined> separator) noexcept {
		return this->template _call<tc::js::string>("join", separator);
	}
	template<typename T>
	inline auto _impl_js_jConcatArray<T>::operator[](double n) noexcept {
		return this->template _getProperty<T>(n);
	}
	template<typename T>
	inline auto _impl_js_jConcatArray<T>::slice(tc::jst::union_t<double, tc::js::undefined> start, tc::jst::union_t<double, tc::js::undefined> end) noexcept {
		return this->template _call<_js_jArray<T>>("slice", start, end);
	}
	inline auto _impl_js_jDataView::buffer() noexcept { return this->template _getProperty<_js_jArrayBuffer>("buffer"); }
	inline auto _impl_js_jDataView::byteLength() noexcept { return this->template _getProperty<double>("byteLength"); }
	inline auto _impl_js_jDataView::byteOffset() noexcept { return this->template _getProperty<double>("byteOffset"); }
	inline auto _impl_js_jDataView::_tcjs_construct(_js_jArrayBufferLike buffer, tc::jst::union_t<double, tc::js::undefined> byteOffset, tc::jst::union_t<double, tc::js::undefined> byteLength) noexcept {
		return _js_jDataView(emscripten::val::global("DataView").new_(buffer, byteOffset, byteLength));
	}
	inline auto _impl_js_jDataView::getBigInt64(double byteOffset, tc::jst::union_t<bool /*false*/, tc::js::undefined> littleEndian) noexcept {
		return this->template _call<tc::js::any /* unsupported type flags =64: bigint*/>("getBigInt64", byteOffset, littleEndian);
	}
	inline auto _impl_js_jDataView::getBigUint64(double byteOffset, tc::jst::union_t<bool /*false*/, tc::js::undefined> littleEndian) noexcept {
		return this->template _call<tc::js::any /* unsupported type flags =64: bigint*/>("getBigUint64", byteOffset, littleEndian);
	}
	inline auto _impl_js_jDataView::getFloat32(double byteOffset, tc::jst::union_t<bool /*false*/, tc::js::undefined> littleEndian) noexcept {
		return this->template _call<double>("getFloat32", byteOffset, littleEndian);
	}
	inline auto _impl_js_jDataView::getFloat64(double byteOffset, tc::jst::union_t<bool /*false*/, tc::js::undefined> littleEndian) noexcept {
		return this->template _call<double>("getFloat64", byteOffset, littleEndian);
	}
	inline auto _impl_js_jDataView::getInt16(double byteOffset, tc::jst::union_t<bool /*false*/, tc::js::undefined> littleEndian) noexcept {
		return this->template _call<double>("getInt16", byteOffset, littleEndian);
	}
	inline auto _impl_js_jDataView::getInt32(double byteOffset, tc::jst::union_t<bool /*false*/, tc::js::undefined> littleEndian) noexcept {
		return this->template _call<double>("getInt32", byteOffset, littleEndian);
	}
	inline auto _impl_js_jDataView::getInt8(double byteOffset) noexcept {
		return this->template _call<double>("getInt8", byteOffset);
	}
	inline auto _impl_js_jDataView::getUint16(double byteOffset, tc::jst::union_t<bool /*false*/, tc::js::undefined> littleEndian) noexcept {
		return this->template _call<double>("getUint16", byteOffset, littleEndian);
	}
	inline auto _impl_js_jDataView::getUint32(double byteOffset, tc::jst::union_t<bool /*false*/, tc::js::undefined> littleEndian) noexcept {
		return this->template _call<double>("getUint32", byteOffset, littleEndian);
	}
	inline auto _impl_js_jDataView::getUint8(double byteOffset) noexcept {
		return this->template _call<double>("getUint8", byteOffset);
	}
	inline auto _impl_js_jDataView::setBigInt64(double byteOffset, tc::js::any /* unsupported type flags =64: bigint*/ value, tc::jst::union_t<bool /*false*/, tc::js::undefined> littleEndian) noexcept {
		 this->template _call<void>("setBigInt64", byteOffset, value, littleEndian);
	}
	inline auto _impl_js_jDataView::setBigUint64(double byteOffset, tc::js::any /* unsupported type flags =64: bigint*/ value, tc::jst::union_t<bool /*false*/, tc::js::undefined> littleEndian) noexcept {
		 this->template _call<void>("setBigUint64", byteOffset, value, littleEndian);
	}
	inline auto _impl_js_jDataView::setFloat32(double byteOffset, double value, tc::jst::union_t<bool /*false*/, tc::js::undefined> littleEndian) noexcept {
		 this->template _call<void>("setFloat32", byteOffset, value, littleEndian);
	}
	inline auto _impl_js_jDataView::setFloat64(double byteOffset, double value, tc::jst::union_t<bool /*false*/, tc::js::undefined> littleEndian) noexcept {
		 this->template _call<void>("setFloat64", byteOffset, value, littleEndian);
	}
	inline auto _impl_js_jDataView::setInt16(double byteOffset, double value, tc::jst::union_t<bool /*false*/, tc::js::undefined> littleEndian) noexcept {
		 this->template _call<void>("setInt16", byteOffset, value, littleEndian);
	}
	inline auto _impl_js_jDataView::setInt32(double byteOffset, double value, tc::jst::union_t<bool /*false*/, tc::js::undefined> littleEndian) noexcept {
		 this->template _call<void>("setInt32", byteOffset, value, littleEndian);
	}
	inline auto _impl_js_jDataView::setInt8(double byteOffset, double value) noexcept {
		 this->template _call<void>("setInt8", byteOffset, value);
	}
	inline auto _impl_js_jDataView::setUint16(double byteOffset, double value, tc::jst::union_t<bool /*false*/, tc::js::undefined> littleEndian) noexcept {
		 this->template _call<void>("setUint16", byteOffset, value, littleEndian);
	}
	inline auto _impl_js_jDataView::setUint32(double byteOffset, double value, tc::jst::union_t<bool /*false*/, tc::js::undefined> littleEndian) noexcept {
		 this->template _call<void>("setUint32", byteOffset, value, littleEndian);
	}
	inline auto _impl_js_jDataView::setUint8(double byteOffset, double value) noexcept {
		 this->template _call<void>("setUint8", byteOffset, value);
	}
	inline auto _impl_js_jDataViewConstructor::prototype() noexcept { return this->template _getProperty<_js_jDataView>("prototype"); }
	inline auto _impl_js_jDate::_tcjs_construct() noexcept {
		return _js_jDate(emscripten::val::global("Date").new_());
	}
	inline auto _impl_js_jDate::_tcjs_construct(tc::jst::union_t<double, tc::js::string> value) noexcept {
		return _js_jDate(emscripten::val::global("Date").new_(value));
	}
	inline auto _impl_js_jDate::_tcjs_construct(double year, double month, tc::jst::union_t<double, tc::js::undefined> date, tc::jst::union_t<double, tc::js::undefined> hours, tc::jst::union_t<double, tc::js::undefined> minutes, tc::jst::union_t<double, tc::js::undefined> seconds, tc::jst::union_t<double, tc::js::undefined> ms) noexcept {
		return _js_jDate(emscripten::val::global("Date").new_(year, month, date, hours, minutes, seconds, ms));
	}
	inline auto _impl_js_jDate::_tcjs_construct(tc::jst::union_t<_js_jDate, double, tc::js::string> value) noexcept {
		return _js_jDate(emscripten::val::global("Date").new_(value));
	}
	inline auto _impl_js_jDate::getDate() noexcept {
		return this->template _call<double>("getDate");
	}
	inline auto _impl_js_jDate::getDay() noexcept {
		return this->template _call<double>("getDay");
	}
	inline auto _impl_js_jDate::getFullYear() noexcept {
		return this->template _call<double>("getFullYear");
	}
	inline auto _impl_js_jDate::getHours() noexcept {
		return this->template _call<double>("getHours");
	}
	inline auto _impl_js_jDate::getMilliseconds() noexcept {
		return this->template _call<double>("getMilliseconds");
	}
	inline auto _impl_js_jDate::getMinutes() noexcept {
		return this->template _call<double>("getMinutes");
	}
	inline auto _impl_js_jDate::getMonth() noexcept {
		return this->template _call<double>("getMonth");
	}
	inline auto _impl_js_jDate::getSeconds() noexcept {
		return this->template _call<double>("getSeconds");
	}
	inline auto _impl_js_jDate::getTime() noexcept {
		return this->template _call<double>("getTime");
	}
	inline auto _impl_js_jDate::getTimezoneOffset() noexcept {
		return this->template _call<double>("getTimezoneOffset");
	}
	inline auto _impl_js_jDate::getUTCDate() noexcept {
		return this->template _call<double>("getUTCDate");
	}
	inline auto _impl_js_jDate::getUTCDay() noexcept {
		return this->template _call<double>("getUTCDay");
	}
	inline auto _impl_js_jDate::getUTCFullYear() noexcept {
		return this->template _call<double>("getUTCFullYear");
	}
	inline auto _impl_js_jDate::getUTCHours() noexcept {
		return this->template _call<double>("getUTCHours");
	}
	inline auto _impl_js_jDate::getUTCMilliseconds() noexcept {
		return this->template _call<double>("getUTCMilliseconds");
	}
	inline auto _impl_js_jDate::getUTCMinutes() noexcept {
		return this->template _call<double>("getUTCMinutes");
	}
	inline auto _impl_js_jDate::getUTCMonth() noexcept {
		return this->template _call<double>("getUTCMonth");
	}
	inline auto _impl_js_jDate::getUTCSeconds() noexcept {
		return this->template _call<double>("getUTCSeconds");
	}
	inline auto _impl_js_jDate::setDate(double date) noexcept {
		return this->template _call<double>("setDate", date);
	}
	inline auto _impl_js_jDate::setFullYear(double year, tc::jst::union_t<double, tc::js::undefined> month, tc::jst::union_t<double, tc::js::undefined> date) noexcept {
		return this->template _call<double>("setFullYear", year, month, date);
	}
	inline auto _impl_js_jDate::setHours(double hours, tc::jst::union_t<double, tc::js::undefined> min, tc::jst::union_t<double, tc::js::undefined> sec, tc::jst::union_t<double, tc::js::undefined> ms) noexcept {
		return this->template _call<double>("setHours", hours, min, sec, ms);
	}
	inline auto _impl_js_jDate::setMilliseconds(double ms) noexcept {
		return this->template _call<double>("setMilliseconds", ms);
	}
	inline auto _impl_js_jDate::setMinutes(double min, tc::jst::union_t<double, tc::js::undefined> sec, tc::jst::union_t<double, tc::js::undefined> ms) noexcept {
		return this->template _call<double>("setMinutes", min, sec, ms);
	}
	inline auto _impl_js_jDate::setMonth(double month, tc::jst::union_t<double, tc::js::undefined> date) noexcept {
		return this->template _call<double>("setMonth", month, date);
	}
	inline auto _impl_js_jDate::setSeconds(double sec, tc::jst::union_t<double, tc::js::undefined> ms) noexcept {
		return this->template _call<double>("setSeconds", sec, ms);
	}
	inline auto _impl_js_jDate::setTime(double time) noexcept {
		return this->template _call<double>("setTime", time);
	}
	inline auto _impl_js_jDate::setUTCDate(double date) noexcept {
		return this->template _call<double>("setUTCDate", date);
	}
	inline auto _impl_js_jDate::setUTCFullYear(double year, tc::jst::union_t<double, tc::js::undefined> month, tc::jst::union_t<double, tc::js::undefined> date) noexcept {
		return this->template _call<double>("setUTCFullYear", year, month, date);
	}
	inline auto _impl_js_jDate::setUTCHours(double hours, tc::jst::union_t<double, tc::js::undefined> min, tc::jst::union_t<double, tc::js::undefined> sec, tc::jst::union_t<double, tc::js::undefined> ms) noexcept {
		return this->template _call<double>("setUTCHours", hours, min, sec, ms);
	}
	inline auto _impl_js_jDate::setUTCMilliseconds(double ms) noexcept {
		return this->template _call<double>("setUTCMilliseconds", ms);
	}
	inline auto _impl_js_jDate::setUTCMinutes(double min, tc::jst::union_t<double, tc::js::undefined> sec, tc::jst::union_t<double, tc::js::undefined> ms) noexcept {
		return this->template _call<double>("setUTCMinutes", min, sec, ms);
	}
	inline auto _impl_js_jDate::setUTCMonth(double month, tc::jst::union_t<double, tc::js::undefined> date) noexcept {
		return this->template _call<double>("setUTCMonth", month, date);
	}
	inline auto _impl_js_jDate::setUTCSeconds(double sec, tc::jst::union_t<double, tc::js::undefined> ms) noexcept {
		return this->template _call<double>("setUTCSeconds", sec, ms);
	}
	inline auto _impl_js_jDate::toDateString() noexcept {
		return this->template _call<tc::js::string>("toDateString");
	}
	inline auto _impl_js_jDate::toISOString() noexcept {
		return this->template _call<tc::js::string>("toISOString");
	}
	inline auto _impl_js_jDate::toJSON(tc::js::any key) noexcept {
		return this->template _call<tc::js::string>("toJSON", key);
	}
	inline auto _impl_js_jDate::toLocaleDateString() noexcept {
		return this->template _call<tc::js::string>("toLocaleDateString");
	}
	inline auto _impl_js_jDate::toLocaleDateString(tc::jst::union_t<_js_jArray<tc::js::string>, tc::js::string, tc::js::undefined> locales, tc::jst::union_t<_js_jIntl_dDateTimeFormatOptions, tc::js::undefined> options) noexcept {
		return this->template _call<tc::js::string>("toLocaleDateString", locales, options);
	}
	inline auto _impl_js_jDate::toLocaleString() noexcept {
		return this->template _call<tc::js::string>("toLocaleString");
	}
	inline auto _impl_js_jDate::toLocaleString(tc::jst::union_t<_js_jArray<tc::js::string>, tc::js::string, tc::js::undefined> locales, tc::jst::union_t<_js_jIntl_dDateTimeFormatOptions, tc::js::undefined> options) noexcept {
		return this->template _call<tc::js::string>("toLocaleString", locales, options);
	}
	inline auto _impl_js_jDate::toLocaleTimeString() noexcept {
		return this->template _call<tc::js::string>("toLocaleTimeString");
	}
	inline auto _impl_js_jDate::toLocaleTimeString(tc::jst::union_t<_js_jArray<tc::js::string>, tc::js::string, tc::js::undefined> locales, tc::jst::union_t<_js_jIntl_dDateTimeFormatOptions, tc::js::undefined> options) noexcept {
		return this->template _call<tc::js::string>("toLocaleTimeString", locales, options);
	}
	inline auto _impl_js_jDate::toString() noexcept {
		return this->template _call<tc::js::string>("toString");
	}
	inline auto _impl_js_jDate::toTimeString() noexcept {
		return this->template _call<tc::js::string>("toTimeString");
	}
	inline auto _impl_js_jDate::toUTCString() noexcept {
		return this->template _call<tc::js::string>("toUTCString");
	}
	inline auto _impl_js_jDate::valueOf() noexcept {
		return this->template _call<double>("valueOf");
	}
	inline auto _impl_js_jDateConstructor::prototype() noexcept { return this->template _getProperty<_js_jDate>("prototype"); }
	inline auto _impl_js_jDateConstructor::UTC(double year, double month, tc::jst::union_t<double, tc::js::undefined> date, tc::jst::union_t<double, tc::js::undefined> hours, tc::jst::union_t<double, tc::js::undefined> minutes, tc::jst::union_t<double, tc::js::undefined> seconds, tc::jst::union_t<double, tc::js::undefined> ms) noexcept {
		return this->template _call<double>("UTC", year, month, date, hours, minutes, seconds, ms);
	}
	inline auto _impl_js_jDateConstructor::now() noexcept {
		return this->template _call<double>("now");
	}
	inline auto _impl_js_jDateConstructor::parse(tc::js::string s) noexcept {
		return this->template _call<double>("parse", s);
	}
	inline auto _impl_js_jError::name() noexcept { return this->template _getProperty<tc::js::string>("name"); }
	inline void _impl_js_jError::name(tc::js::string v) noexcept { this->template _setProperty("name", v); }
	inline auto _impl_js_jError::message() noexcept { return this->template _getProperty<tc::js::string>("message"); }
	inline void _impl_js_jError::message(tc::js::string v) noexcept { this->template _setProperty("message", v); }
	inline auto _impl_js_jError::stack() noexcept { return this->template _getProperty<tc::jst::union_t<tc::js::string, tc::js::undefined>>("stack"); }
	inline void _impl_js_jError::stack(tc::jst::union_t<tc::js::string, tc::js::undefined> v) noexcept { this->template _setProperty("stack", v); }
	inline auto _impl_js_jError::_tcjs_construct(tc::jst::union_t<tc::js::string, tc::js::undefined> message) noexcept {
		return _js_jError(emscripten::val::global("Error").new_(message));
	}
	inline auto _impl_js_jErrorConstructor::prototype() noexcept { return this->template _getProperty<_js_jError>("prototype"); }
	inline auto _impl_js_jErrorConstructor::prepareStackTrace() noexcept { return this->template _getProperty<tc::jst::union_t<tc::js::undefined, tc::jst::function<tc::js::any(_js_jError, _js_jArray<tc::js::any /*UnknownClassOrInterface=NodeJS.CallSite*/>)>>>("prepareStackTrace"); }
	inline void _impl_js_jErrorConstructor::prepareStackTrace(tc::jst::union_t<tc::js::undefined, tc::jst::function<tc::js::any(_js_jError, _js_jArray<tc::js::any /*UnknownClassOrInterface=NodeJS.CallSite*/>)>> v) noexcept { this->template _setProperty("prepareStackTrace", v); }
	inline auto _impl_js_jErrorConstructor::stackTraceLimit() noexcept { return this->template _getProperty<double>("stackTraceLimit"); }
	inline void _impl_js_jErrorConstructor::stackTraceLimit(double v) noexcept { this->template _setProperty("stackTraceLimit", v); }
	inline auto _impl_js_jErrorConstructor::captureStackTrace(tc::js::any /* unsupported type flags =67108864: object*/ targetObject, tc::jst::union_t<_js_jFunction, tc::js::undefined> constructorOpt) noexcept {
		 this->template _call<void>("captureStackTrace", targetObject, constructorOpt);
	}
	inline auto _impl_js_jEvalError::_tcjs_construct(tc::jst::union_t<tc::js::string, tc::js::undefined> message) noexcept {
		return _js_jEvalError(emscripten::val::global("EvalError").new_(message));
	}
	inline auto _impl_js_jEvalErrorConstructor::prototype() noexcept { return this->template _getProperty<_js_jEvalError>("prototype"); }
	inline auto _impl_js_jFloat32Array::BYTES_PER_ELEMENT() noexcept { return this->template _getProperty<double>("BYTES_PER_ELEMENT"); }
	inline auto _impl_js_jFloat32Array::buffer() noexcept { return this->template _getProperty<_js_jArrayBufferLike>("buffer"); }
	inline auto _impl_js_jFloat32Array::byteLength() noexcept { return this->template _getProperty<double>("byteLength"); }
	inline auto _impl_js_jFloat32Array::byteOffset() noexcept { return this->template _getProperty<double>("byteOffset"); }
	inline auto _impl_js_jFloat32Array::length() noexcept { return this->template _getProperty<double>("length"); }
	inline auto _impl_js_jFloat32Array::_tcjs_construct() noexcept {
		return _js_jFloat32Array(emscripten::val::global("Float32Array").new_());
	}
	inline auto _impl_js_jFloat32Array::_tcjs_construct(double length) noexcept {
		return _js_jFloat32Array(emscripten::val::global("Float32Array").new_(length));
	}
	inline auto _impl_js_jFloat32Array::_tcjs_construct(tc::js::any /*UnionWithUnsupportedSubtype=[_js_jArrayBuffer,tc::js::any,_js_jArrayLike<double>]*/ array) noexcept {
		return _js_jFloat32Array(emscripten::val::global("Float32Array").new_(array));
	}
	inline auto _impl_js_jFloat32Array::_tcjs_construct(_js_jArrayBufferLike buffer, tc::jst::union_t<double, tc::js::undefined> byteOffset, tc::jst::union_t<double, tc::js::undefined> length) noexcept {
		return _js_jFloat32Array(emscripten::val::global("Float32Array").new_(buffer, byteOffset, length));
	}
	inline auto _impl_js_jFloat32Array::_tcjs_construct(_js_jIterable<double> elements) noexcept {
		return _js_jFloat32Array(emscripten::val::global("Float32Array").new_(elements));
	}
	inline auto _impl_js_jFloat32Array::copyWithin(double target, double start, tc::jst::union_t<double, tc::js::undefined> end) noexcept {
		return this->template _call<_js_jFloat32Array>("copyWithin", target, start, end);
	}
	inline auto _impl_js_jFloat32Array::entries() noexcept {
		return this->template _call<_js_jIterableIterator<std::tuple<double, double>>>("entries");
	}
	inline auto _impl_js_jFloat32Array::every(tc::jst::function<tc::js::any(double, double, _js_jFloat32Array)> predicate, tc::js::any thisArg) noexcept {
		return this->template _call<bool /*false*/>("every", predicate, thisArg);
	}
	inline auto _impl_js_jFloat32Array::fill(double value, tc::jst::union_t<double, tc::js::undefined> start, tc::jst::union_t<double, tc::js::undefined> end) noexcept {
		return this->template _call<_js_jFloat32Array>("fill", value, start, end);
	}
	inline auto _impl_js_jFloat32Array::filter(tc::jst::function<tc::js::any(double, double, _js_jFloat32Array)> predicate, tc::js::any thisArg) noexcept {
		return this->template _call<_js_jFloat32Array>("filter", predicate, thisArg);
	}
	inline auto _impl_js_jFloat32Array::find(tc::jst::function<bool /*false*/(double, double, _js_jFloat32Array)> predicate, tc::js::any thisArg) noexcept {
		return this->template _call<tc::jst::union_t<double, tc::js::undefined>>("find", predicate, thisArg);
	}
	inline auto _impl_js_jFloat32Array::findIndex(tc::jst::function<bool /*false*/(double, double, _js_jFloat32Array)> predicate, tc::js::any thisArg) noexcept {
		return this->template _call<double>("findIndex", predicate, thisArg);
	}
	inline auto _impl_js_jFloat32Array::forEach(tc::jst::function<void(double, double, _js_jFloat32Array)> callbackfn, tc::js::any thisArg) noexcept {
		 this->template _call<void>("forEach", callbackfn, thisArg);
	}
	inline auto _impl_js_jFloat32Array::includes(double searchElement, tc::jst::union_t<double, tc::js::undefined> fromIndex) noexcept {
		return this->template _call<bool /*false*/>("includes", searchElement, fromIndex);
	}
	inline auto _impl_js_jFloat32Array::indexOf(double searchElement, tc::jst::union_t<double, tc::js::undefined> fromIndex) noexcept {
		return this->template _call<double>("indexOf", searchElement, fromIndex);
	}
	inline auto _impl_js_jFloat32Array::join(tc::jst::union_t<tc::js::string, tc::js::undefined> separator) noexcept {
		return this->template _call<tc::js::string>("join", separator);
	}
	inline auto _impl_js_jFloat32Array::keys() noexcept {
		return this->template _call<_js_jIterableIterator<double>>("keys");
	}
	inline auto _impl_js_jFloat32Array::lastIndexOf(double searchElement, tc::jst::union_t<double, tc::js::undefined> fromIndex) noexcept {
		return this->template _call<double>("lastIndexOf", searchElement, fromIndex);
	}
	inline auto _impl_js_jFloat32Array::map(tc::jst::function<double(double, double, _js_jFloat32Array)> callbackfn, tc::js::any thisArg) noexcept {
		return this->template _call<_js_jFloat32Array>("map", callbackfn, thisArg);
	}
	inline auto _impl_js_jFloat32Array::operator[](double index) noexcept {
		return this->template _getProperty<double>(index);
	}
	template<typename U>
	inline auto _impl_js_jFloat32Array::reduce(tc::jst::function<U(U, double, double, _js_jFloat32Array)> callbackfn, U initialValue) noexcept {
		return this->template _call<U>("reduce", callbackfn, initialValue);
	}
	inline auto _impl_js_jFloat32Array::reduce(tc::jst::function<double(double, double, double, _js_jFloat32Array)> callbackfn) noexcept {
		return this->template _call<double>("reduce", callbackfn);
	}
	inline auto _impl_js_jFloat32Array::reduce(tc::jst::function<double(double, double, double, _js_jFloat32Array)> callbackfn, double initialValue) noexcept {
		return this->template _call<double>("reduce", callbackfn, initialValue);
	}
	template<typename U>
	inline auto _impl_js_jFloat32Array::reduceRight(tc::jst::function<U(U, double, double, _js_jFloat32Array)> callbackfn, U initialValue) noexcept {
		return this->template _call<U>("reduceRight", callbackfn, initialValue);
	}
	inline auto _impl_js_jFloat32Array::reduceRight(tc::jst::function<double(double, double, double, _js_jFloat32Array)> callbackfn) noexcept {
		return this->template _call<double>("reduceRight", callbackfn);
	}
	inline auto _impl_js_jFloat32Array::reduceRight(tc::jst::function<double(double, double, double, _js_jFloat32Array)> callbackfn, double initialValue) noexcept {
		return this->template _call<double>("reduceRight", callbackfn, initialValue);
	}
	inline auto _impl_js_jFloat32Array::reverse() noexcept {
		return this->template _call<_js_jFloat32Array>("reverse");
	}
	inline auto _impl_js_jFloat32Array::set(_js_jArrayLike<double> array, tc::jst::union_t<double, tc::js::undefined> offset) noexcept {
		 this->template _call<void>("set", array, offset);
	}
	inline auto _impl_js_jFloat32Array::slice(tc::jst::union_t<double, tc::js::undefined> start, tc::jst::union_t<double, tc::js::undefined> end) noexcept {
		return this->template _call<_js_jFloat32Array>("slice", start, end);
	}
	inline auto _impl_js_jFloat32Array::some(tc::jst::function<tc::js::any(double, double, _js_jFloat32Array)> predicate, tc::js::any thisArg) noexcept {
		return this->template _call<bool /*false*/>("some", predicate, thisArg);
	}
	inline auto _impl_js_jFloat32Array::sort(tc::jst::union_t<tc::js::undefined, tc::jst::function<double(double, double)>> compareFn) noexcept {
		return this->template _call<_js_jFloat32Array>("sort", compareFn);
	}
	inline auto _impl_js_jFloat32Array::subarray(tc::jst::union_t<double, tc::js::undefined> begin, tc::jst::union_t<double, tc::js::undefined> end) noexcept {
		return this->template _call<_js_jFloat32Array>("subarray", begin, end);
	}
	inline auto _impl_js_jFloat32Array::toLocaleString() noexcept {
		return this->template _call<tc::js::string>("toLocaleString");
	}
	inline auto _impl_js_jFloat32Array::toString() noexcept {
		return this->template _call<tc::js::string>("toString");
	}
	inline auto _impl_js_jFloat32Array::valueOf() noexcept {
		return this->template _call<_js_jFloat32Array>("valueOf");
	}
	inline auto _impl_js_jFloat32Array::values() noexcept {
		return this->template _call<_js_jIterableIterator<double>>("values");
	}
	inline auto _impl_js_jFloat32ArrayConstructor::prototype() noexcept { return this->template _getProperty<_js_jFloat32Array>("prototype"); }
	inline auto _impl_js_jFloat32ArrayConstructor::BYTES_PER_ELEMENT() noexcept { return this->template _getProperty<double>("BYTES_PER_ELEMENT"); }
	template<typename T>
	inline auto _impl_js_jFloat32ArrayConstructor::from(_js_jArrayLike<T> arrayLike, tc::jst::function<double(T, double)> mapfn, tc::js::any thisArg) noexcept {
		return this->template _call<_js_jFloat32Array>("from", arrayLike, mapfn, thisArg);
	}
	inline auto _impl_js_jFloat32ArrayConstructor::from(_js_jArrayLike<double> arrayLike) noexcept {
		return this->template _call<_js_jFloat32Array>("from", arrayLike);
	}
	inline auto _impl_js_jFloat32ArrayConstructor::from(_js_jIterable<double> arrayLike, tc::jst::union_t<tc::js::undefined, tc::jst::function<double(double, double)>> mapfn, tc::js::any thisArg) noexcept {
		return this->template _call<_js_jFloat32Array>("from", arrayLike, mapfn, thisArg);
	}
	inline auto _impl_js_jFloat32ArrayConstructor::of(tc::jst::variadic_arg_t, double items) noexcept {
		return this->template _call<_js_jFloat32Array>("of", items);
	}
	inline auto _impl_js_jFloat64Array::BYTES_PER_ELEMENT() noexcept { return this->template _getProperty<double>("BYTES_PER_ELEMENT"); }
	inline auto _impl_js_jFloat64Array::buffer() noexcept { return this->template _getProperty<_js_jArrayBufferLike>("buffer"); }
	inline auto _impl_js_jFloat64Array::byteLength() noexcept { return this->template _getProperty<double>("byteLength"); }
	inline auto _impl_js_jFloat64Array::byteOffset() noexcept { return this->template _getProperty<double>("byteOffset"); }
	inline auto _impl_js_jFloat64Array::length() noexcept { return this->template _getProperty<double>("length"); }
	inline auto _impl_js_jFloat64Array::_tcjs_construct() noexcept {
		return _js_jFloat64Array(emscripten::val::global("Float64Array").new_());
	}
	inline auto _impl_js_jFloat64Array::_tcjs_construct(double length) noexcept {
		return _js_jFloat64Array(emscripten::val::global("Float64Array").new_(length));
	}
	inline auto _impl_js_jFloat64Array::_tcjs_construct(tc::js::any /*UnionWithUnsupportedSubtype=[_js_jArrayBuffer,tc::js::any,_js_jArrayLike<double>]*/ array) noexcept {
		return _js_jFloat64Array(emscripten::val::global("Float64Array").new_(array));
	}
	inline auto _impl_js_jFloat64Array::_tcjs_construct(_js_jArrayBufferLike buffer, tc::jst::union_t<double, tc::js::undefined> byteOffset, tc::jst::union_t<double, tc::js::undefined> length) noexcept {
		return _js_jFloat64Array(emscripten::val::global("Float64Array").new_(buffer, byteOffset, length));
	}
	inline auto _impl_js_jFloat64Array::_tcjs_construct(_js_jIterable<double> elements) noexcept {
		return _js_jFloat64Array(emscripten::val::global("Float64Array").new_(elements));
	}
	inline auto _impl_js_jFloat64Array::copyWithin(double target, double start, tc::jst::union_t<double, tc::js::undefined> end) noexcept {
		return this->template _call<_js_jFloat64Array>("copyWithin", target, start, end);
	}
	inline auto _impl_js_jFloat64Array::entries() noexcept {
		return this->template _call<_js_jIterableIterator<std::tuple<double, double>>>("entries");
	}
	inline auto _impl_js_jFloat64Array::every(tc::jst::function<tc::js::any(double, double, _js_jFloat64Array)> predicate, tc::js::any thisArg) noexcept {
		return this->template _call<bool /*false*/>("every", predicate, thisArg);
	}
	inline auto _impl_js_jFloat64Array::fill(double value, tc::jst::union_t<double, tc::js::undefined> start, tc::jst::union_t<double, tc::js::undefined> end) noexcept {
		return this->template _call<_js_jFloat64Array>("fill", value, start, end);
	}
	inline auto _impl_js_jFloat64Array::filter(tc::jst::function<tc::js::any(double, double, _js_jFloat64Array)> predicate, tc::js::any thisArg) noexcept {
		return this->template _call<_js_jFloat64Array>("filter", predicate, thisArg);
	}
	inline auto _impl_js_jFloat64Array::find(tc::jst::function<bool /*false*/(double, double, _js_jFloat64Array)> predicate, tc::js::any thisArg) noexcept {
		return this->template _call<tc::jst::union_t<double, tc::js::undefined>>("find", predicate, thisArg);
	}
	inline auto _impl_js_jFloat64Array::findIndex(tc::jst::function<bool /*false*/(double, double, _js_jFloat64Array)> predicate, tc::js::any thisArg) noexcept {
		return this->template _call<double>("findIndex", predicate, thisArg);
	}
	inline auto _impl_js_jFloat64Array::forEach(tc::jst::function<void(double, double, _js_jFloat64Array)> callbackfn, tc::js::any thisArg) noexcept {
		 this->template _call<void>("forEach", callbackfn, thisArg);
	}
	inline auto _impl_js_jFloat64Array::includes(double searchElement, tc::jst::union_t<double, tc::js::undefined> fromIndex) noexcept {
		return this->template _call<bool /*false*/>("includes", searchElement, fromIndex);
	}
	inline auto _impl_js_jFloat64Array::indexOf(double searchElement, tc::jst::union_t<double, tc::js::undefined> fromIndex) noexcept {
		return this->template _call<double>("indexOf", searchElement, fromIndex);
	}
	inline auto _impl_js_jFloat64Array::join(tc::jst::union_t<tc::js::string, tc::js::undefined> separator) noexcept {
		return this->template _call<tc::js::string>("join", separator);
	}
	inline auto _impl_js_jFloat64Array::keys() noexcept {
		return this->template _call<_js_jIterableIterator<double>>("keys");
	}
	inline auto _impl_js_jFloat64Array::lastIndexOf(double searchElement, tc::jst::union_t<double, tc::js::undefined> fromIndex) noexcept {
		return this->template _call<double>("lastIndexOf", searchElement, fromIndex);
	}
	inline auto _impl_js_jFloat64Array::map(tc::jst::function<double(double, double, _js_jFloat64Array)> callbackfn, tc::js::any thisArg) noexcept {
		return this->template _call<_js_jFloat64Array>("map", callbackfn, thisArg);
	}
	inline auto _impl_js_jFloat64Array::operator[](double index) noexcept {
		return this->template _getProperty<double>(index);
	}
	template<typename U>
	inline auto _impl_js_jFloat64Array::reduce(tc::jst::function<U(U, double, double, _js_jFloat64Array)> callbackfn, U initialValue) noexcept {
		return this->template _call<U>("reduce", callbackfn, initialValue);
	}
	inline auto _impl_js_jFloat64Array::reduce(tc::jst::function<double(double, double, double, _js_jFloat64Array)> callbackfn) noexcept {
		return this->template _call<double>("reduce", callbackfn);
	}
	inline auto _impl_js_jFloat64Array::reduce(tc::jst::function<double(double, double, double, _js_jFloat64Array)> callbackfn, double initialValue) noexcept {
		return this->template _call<double>("reduce", callbackfn, initialValue);
	}
	template<typename U>
	inline auto _impl_js_jFloat64Array::reduceRight(tc::jst::function<U(U, double, double, _js_jFloat64Array)> callbackfn, U initialValue) noexcept {
		return this->template _call<U>("reduceRight", callbackfn, initialValue);
	}
	inline auto _impl_js_jFloat64Array::reduceRight(tc::jst::function<double(double, double, double, _js_jFloat64Array)> callbackfn) noexcept {
		return this->template _call<double>("reduceRight", callbackfn);
	}
	inline auto _impl_js_jFloat64Array::reduceRight(tc::jst::function<double(double, double, double, _js_jFloat64Array)> callbackfn, double initialValue) noexcept {
		return this->template _call<double>("reduceRight", callbackfn, initialValue);
	}
	inline auto _impl_js_jFloat64Array::reverse() noexcept {
		return this->template _call<_js_jFloat64Array>("reverse");
	}
	inline auto _impl_js_jFloat64Array::set(_js_jArrayLike<double> array, tc::jst::union_t<double, tc::js::undefined> offset) noexcept {
		 this->template _call<void>("set", array, offset);
	}
	inline auto _impl_js_jFloat64Array::slice(tc::jst::union_t<double, tc::js::undefined> start, tc::jst::union_t<double, tc::js::undefined> end) noexcept {
		return this->template _call<_js_jFloat64Array>("slice", start, end);
	}
	inline auto _impl_js_jFloat64Array::some(tc::jst::function<tc::js::any(double, double, _js_jFloat64Array)> predicate, tc::js::any thisArg) noexcept {
		return this->template _call<bool /*false*/>("some", predicate, thisArg);
	}
	inline auto _impl_js_jFloat64Array::sort(tc::jst::union_t<tc::js::undefined, tc::jst::function<double(double, double)>> compareFn) noexcept {
		return this->template _call<_js_jFloat64Array>("sort", compareFn);
	}
	inline auto _impl_js_jFloat64Array::subarray(tc::jst::union_t<double, tc::js::undefined> begin, tc::jst::union_t<double, tc::js::undefined> end) noexcept {
		return this->template _call<_js_jFloat64Array>("subarray", begin, end);
	}
	inline auto _impl_js_jFloat64Array::toString() noexcept {
		return this->template _call<tc::js::string>("toString");
	}
	inline auto _impl_js_jFloat64Array::valueOf() noexcept {
		return this->template _call<_js_jFloat64Array>("valueOf");
	}
	inline auto _impl_js_jFloat64Array::values() noexcept {
		return this->template _call<_js_jIterableIterator<double>>("values");
	}
	inline auto _impl_js_jFloat64ArrayConstructor::prototype() noexcept { return this->template _getProperty<_js_jFloat64Array>("prototype"); }
	inline auto _impl_js_jFloat64ArrayConstructor::BYTES_PER_ELEMENT() noexcept { return this->template _getProperty<double>("BYTES_PER_ELEMENT"); }
	template<typename T>
	inline auto _impl_js_jFloat64ArrayConstructor::from(_js_jArrayLike<T> arrayLike, tc::jst::function<double(T, double)> mapfn, tc::js::any thisArg) noexcept {
		return this->template _call<_js_jFloat64Array>("from", arrayLike, mapfn, thisArg);
	}
	inline auto _impl_js_jFloat64ArrayConstructor::from(_js_jArrayLike<double> arrayLike) noexcept {
		return this->template _call<_js_jFloat64Array>("from", arrayLike);
	}
	inline auto _impl_js_jFloat64ArrayConstructor::from(_js_jIterable<double> arrayLike, tc::jst::union_t<tc::js::undefined, tc::jst::function<double(double, double)>> mapfn, tc::js::any thisArg) noexcept {
		return this->template _call<_js_jFloat64Array>("from", arrayLike, mapfn, thisArg);
	}
	inline auto _impl_js_jFloat64ArrayConstructor::of(tc::jst::variadic_arg_t, double items) noexcept {
		return this->template _call<_js_jFloat64Array>("of", items);
	}
	inline auto _impl_js_jFunctionConstructor::prototype() noexcept { return this->template _getProperty<_js_jFunction>("prototype"); }
	template<typename T, typename TReturn, typename TNext>
	inline auto _impl_js_jIterator<T, TReturn, TNext>::next(tc::jst::union_t<std::tuple<>, std::tuple<TNext>> args) noexcept {
		return this->template _call<_js_jIteratorResult<T, TReturn>>("next", args);
	}
	template<typename T, typename TReturn, typename TNext>
	inline auto _impl_js_jGenerator<T, TReturn, TNext>::next(tc::jst::union_t<std::tuple<>, std::tuple<TNext>> args) noexcept {
		return this->template _call<_js_jIteratorResult<T, TReturn>>("next", args);
	}
	template<typename T, typename TReturn, typename TNext>
	inline auto _impl_js_jGenerator<T, TReturn, TNext>::return_(TReturn value) noexcept {
		return this->template _call<_js_jIteratorResult<T, TReturn>>("return", value);
	}
	template<typename T, typename TReturn, typename TNext>
	inline auto _impl_js_jGenerator<T, TReturn, TNext>::throw_(tc::js::any e) noexcept {
		return this->template _call<_js_jIteratorResult<T, TReturn>>("throw", e);
	}
	inline auto _impl_js_jGeneratorFunction::length() noexcept { return this->template _getProperty<double>("length"); }
	inline auto _impl_js_jGeneratorFunction::name() noexcept { return this->template _getProperty<tc::js::string>("name"); }
	inline auto _impl_js_jGeneratorFunction::prototype() noexcept { return this->template _getProperty<_js_jGenerator<tc::js::any, tc::js::any, tc::js::any>>("prototype"); }
	inline auto _impl_js_jGeneratorFunctionConstructor::length() noexcept { return this->template _getProperty<double>("length"); }
	inline auto _impl_js_jGeneratorFunctionConstructor::name() noexcept { return this->template _getProperty<tc::js::string>("name"); }
	inline auto _impl_js_jGeneratorFunctionConstructor::prototype() noexcept { return this->template _getProperty<_js_jGeneratorFunction>("prototype"); }
	inline auto _impl_js_jIArguments::length() noexcept { return this->template _getProperty<double>("length"); }
	inline void _impl_js_jIArguments::length(double v) noexcept { this->template _setProperty("length", v); }
	inline auto _impl_js_jIArguments::callee() noexcept { return this->template _getProperty<_js_jFunction>("callee"); }
	inline void _impl_js_jIArguments::callee(_js_jFunction v) noexcept { this->template _setProperty("callee", v); }
	inline auto _impl_js_jIArguments::operator[](double index) noexcept {
		return this->template _getProperty<tc::js::any>(index);
	}
	inline auto _impl_js_jImportMeta::url() noexcept { return this->template _getProperty<tc::js::string>("url"); }
	inline void _impl_js_jImportMeta::url(tc::js::string v) noexcept { this->template _setProperty("url", v); }
	inline auto _impl_js_jInt16Array::BYTES_PER_ELEMENT() noexcept { return this->template _getProperty<double>("BYTES_PER_ELEMENT"); }
	inline auto _impl_js_jInt16Array::buffer() noexcept { return this->template _getProperty<_js_jArrayBufferLike>("buffer"); }
	inline auto _impl_js_jInt16Array::byteLength() noexcept { return this->template _getProperty<double>("byteLength"); }
	inline auto _impl_js_jInt16Array::byteOffset() noexcept { return this->template _getProperty<double>("byteOffset"); }
	inline auto _impl_js_jInt16Array::length() noexcept { return this->template _getProperty<double>("length"); }
	inline auto _impl_js_jInt16Array::_tcjs_construct() noexcept {
		return _js_jInt16Array(emscripten::val::global("Int16Array").new_());
	}
	inline auto _impl_js_jInt16Array::_tcjs_construct(double length) noexcept {
		return _js_jInt16Array(emscripten::val::global("Int16Array").new_(length));
	}
	inline auto _impl_js_jInt16Array::_tcjs_construct(tc::js::any /*UnionWithUnsupportedSubtype=[_js_jArrayBuffer,tc::js::any,_js_jArrayLike<double>]*/ array) noexcept {
		return _js_jInt16Array(emscripten::val::global("Int16Array").new_(array));
	}
	inline auto _impl_js_jInt16Array::_tcjs_construct(_js_jArrayBufferLike buffer, tc::jst::union_t<double, tc::js::undefined> byteOffset, tc::jst::union_t<double, tc::js::undefined> length) noexcept {
		return _js_jInt16Array(emscripten::val::global("Int16Array").new_(buffer, byteOffset, length));
	}
	inline auto _impl_js_jInt16Array::_tcjs_construct(_js_jIterable<double> elements) noexcept {
		return _js_jInt16Array(emscripten::val::global("Int16Array").new_(elements));
	}
	inline auto _impl_js_jInt16Array::copyWithin(double target, double start, tc::jst::union_t<double, tc::js::undefined> end) noexcept {
		return this->template _call<_js_jInt16Array>("copyWithin", target, start, end);
	}
	inline auto _impl_js_jInt16Array::entries() noexcept {
		return this->template _call<_js_jIterableIterator<std::tuple<double, double>>>("entries");
	}
	inline auto _impl_js_jInt16Array::every(tc::jst::function<tc::js::any(double, double, _js_jInt16Array)> predicate, tc::js::any thisArg) noexcept {
		return this->template _call<bool /*false*/>("every", predicate, thisArg);
	}
	inline auto _impl_js_jInt16Array::fill(double value, tc::jst::union_t<double, tc::js::undefined> start, tc::jst::union_t<double, tc::js::undefined> end) noexcept {
		return this->template _call<_js_jInt16Array>("fill", value, start, end);
	}
	inline auto _impl_js_jInt16Array::filter(tc::jst::function<tc::js::any(double, double, _js_jInt16Array)> predicate, tc::js::any thisArg) noexcept {
		return this->template _call<_js_jInt16Array>("filter", predicate, thisArg);
	}
	inline auto _impl_js_jInt16Array::find(tc::jst::function<bool /*false*/(double, double, _js_jInt16Array)> predicate, tc::js::any thisArg) noexcept {
		return this->template _call<tc::jst::union_t<double, tc::js::undefined>>("find", predicate, thisArg);
	}
	inline auto _impl_js_jInt16Array::findIndex(tc::jst::function<bool /*false*/(double, double, _js_jInt16Array)> predicate, tc::js::any thisArg) noexcept {
		return this->template _call<double>("findIndex", predicate, thisArg);
	}
	inline auto _impl_js_jInt16Array::forEach(tc::jst::function<void(double, double, _js_jInt16Array)> callbackfn, tc::js::any thisArg) noexcept {
		 this->template _call<void>("forEach", callbackfn, thisArg);
	}
	inline auto _impl_js_jInt16Array::includes(double searchElement, tc::jst::union_t<double, tc::js::undefined> fromIndex) noexcept {
		return this->template _call<bool /*false*/>("includes", searchElement, fromIndex);
	}
	inline auto _impl_js_jInt16Array::indexOf(double searchElement, tc::jst::union_t<double, tc::js::undefined> fromIndex) noexcept {
		return this->template _call<double>("indexOf", searchElement, fromIndex);
	}
	inline auto _impl_js_jInt16Array::join(tc::jst::union_t<tc::js::string, tc::js::undefined> separator) noexcept {
		return this->template _call<tc::js::string>("join", separator);
	}
	inline auto _impl_js_jInt16Array::keys() noexcept {
		return this->template _call<_js_jIterableIterator<double>>("keys");
	}
	inline auto _impl_js_jInt16Array::lastIndexOf(double searchElement, tc::jst::union_t<double, tc::js::undefined> fromIndex) noexcept {
		return this->template _call<double>("lastIndexOf", searchElement, fromIndex);
	}
	inline auto _impl_js_jInt16Array::map(tc::jst::function<double(double, double, _js_jInt16Array)> callbackfn, tc::js::any thisArg) noexcept {
		return this->template _call<_js_jInt16Array>("map", callbackfn, thisArg);
	}
	inline auto _impl_js_jInt16Array::operator[](double index) noexcept {
		return this->template _getProperty<double>(index);
	}
	template<typename U>
	inline auto _impl_js_jInt16Array::reduce(tc::jst::function<U(U, double, double, _js_jInt16Array)> callbackfn, U initialValue) noexcept {
		return this->template _call<U>("reduce", callbackfn, initialValue);
	}
	inline auto _impl_js_jInt16Array::reduce(tc::jst::function<double(double, double, double, _js_jInt16Array)> callbackfn) noexcept {
		return this->template _call<double>("reduce", callbackfn);
	}
	inline auto _impl_js_jInt16Array::reduce(tc::jst::function<double(double, double, double, _js_jInt16Array)> callbackfn, double initialValue) noexcept {
		return this->template _call<double>("reduce", callbackfn, initialValue);
	}
	template<typename U>
	inline auto _impl_js_jInt16Array::reduceRight(tc::jst::function<U(U, double, double, _js_jInt16Array)> callbackfn, U initialValue) noexcept {
		return this->template _call<U>("reduceRight", callbackfn, initialValue);
	}
	inline auto _impl_js_jInt16Array::reduceRight(tc::jst::function<double(double, double, double, _js_jInt16Array)> callbackfn) noexcept {
		return this->template _call<double>("reduceRight", callbackfn);
	}
	inline auto _impl_js_jInt16Array::reduceRight(tc::jst::function<double(double, double, double, _js_jInt16Array)> callbackfn, double initialValue) noexcept {
		return this->template _call<double>("reduceRight", callbackfn, initialValue);
	}
	inline auto _impl_js_jInt16Array::reverse() noexcept {
		return this->template _call<_js_jInt16Array>("reverse");
	}
	inline auto _impl_js_jInt16Array::set(_js_jArrayLike<double> array, tc::jst::union_t<double, tc::js::undefined> offset) noexcept {
		 this->template _call<void>("set", array, offset);
	}
	inline auto _impl_js_jInt16Array::slice(tc::jst::union_t<double, tc::js::undefined> start, tc::jst::union_t<double, tc::js::undefined> end) noexcept {
		return this->template _call<_js_jInt16Array>("slice", start, end);
	}
	inline auto _impl_js_jInt16Array::some(tc::jst::function<tc::js::any(double, double, _js_jInt16Array)> predicate, tc::js::any thisArg) noexcept {
		return this->template _call<bool /*false*/>("some", predicate, thisArg);
	}
	inline auto _impl_js_jInt16Array::sort(tc::jst::union_t<tc::js::undefined, tc::jst::function<double(double, double)>> compareFn) noexcept {
		return this->template _call<_js_jInt16Array>("sort", compareFn);
	}
	inline auto _impl_js_jInt16Array::subarray(tc::jst::union_t<double, tc::js::undefined> begin, tc::jst::union_t<double, tc::js::undefined> end) noexcept {
		return this->template _call<_js_jInt16Array>("subarray", begin, end);
	}
	inline auto _impl_js_jInt16Array::toLocaleString() noexcept {
		return this->template _call<tc::js::string>("toLocaleString");
	}
	inline auto _impl_js_jInt16Array::toString() noexcept {
		return this->template _call<tc::js::string>("toString");
	}
	inline auto _impl_js_jInt16Array::valueOf() noexcept {
		return this->template _call<_js_jInt16Array>("valueOf");
	}
	inline auto _impl_js_jInt16Array::values() noexcept {
		return this->template _call<_js_jIterableIterator<double>>("values");
	}
	inline auto _impl_js_jInt16ArrayConstructor::prototype() noexcept { return this->template _getProperty<_js_jInt16Array>("prototype"); }
	inline auto _impl_js_jInt16ArrayConstructor::BYTES_PER_ELEMENT() noexcept { return this->template _getProperty<double>("BYTES_PER_ELEMENT"); }
	template<typename T>
	inline auto _impl_js_jInt16ArrayConstructor::from(_js_jArrayLike<T> arrayLike, tc::jst::function<double(T, double)> mapfn, tc::js::any thisArg) noexcept {
		return this->template _call<_js_jInt16Array>("from", arrayLike, mapfn, thisArg);
	}
	inline auto _impl_js_jInt16ArrayConstructor::from(_js_jArrayLike<double> arrayLike) noexcept {
		return this->template _call<_js_jInt16Array>("from", arrayLike);
	}
	inline auto _impl_js_jInt16ArrayConstructor::from(_js_jIterable<double> arrayLike, tc::jst::union_t<tc::js::undefined, tc::jst::function<double(double, double)>> mapfn, tc::js::any thisArg) noexcept {
		return this->template _call<_js_jInt16Array>("from", arrayLike, mapfn, thisArg);
	}
	inline auto _impl_js_jInt16ArrayConstructor::of(tc::jst::variadic_arg_t, double items) noexcept {
		return this->template _call<_js_jInt16Array>("of", items);
	}
	inline auto _impl_js_jInt32Array::BYTES_PER_ELEMENT() noexcept { return this->template _getProperty<double>("BYTES_PER_ELEMENT"); }
	inline auto _impl_js_jInt32Array::buffer() noexcept { return this->template _getProperty<_js_jArrayBufferLike>("buffer"); }
	inline auto _impl_js_jInt32Array::byteLength() noexcept { return this->template _getProperty<double>("byteLength"); }
	inline auto _impl_js_jInt32Array::byteOffset() noexcept { return this->template _getProperty<double>("byteOffset"); }
	inline auto _impl_js_jInt32Array::length() noexcept { return this->template _getProperty<double>("length"); }
	inline auto _impl_js_jInt32Array::_tcjs_construct() noexcept {
		return _js_jInt32Array(emscripten::val::global("Int32Array").new_());
	}
	inline auto _impl_js_jInt32Array::_tcjs_construct(double length) noexcept {
		return _js_jInt32Array(emscripten::val::global("Int32Array").new_(length));
	}
	inline auto _impl_js_jInt32Array::_tcjs_construct(tc::js::any /*UnionWithUnsupportedSubtype=[_js_jArrayBuffer,tc::js::any,_js_jArrayLike<double>]*/ array) noexcept {
		return _js_jInt32Array(emscripten::val::global("Int32Array").new_(array));
	}
	inline auto _impl_js_jInt32Array::_tcjs_construct(_js_jArrayBufferLike buffer, tc::jst::union_t<double, tc::js::undefined> byteOffset, tc::jst::union_t<double, tc::js::undefined> length) noexcept {
		return _js_jInt32Array(emscripten::val::global("Int32Array").new_(buffer, byteOffset, length));
	}
	inline auto _impl_js_jInt32Array::_tcjs_construct(_js_jIterable<double> elements) noexcept {
		return _js_jInt32Array(emscripten::val::global("Int32Array").new_(elements));
	}
	inline auto _impl_js_jInt32Array::copyWithin(double target, double start, tc::jst::union_t<double, tc::js::undefined> end) noexcept {
		return this->template _call<_js_jInt32Array>("copyWithin", target, start, end);
	}
	inline auto _impl_js_jInt32Array::entries() noexcept {
		return this->template _call<_js_jIterableIterator<std::tuple<double, double>>>("entries");
	}
	inline auto _impl_js_jInt32Array::every(tc::jst::function<tc::js::any(double, double, _js_jInt32Array)> predicate, tc::js::any thisArg) noexcept {
		return this->template _call<bool /*false*/>("every", predicate, thisArg);
	}
	inline auto _impl_js_jInt32Array::fill(double value, tc::jst::union_t<double, tc::js::undefined> start, tc::jst::union_t<double, tc::js::undefined> end) noexcept {
		return this->template _call<_js_jInt32Array>("fill", value, start, end);
	}
	inline auto _impl_js_jInt32Array::filter(tc::jst::function<tc::js::any(double, double, _js_jInt32Array)> predicate, tc::js::any thisArg) noexcept {
		return this->template _call<_js_jInt32Array>("filter", predicate, thisArg);
	}
	inline auto _impl_js_jInt32Array::find(tc::jst::function<bool /*false*/(double, double, _js_jInt32Array)> predicate, tc::js::any thisArg) noexcept {
		return this->template _call<tc::jst::union_t<double, tc::js::undefined>>("find", predicate, thisArg);
	}
	inline auto _impl_js_jInt32Array::findIndex(tc::jst::function<bool /*false*/(double, double, _js_jInt32Array)> predicate, tc::js::any thisArg) noexcept {
		return this->template _call<double>("findIndex", predicate, thisArg);
	}
	inline auto _impl_js_jInt32Array::forEach(tc::jst::function<void(double, double, _js_jInt32Array)> callbackfn, tc::js::any thisArg) noexcept {
		 this->template _call<void>("forEach", callbackfn, thisArg);
	}
	inline auto _impl_js_jInt32Array::includes(double searchElement, tc::jst::union_t<double, tc::js::undefined> fromIndex) noexcept {
		return this->template _call<bool /*false*/>("includes", searchElement, fromIndex);
	}
	inline auto _impl_js_jInt32Array::indexOf(double searchElement, tc::jst::union_t<double, tc::js::undefined> fromIndex) noexcept {
		return this->template _call<double>("indexOf", searchElement, fromIndex);
	}
	inline auto _impl_js_jInt32Array::join(tc::jst::union_t<tc::js::string, tc::js::undefined> separator) noexcept {
		return this->template _call<tc::js::string>("join", separator);
	}
	inline auto _impl_js_jInt32Array::keys() noexcept {
		return this->template _call<_js_jIterableIterator<double>>("keys");
	}
	inline auto _impl_js_jInt32Array::lastIndexOf(double searchElement, tc::jst::union_t<double, tc::js::undefined> fromIndex) noexcept {
		return this->template _call<double>("lastIndexOf", searchElement, fromIndex);
	}
	inline auto _impl_js_jInt32Array::map(tc::jst::function<double(double, double, _js_jInt32Array)> callbackfn, tc::js::any thisArg) noexcept {
		return this->template _call<_js_jInt32Array>("map", callbackfn, thisArg);
	}
	inline auto _impl_js_jInt32Array::operator[](double index) noexcept {
		return this->template _getProperty<double>(index);
	}
	template<typename U>
	inline auto _impl_js_jInt32Array::reduce(tc::jst::function<U(U, double, double, _js_jInt32Array)> callbackfn, U initialValue) noexcept {
		return this->template _call<U>("reduce", callbackfn, initialValue);
	}
	inline auto _impl_js_jInt32Array::reduce(tc::jst::function<double(double, double, double, _js_jInt32Array)> callbackfn) noexcept {
		return this->template _call<double>("reduce", callbackfn);
	}
	inline auto _impl_js_jInt32Array::reduce(tc::jst::function<double(double, double, double, _js_jInt32Array)> callbackfn, double initialValue) noexcept {
		return this->template _call<double>("reduce", callbackfn, initialValue);
	}
	template<typename U>
	inline auto _impl_js_jInt32Array::reduceRight(tc::jst::function<U(U, double, double, _js_jInt32Array)> callbackfn, U initialValue) noexcept {
		return this->template _call<U>("reduceRight", callbackfn, initialValue);
	}
	inline auto _impl_js_jInt32Array::reduceRight(tc::jst::function<double(double, double, double, _js_jInt32Array)> callbackfn) noexcept {
		return this->template _call<double>("reduceRight", callbackfn);
	}
	inline auto _impl_js_jInt32Array::reduceRight(tc::jst::function<double(double, double, double, _js_jInt32Array)> callbackfn, double initialValue) noexcept {
		return this->template _call<double>("reduceRight", callbackfn, initialValue);
	}
	inline auto _impl_js_jInt32Array::reverse() noexcept {
		return this->template _call<_js_jInt32Array>("reverse");
	}
	inline auto _impl_js_jInt32Array::set(_js_jArrayLike<double> array, tc::jst::union_t<double, tc::js::undefined> offset) noexcept {
		 this->template _call<void>("set", array, offset);
	}
	inline auto _impl_js_jInt32Array::slice(tc::jst::union_t<double, tc::js::undefined> start, tc::jst::union_t<double, tc::js::undefined> end) noexcept {
		return this->template _call<_js_jInt32Array>("slice", start, end);
	}
	inline auto _impl_js_jInt32Array::some(tc::jst::function<tc::js::any(double, double, _js_jInt32Array)> predicate, tc::js::any thisArg) noexcept {
		return this->template _call<bool /*false*/>("some", predicate, thisArg);
	}
	inline auto _impl_js_jInt32Array::sort(tc::jst::union_t<tc::js::undefined, tc::jst::function<double(double, double)>> compareFn) noexcept {
		return this->template _call<_js_jInt32Array>("sort", compareFn);
	}
	inline auto _impl_js_jInt32Array::subarray(tc::jst::union_t<double, tc::js::undefined> begin, tc::jst::union_t<double, tc::js::undefined> end) noexcept {
		return this->template _call<_js_jInt32Array>("subarray", begin, end);
	}
	inline auto _impl_js_jInt32Array::toLocaleString() noexcept {
		return this->template _call<tc::js::string>("toLocaleString");
	}
	inline auto _impl_js_jInt32Array::toString() noexcept {
		return this->template _call<tc::js::string>("toString");
	}
	inline auto _impl_js_jInt32Array::valueOf() noexcept {
		return this->template _call<_js_jInt32Array>("valueOf");
	}
	inline auto _impl_js_jInt32Array::values() noexcept {
		return this->template _call<_js_jIterableIterator<double>>("values");
	}
	inline auto _impl_js_jInt32ArrayConstructor::prototype() noexcept { return this->template _getProperty<_js_jInt32Array>("prototype"); }
	inline auto _impl_js_jInt32ArrayConstructor::BYTES_PER_ELEMENT() noexcept { return this->template _getProperty<double>("BYTES_PER_ELEMENT"); }
	template<typename T>
	inline auto _impl_js_jInt32ArrayConstructor::from(_js_jArrayLike<T> arrayLike, tc::jst::function<double(T, double)> mapfn, tc::js::any thisArg) noexcept {
		return this->template _call<_js_jInt32Array>("from", arrayLike, mapfn, thisArg);
	}
	inline auto _impl_js_jInt32ArrayConstructor::from(_js_jArrayLike<double> arrayLike) noexcept {
		return this->template _call<_js_jInt32Array>("from", arrayLike);
	}
	inline auto _impl_js_jInt32ArrayConstructor::from(_js_jIterable<double> arrayLike, tc::jst::union_t<tc::js::undefined, tc::jst::function<double(double, double)>> mapfn, tc::js::any thisArg) noexcept {
		return this->template _call<_js_jInt32Array>("from", arrayLike, mapfn, thisArg);
	}
	inline auto _impl_js_jInt32ArrayConstructor::of(tc::jst::variadic_arg_t, double items) noexcept {
		return this->template _call<_js_jInt32Array>("of", items);
	}
	inline auto _impl_js_jInt8Array::BYTES_PER_ELEMENT() noexcept { return this->template _getProperty<double>("BYTES_PER_ELEMENT"); }
	inline auto _impl_js_jInt8Array::buffer() noexcept { return this->template _getProperty<_js_jArrayBufferLike>("buffer"); }
	inline auto _impl_js_jInt8Array::byteLength() noexcept { return this->template _getProperty<double>("byteLength"); }
	inline auto _impl_js_jInt8Array::byteOffset() noexcept { return this->template _getProperty<double>("byteOffset"); }
	inline auto _impl_js_jInt8Array::length() noexcept { return this->template _getProperty<double>("length"); }
	inline auto _impl_js_jInt8Array::_tcjs_construct() noexcept {
		return _js_jInt8Array(emscripten::val::global("Int8Array").new_());
	}
	inline auto _impl_js_jInt8Array::_tcjs_construct(double length) noexcept {
		return _js_jInt8Array(emscripten::val::global("Int8Array").new_(length));
	}
	inline auto _impl_js_jInt8Array::_tcjs_construct(tc::js::any /*UnionWithUnsupportedSubtype=[_js_jArrayBuffer,tc::js::any,_js_jArrayLike<double>]*/ array) noexcept {
		return _js_jInt8Array(emscripten::val::global("Int8Array").new_(array));
	}
	inline auto _impl_js_jInt8Array::_tcjs_construct(_js_jArrayBufferLike buffer, tc::jst::union_t<double, tc::js::undefined> byteOffset, tc::jst::union_t<double, tc::js::undefined> length) noexcept {
		return _js_jInt8Array(emscripten::val::global("Int8Array").new_(buffer, byteOffset, length));
	}
	inline auto _impl_js_jInt8Array::_tcjs_construct(_js_jIterable<double> elements) noexcept {
		return _js_jInt8Array(emscripten::val::global("Int8Array").new_(elements));
	}
	inline auto _impl_js_jInt8Array::copyWithin(double target, double start, tc::jst::union_t<double, tc::js::undefined> end) noexcept {
		return this->template _call<_js_jInt8Array>("copyWithin", target, start, end);
	}
	inline auto _impl_js_jInt8Array::entries() noexcept {
		return this->template _call<_js_jIterableIterator<std::tuple<double, double>>>("entries");
	}
	inline auto _impl_js_jInt8Array::every(tc::jst::function<tc::js::any(double, double, _js_jInt8Array)> predicate, tc::js::any thisArg) noexcept {
		return this->template _call<bool /*false*/>("every", predicate, thisArg);
	}
	inline auto _impl_js_jInt8Array::fill(double value, tc::jst::union_t<double, tc::js::undefined> start, tc::jst::union_t<double, tc::js::undefined> end) noexcept {
		return this->template _call<_js_jInt8Array>("fill", value, start, end);
	}
	inline auto _impl_js_jInt8Array::filter(tc::jst::function<tc::js::any(double, double, _js_jInt8Array)> predicate, tc::js::any thisArg) noexcept {
		return this->template _call<_js_jInt8Array>("filter", predicate, thisArg);
	}
	inline auto _impl_js_jInt8Array::find(tc::jst::function<bool /*false*/(double, double, _js_jInt8Array)> predicate, tc::js::any thisArg) noexcept {
		return this->template _call<tc::jst::union_t<double, tc::js::undefined>>("find", predicate, thisArg);
	}
	inline auto _impl_js_jInt8Array::findIndex(tc::jst::function<bool /*false*/(double, double, _js_jInt8Array)> predicate, tc::js::any thisArg) noexcept {
		return this->template _call<double>("findIndex", predicate, thisArg);
	}
	inline auto _impl_js_jInt8Array::forEach(tc::jst::function<void(double, double, _js_jInt8Array)> callbackfn, tc::js::any thisArg) noexcept {
		 this->template _call<void>("forEach", callbackfn, thisArg);
	}
	inline auto _impl_js_jInt8Array::includes(double searchElement, tc::jst::union_t<double, tc::js::undefined> fromIndex) noexcept {
		return this->template _call<bool /*false*/>("includes", searchElement, fromIndex);
	}
	inline auto _impl_js_jInt8Array::indexOf(double searchElement, tc::jst::union_t<double, tc::js::undefined> fromIndex) noexcept {
		return this->template _call<double>("indexOf", searchElement, fromIndex);
	}
	inline auto _impl_js_jInt8Array::join(tc::jst::union_t<tc::js::string, tc::js::undefined> separator) noexcept {
		return this->template _call<tc::js::string>("join", separator);
	}
	inline auto _impl_js_jInt8Array::keys() noexcept {
		return this->template _call<_js_jIterableIterator<double>>("keys");
	}
	inline auto _impl_js_jInt8Array::lastIndexOf(double searchElement, tc::jst::union_t<double, tc::js::undefined> fromIndex) noexcept {
		return this->template _call<double>("lastIndexOf", searchElement, fromIndex);
	}
	inline auto _impl_js_jInt8Array::map(tc::jst::function<double(double, double, _js_jInt8Array)> callbackfn, tc::js::any thisArg) noexcept {
		return this->template _call<_js_jInt8Array>("map", callbackfn, thisArg);
	}
	inline auto _impl_js_jInt8Array::operator[](double index) noexcept {
		return this->template _getProperty<double>(index);
	}
	template<typename U>
	inline auto _impl_js_jInt8Array::reduce(tc::jst::function<U(U, double, double, _js_jInt8Array)> callbackfn, U initialValue) noexcept {
		return this->template _call<U>("reduce", callbackfn, initialValue);
	}
	inline auto _impl_js_jInt8Array::reduce(tc::jst::function<double(double, double, double, _js_jInt8Array)> callbackfn) noexcept {
		return this->template _call<double>("reduce", callbackfn);
	}
	inline auto _impl_js_jInt8Array::reduce(tc::jst::function<double(double, double, double, _js_jInt8Array)> callbackfn, double initialValue) noexcept {
		return this->template _call<double>("reduce", callbackfn, initialValue);
	}
	template<typename U>
	inline auto _impl_js_jInt8Array::reduceRight(tc::jst::function<U(U, double, double, _js_jInt8Array)> callbackfn, U initialValue) noexcept {
		return this->template _call<U>("reduceRight", callbackfn, initialValue);
	}
	inline auto _impl_js_jInt8Array::reduceRight(tc::jst::function<double(double, double, double, _js_jInt8Array)> callbackfn) noexcept {
		return this->template _call<double>("reduceRight", callbackfn);
	}
	inline auto _impl_js_jInt8Array::reduceRight(tc::jst::function<double(double, double, double, _js_jInt8Array)> callbackfn, double initialValue) noexcept {
		return this->template _call<double>("reduceRight", callbackfn, initialValue);
	}
	inline auto _impl_js_jInt8Array::reverse() noexcept {
		return this->template _call<_js_jInt8Array>("reverse");
	}
	inline auto _impl_js_jInt8Array::set(_js_jArrayLike<double> array, tc::jst::union_t<double, tc::js::undefined> offset) noexcept {
		 this->template _call<void>("set", array, offset);
	}
	inline auto _impl_js_jInt8Array::slice(tc::jst::union_t<double, tc::js::undefined> start, tc::jst::union_t<double, tc::js::undefined> end) noexcept {
		return this->template _call<_js_jInt8Array>("slice", start, end);
	}
	inline auto _impl_js_jInt8Array::some(tc::jst::function<tc::js::any(double, double, _js_jInt8Array)> predicate, tc::js::any thisArg) noexcept {
		return this->template _call<bool /*false*/>("some", predicate, thisArg);
	}
	inline auto _impl_js_jInt8Array::sort(tc::jst::union_t<tc::js::undefined, tc::jst::function<double(double, double)>> compareFn) noexcept {
		return this->template _call<_js_jInt8Array>("sort", compareFn);
	}
	inline auto _impl_js_jInt8Array::subarray(tc::jst::union_t<double, tc::js::undefined> begin, tc::jst::union_t<double, tc::js::undefined> end) noexcept {
		return this->template _call<_js_jInt8Array>("subarray", begin, end);
	}
	inline auto _impl_js_jInt8Array::toLocaleString() noexcept {
		return this->template _call<tc::js::string>("toLocaleString");
	}
	inline auto _impl_js_jInt8Array::toString() noexcept {
		return this->template _call<tc::js::string>("toString");
	}
	inline auto _impl_js_jInt8Array::valueOf() noexcept {
		return this->template _call<_js_jInt8Array>("valueOf");
	}
	inline auto _impl_js_jInt8Array::values() noexcept {
		return this->template _call<_js_jIterableIterator<double>>("values");
	}
	inline auto _impl_js_jInt8ArrayConstructor::prototype() noexcept { return this->template _getProperty<_js_jInt8Array>("prototype"); }
	inline auto _impl_js_jInt8ArrayConstructor::BYTES_PER_ELEMENT() noexcept { return this->template _getProperty<double>("BYTES_PER_ELEMENT"); }
	template<typename T>
	inline auto _impl_js_jInt8ArrayConstructor::from(_js_jArrayLike<T> arrayLike, tc::jst::function<double(T, double)> mapfn, tc::js::any thisArg) noexcept {
		return this->template _call<_js_jInt8Array>("from", arrayLike, mapfn, thisArg);
	}
	inline auto _impl_js_jInt8ArrayConstructor::from(_js_jArrayLike<double> arrayLike) noexcept {
		return this->template _call<_js_jInt8Array>("from", arrayLike);
	}
	inline auto _impl_js_jInt8ArrayConstructor::from(_js_jIterable<double> arrayLike, tc::jst::union_t<tc::js::undefined, tc::jst::function<double(double, double)>> mapfn, tc::js::any thisArg) noexcept {
		return this->template _call<_js_jInt8Array>("from", arrayLike, mapfn, thisArg);
	}
	inline auto _impl_js_jInt8ArrayConstructor::of(tc::jst::variadic_arg_t, double items) noexcept {
		return this->template _call<_js_jInt8Array>("of", items);
	}
	inline auto _impl_js_jIntl_dCollator::_tcjs_construct(tc::jst::union_t<_js_jArray<tc::js::string>, tc::js::string, tc::js::undefined> locales, tc::jst::union_t<_js_jIntl_dCollatorOptions, tc::js::undefined> options) noexcept {
		return _js_jIntl_dCollator(emscripten::val::global("Intl")["Collator"].new_(locales, options));
	}
	inline auto _impl_js_jIntl_dCollator::compare(tc::js::string x, tc::js::string y) noexcept {
		return this->template _call<double>("compare", x, y);
	}
	inline auto _impl_js_jIntl_dCollator::resolvedOptions() noexcept {
		return this->template _call<_js_jIntl_dResolvedCollatorOptions>("resolvedOptions");
	}
	inline auto _impl_js_jIntl_dCollatorOptions::usage() noexcept { return this->template _getProperty<tc::jst::union_t<tc::js::string, tc::js::undefined>>("usage"); }
	inline void _impl_js_jIntl_dCollatorOptions::usage(tc::jst::union_t<tc::js::string, tc::js::undefined> v) noexcept { this->template _setProperty("usage", v); }
	inline auto _impl_js_jIntl_dCollatorOptions::localeMatcher() noexcept { return this->template _getProperty<tc::jst::union_t<tc::js::string, tc::js::undefined>>("localeMatcher"); }
	inline void _impl_js_jIntl_dCollatorOptions::localeMatcher(tc::jst::union_t<tc::js::string, tc::js::undefined> v) noexcept { this->template _setProperty("localeMatcher", v); }
	inline auto _impl_js_jIntl_dCollatorOptions::numeric() noexcept { return this->template _getProperty<tc::jst::union_t<bool /*false*/, tc::js::undefined>>("numeric"); }
	inline void _impl_js_jIntl_dCollatorOptions::numeric(tc::jst::union_t<bool /*false*/, tc::js::undefined> v) noexcept { this->template _setProperty("numeric", v); }
	inline auto _impl_js_jIntl_dCollatorOptions::caseFirst() noexcept { return this->template _getProperty<tc::jst::union_t<tc::js::string, tc::js::undefined>>("caseFirst"); }
	inline void _impl_js_jIntl_dCollatorOptions::caseFirst(tc::jst::union_t<tc::js::string, tc::js::undefined> v) noexcept { this->template _setProperty("caseFirst", v); }
	inline auto _impl_js_jIntl_dCollatorOptions::sensitivity() noexcept { return this->template _getProperty<tc::jst::union_t<tc::js::string, tc::js::undefined>>("sensitivity"); }
	inline void _impl_js_jIntl_dCollatorOptions::sensitivity(tc::jst::union_t<tc::js::string, tc::js::undefined> v) noexcept { this->template _setProperty("sensitivity", v); }
	inline auto _impl_js_jIntl_dCollatorOptions::ignorePunctuation() noexcept { return this->template _getProperty<tc::jst::union_t<bool /*false*/, tc::js::undefined>>("ignorePunctuation"); }
	inline void _impl_js_jIntl_dCollatorOptions::ignorePunctuation(tc::jst::union_t<bool /*false*/, tc::js::undefined> v) noexcept { this->template _setProperty("ignorePunctuation", v); }
	inline auto _impl_js_jIntl_dCollatorOptions::_tcjs_construct() noexcept {
		return emscripten::val::object();
	}
	inline auto _impl_js_jIntl_dDateTimeFormat::_tcjs_construct(tc::jst::union_t<_js_jArray<tc::js::string>, tc::js::string, tc::js::undefined> locales, tc::jst::union_t<_js_jIntl_dDateTimeFormatOptions, tc::js::undefined> options) noexcept {
		return _js_jIntl_dDateTimeFormat(emscripten::val::global("Intl")["DateTimeFormat"].new_(locales, options));
	}
	inline auto _impl_js_jIntl_dDateTimeFormat::format(tc::jst::union_t<_js_jDate, double, tc::js::undefined> date) noexcept {
		return this->template _call<tc::js::string>("format", date);
	}
	inline auto _impl_js_jIntl_dDateTimeFormat::formatToParts(tc::jst::union_t<_js_jDate, double, tc::js::undefined> date) noexcept {
		return this->template _call<_js_jArray<_js_jIntl_dDateTimeFormatPart>>("formatToParts", date);
	}
	inline auto _impl_js_jIntl_dDateTimeFormat::resolvedOptions() noexcept {
		return this->template _call<_js_jIntl_dResolvedDateTimeFormatOptions>("resolvedOptions");
	}
	inline auto _impl_js_jIntl_dDateTimeFormatOptions::localeMatcher() noexcept { return this->template _getProperty<tc::jst::union_t<tc::js::string /*"lookup"*/, tc::js::undefined>>("localeMatcher"); }
	inline void _impl_js_jIntl_dDateTimeFormatOptions::localeMatcher(tc::jst::union_t<tc::js::string /*"lookup"*/, tc::js::undefined> v) noexcept { this->template _setProperty("localeMatcher", v); }
	inline auto _impl_js_jIntl_dDateTimeFormatOptions::weekday() noexcept { return this->template _getProperty<tc::jst::union_t<tc::js::string /*"long"*/, tc::js::undefined>>("weekday"); }
	inline void _impl_js_jIntl_dDateTimeFormatOptions::weekday(tc::jst::union_t<tc::js::string /*"long"*/, tc::js::undefined> v) noexcept { this->template _setProperty("weekday", v); }
	inline auto _impl_js_jIntl_dDateTimeFormatOptions::era() noexcept { return this->template _getProperty<tc::jst::union_t<tc::js::string /*"long"*/, tc::js::undefined>>("era"); }
	inline void _impl_js_jIntl_dDateTimeFormatOptions::era(tc::jst::union_t<tc::js::string /*"long"*/, tc::js::undefined> v) noexcept { this->template _setProperty("era", v); }
	inline auto _impl_js_jIntl_dDateTimeFormatOptions::year() noexcept { return this->template _getProperty<tc::jst::union_t<tc::js::string /*"numeric"*/, tc::js::undefined>>("year"); }
	inline void _impl_js_jIntl_dDateTimeFormatOptions::year(tc::jst::union_t<tc::js::string /*"numeric"*/, tc::js::undefined> v) noexcept { this->template _setProperty("year", v); }
	inline auto _impl_js_jIntl_dDateTimeFormatOptions::month() noexcept { return this->template _getProperty<tc::jst::union_t<tc::js::string /*"long"*/, tc::js::undefined>>("month"); }
	inline void _impl_js_jIntl_dDateTimeFormatOptions::month(tc::jst::union_t<tc::js::string /*"long"*/, tc::js::undefined> v) noexcept { this->template _setProperty("month", v); }
	inline auto _impl_js_jIntl_dDateTimeFormatOptions::day() noexcept { return this->template _getProperty<tc::jst::union_t<tc::js::string /*"numeric"*/, tc::js::undefined>>("day"); }
	inline void _impl_js_jIntl_dDateTimeFormatOptions::day(tc::jst::union_t<tc::js::string /*"numeric"*/, tc::js::undefined> v) noexcept { this->template _setProperty("day", v); }
	inline auto _impl_js_jIntl_dDateTimeFormatOptions::hour() noexcept { return this->template _getProperty<tc::jst::union_t<tc::js::string /*"numeric"*/, tc::js::undefined>>("hour"); }
	inline void _impl_js_jIntl_dDateTimeFormatOptions::hour(tc::jst::union_t<tc::js::string /*"numeric"*/, tc::js::undefined> v) noexcept { this->template _setProperty("hour", v); }
	inline auto _impl_js_jIntl_dDateTimeFormatOptions::minute() noexcept { return this->template _getProperty<tc::jst::union_t<tc::js::string /*"numeric"*/, tc::js::undefined>>("minute"); }
	inline void _impl_js_jIntl_dDateTimeFormatOptions::minute(tc::jst::union_t<tc::js::string /*"numeric"*/, tc::js::undefined> v) noexcept { this->template _setProperty("minute", v); }
	inline auto _impl_js_jIntl_dDateTimeFormatOptions::second() noexcept { return this->template _getProperty<tc::jst::union_t<tc::js::string /*"numeric"*/, tc::js::undefined>>("second"); }
	inline void _impl_js_jIntl_dDateTimeFormatOptions::second(tc::jst::union_t<tc::js::string /*"numeric"*/, tc::js::undefined> v) noexcept { this->template _setProperty("second", v); }
	inline auto _impl_js_jIntl_dDateTimeFormatOptions::timeZoneName() noexcept { return this->template _getProperty<tc::jst::union_t<tc::js::string /*"long"*/, tc::js::undefined>>("timeZoneName"); }
	inline void _impl_js_jIntl_dDateTimeFormatOptions::timeZoneName(tc::jst::union_t<tc::js::string /*"long"*/, tc::js::undefined> v) noexcept { this->template _setProperty("timeZoneName", v); }
	inline auto _impl_js_jIntl_dDateTimeFormatOptions::formatMatcher() noexcept { return this->template _getProperty<tc::jst::union_t<tc::js::string /*"best fit"*/, tc::js::undefined>>("formatMatcher"); }
	inline void _impl_js_jIntl_dDateTimeFormatOptions::formatMatcher(tc::jst::union_t<tc::js::string /*"best fit"*/, tc::js::undefined> v) noexcept { this->template _setProperty("formatMatcher", v); }
	inline auto _impl_js_jIntl_dDateTimeFormatOptions::hour12() noexcept { return this->template _getProperty<tc::jst::union_t<bool /*false*/, tc::js::undefined>>("hour12"); }
	inline void _impl_js_jIntl_dDateTimeFormatOptions::hour12(tc::jst::union_t<bool /*false*/, tc::js::undefined> v) noexcept { this->template _setProperty("hour12", v); }
	inline auto _impl_js_jIntl_dDateTimeFormatOptions::timeZone() noexcept { return this->template _getProperty<tc::jst::union_t<tc::js::string, tc::js::undefined>>("timeZone"); }
	inline void _impl_js_jIntl_dDateTimeFormatOptions::timeZone(tc::jst::union_t<tc::js::string, tc::js::undefined> v) noexcept { this->template _setProperty("timeZone", v); }
	inline auto _impl_js_jIntl_dDateTimeFormatOptions::_tcjs_construct() noexcept {
		return emscripten::val::object();
	}
	inline auto _impl_js_jIntl_dDateTimeFormatPart::type() noexcept { return this->template _getProperty<tc::js::string /*"day"*/>("type"); }
	inline void _impl_js_jIntl_dDateTimeFormatPart::type(tc::js::string /*"day"*/ v) noexcept { this->template _setProperty("type", v); }
	inline auto _impl_js_jIntl_dDateTimeFormatPart::value() noexcept { return this->template _getProperty<tc::js::string>("value"); }
	inline void _impl_js_jIntl_dDateTimeFormatPart::value(tc::js::string v) noexcept { this->template _setProperty("value", v); }
	inline auto _impl_js_jIntl_dNumberFormat::_tcjs_construct(tc::jst::union_t<_js_jArray<tc::js::string>, tc::js::string, tc::js::undefined> locales, tc::jst::union_t<_js_jIntl_dNumberFormatOptions, tc::js::undefined> options) noexcept {
		return _js_jIntl_dNumberFormat(emscripten::val::global("Intl")["NumberFormat"].new_(locales, options));
	}
	inline auto _impl_js_jIntl_dNumberFormat::format(double value) noexcept {
		return this->template _call<tc::js::string>("format", value);
	}
	inline auto _impl_js_jIntl_dNumberFormat::format(tc::js::any /*UnionWithUnsupportedSubtype=[double,tc::js::any]*/ value) noexcept {
		return this->template _call<tc::js::string>("format", value);
	}
	inline auto _impl_js_jIntl_dNumberFormat::formatToParts(tc::js::any /*UnionWithUnsupportedSubtype=[tc::js::undefined,double,tc::js::any]*/ number) noexcept {
		return this->template _call<_js_jArray<_js_jIntl_dNumberFormatPart>>("formatToParts", number);
	}
	inline auto _impl_js_jIntl_dNumberFormat::resolvedOptions() noexcept {
		return this->template _call<_js_jIntl_dResolvedNumberFormatOptions>("resolvedOptions");
	}
	inline auto _impl_js_jIntl_dNumberFormatOptions::localeMatcher() noexcept { return this->template _getProperty<tc::jst::union_t<tc::js::string, tc::js::undefined>>("localeMatcher"); }
	inline void _impl_js_jIntl_dNumberFormatOptions::localeMatcher(tc::jst::union_t<tc::js::string, tc::js::undefined> v) noexcept { this->template _setProperty("localeMatcher", v); }
	inline auto _impl_js_jIntl_dNumberFormatOptions::style() noexcept { return this->template _getProperty<tc::jst::union_t<tc::js::string, tc::js::undefined>>("style"); }
	inline void _impl_js_jIntl_dNumberFormatOptions::style(tc::jst::union_t<tc::js::string, tc::js::undefined> v) noexcept { this->template _setProperty("style", v); }
	inline auto _impl_js_jIntl_dNumberFormatOptions::currency() noexcept { return this->template _getProperty<tc::jst::union_t<tc::js::string, tc::js::undefined>>("currency"); }
	inline void _impl_js_jIntl_dNumberFormatOptions::currency(tc::jst::union_t<tc::js::string, tc::js::undefined> v) noexcept { this->template _setProperty("currency", v); }
	inline auto _impl_js_jIntl_dNumberFormatOptions::currencyDisplay() noexcept { return this->template _getProperty<tc::jst::union_t<tc::js::string, tc::js::undefined>>("currencyDisplay"); }
	inline void _impl_js_jIntl_dNumberFormatOptions::currencyDisplay(tc::jst::union_t<tc::js::string, tc::js::undefined> v) noexcept { this->template _setProperty("currencyDisplay", v); }
	inline auto _impl_js_jIntl_dNumberFormatOptions::currencySign() noexcept { return this->template _getProperty<tc::jst::union_t<tc::js::string, tc::js::undefined>>("currencySign"); }
	inline void _impl_js_jIntl_dNumberFormatOptions::currencySign(tc::jst::union_t<tc::js::string, tc::js::undefined> v) noexcept { this->template _setProperty("currencySign", v); }
	inline auto _impl_js_jIntl_dNumberFormatOptions::useGrouping() noexcept { return this->template _getProperty<tc::jst::union_t<bool /*false*/, tc::js::undefined>>("useGrouping"); }
	inline void _impl_js_jIntl_dNumberFormatOptions::useGrouping(tc::jst::union_t<bool /*false*/, tc::js::undefined> v) noexcept { this->template _setProperty("useGrouping", v); }
	inline auto _impl_js_jIntl_dNumberFormatOptions::minimumIntegerDigits() noexcept { return this->template _getProperty<tc::jst::union_t<double, tc::js::undefined>>("minimumIntegerDigits"); }
	inline void _impl_js_jIntl_dNumberFormatOptions::minimumIntegerDigits(tc::jst::union_t<double, tc::js::undefined> v) noexcept { this->template _setProperty("minimumIntegerDigits", v); }
	inline auto _impl_js_jIntl_dNumberFormatOptions::minimumFractionDigits() noexcept { return this->template _getProperty<tc::jst::union_t<double, tc::js::undefined>>("minimumFractionDigits"); }
	inline void _impl_js_jIntl_dNumberFormatOptions::minimumFractionDigits(tc::jst::union_t<double, tc::js::undefined> v) noexcept { this->template _setProperty("minimumFractionDigits", v); }
	inline auto _impl_js_jIntl_dNumberFormatOptions::maximumFractionDigits() noexcept { return this->template _getProperty<tc::jst::union_t<double, tc::js::undefined>>("maximumFractionDigits"); }
	inline void _impl_js_jIntl_dNumberFormatOptions::maximumFractionDigits(tc::jst::union_t<double, tc::js::undefined> v) noexcept { this->template _setProperty("maximumFractionDigits", v); }
	inline auto _impl_js_jIntl_dNumberFormatOptions::minimumSignificantDigits() noexcept { return this->template _getProperty<tc::jst::union_t<double, tc::js::undefined>>("minimumSignificantDigits"); }
	inline void _impl_js_jIntl_dNumberFormatOptions::minimumSignificantDigits(tc::jst::union_t<double, tc::js::undefined> v) noexcept { this->template _setProperty("minimumSignificantDigits", v); }
	inline auto _impl_js_jIntl_dNumberFormatOptions::maximumSignificantDigits() noexcept { return this->template _getProperty<tc::jst::union_t<double, tc::js::undefined>>("maximumSignificantDigits"); }
	inline void _impl_js_jIntl_dNumberFormatOptions::maximumSignificantDigits(tc::jst::union_t<double, tc::js::undefined> v) noexcept { this->template _setProperty("maximumSignificantDigits", v); }
	inline auto _impl_js_jIntl_dNumberFormatOptions::_tcjs_construct() noexcept {
		return emscripten::val::object();
	}
	inline auto _impl_js_jIntl_dNumberFormatPart::type() noexcept { return this->template _getProperty<tc::js::string /*"literal"*/>("type"); }
	inline void _impl_js_jIntl_dNumberFormatPart::type(tc::js::string /*"literal"*/ v) noexcept { this->template _setProperty("type", v); }
	inline auto _impl_js_jIntl_dNumberFormatPart::value() noexcept { return this->template _getProperty<tc::js::string>("value"); }
	inline void _impl_js_jIntl_dNumberFormatPart::value(tc::js::string v) noexcept { this->template _setProperty("value", v); }
	inline auto _impl_js_jIntl_dPluralRules::_tcjs_construct(tc::jst::union_t<_js_jArray<tc::js::string>, tc::js::string, tc::js::undefined> locales, tc::jst::union_t<_js_jIntl_dPluralRulesOptions, tc::js::undefined> options) noexcept {
		return _js_jIntl_dPluralRules(emscripten::val::global("Intl")["PluralRules"].new_(locales, options));
	}
	inline auto _impl_js_jIntl_dPluralRules::resolvedOptions() noexcept {
		return this->template _call<_js_jIntl_dResolvedPluralRulesOptions>("resolvedOptions");
	}
	inline auto _impl_js_jIntl_dPluralRules::select(double n) noexcept {
		return this->template _call<tc::js::string /*"zero"*/>("select", n);
	}
	inline auto _impl_js_jIntl_dPluralRulesOptions::localeMatcher() noexcept { return this->template _getProperty<tc::jst::union_t<tc::js::string /*"lookup"*/, tc::js::undefined>>("localeMatcher"); }
	inline void _impl_js_jIntl_dPluralRulesOptions::localeMatcher(tc::jst::union_t<tc::js::string /*"lookup"*/, tc::js::undefined> v) noexcept { this->template _setProperty("localeMatcher", v); }
	inline auto _impl_js_jIntl_dPluralRulesOptions::type() noexcept { return this->template _getProperty<tc::jst::union_t<tc::js::string /*"cardinal"*/, tc::js::undefined>>("type"); }
	inline void _impl_js_jIntl_dPluralRulesOptions::type(tc::jst::union_t<tc::js::string /*"cardinal"*/, tc::js::undefined> v) noexcept { this->template _setProperty("type", v); }
	inline auto _impl_js_jIntl_dPluralRulesOptions::minimumIntegerDigits() noexcept { return this->template _getProperty<tc::jst::union_t<double, tc::js::undefined>>("minimumIntegerDigits"); }
	inline void _impl_js_jIntl_dPluralRulesOptions::minimumIntegerDigits(tc::jst::union_t<double, tc::js::undefined> v) noexcept { this->template _setProperty("minimumIntegerDigits", v); }
	inline auto _impl_js_jIntl_dPluralRulesOptions::minimumFractionDigits() noexcept { return this->template _getProperty<tc::jst::union_t<double, tc::js::undefined>>("minimumFractionDigits"); }
	inline void _impl_js_jIntl_dPluralRulesOptions::minimumFractionDigits(tc::jst::union_t<double, tc::js::undefined> v) noexcept { this->template _setProperty("minimumFractionDigits", v); }
	inline auto _impl_js_jIntl_dPluralRulesOptions::maximumFractionDigits() noexcept { return this->template _getProperty<tc::jst::union_t<double, tc::js::undefined>>("maximumFractionDigits"); }
	inline void _impl_js_jIntl_dPluralRulesOptions::maximumFractionDigits(tc::jst::union_t<double, tc::js::undefined> v) noexcept { this->template _setProperty("maximumFractionDigits", v); }
	inline auto _impl_js_jIntl_dPluralRulesOptions::minimumSignificantDigits() noexcept { return this->template _getProperty<tc::jst::union_t<double, tc::js::undefined>>("minimumSignificantDigits"); }
	inline void _impl_js_jIntl_dPluralRulesOptions::minimumSignificantDigits(tc::jst::union_t<double, tc::js::undefined> v) noexcept { this->template _setProperty("minimumSignificantDigits", v); }
	inline auto _impl_js_jIntl_dPluralRulesOptions::maximumSignificantDigits() noexcept { return this->template _getProperty<tc::jst::union_t<double, tc::js::undefined>>("maximumSignificantDigits"); }
	inline void _impl_js_jIntl_dPluralRulesOptions::maximumSignificantDigits(tc::jst::union_t<double, tc::js::undefined> v) noexcept { this->template _setProperty("maximumSignificantDigits", v); }
	inline auto _impl_js_jIntl_dPluralRulesOptions::_tcjs_construct() noexcept {
		return emscripten::val::object();
	}
	inline auto _impl_js_jIntl_dResolvedCollatorOptions::locale() noexcept { return this->template _getProperty<tc::js::string>("locale"); }
	inline void _impl_js_jIntl_dResolvedCollatorOptions::locale(tc::js::string v) noexcept { this->template _setProperty("locale", v); }
	inline auto _impl_js_jIntl_dResolvedCollatorOptions::usage() noexcept { return this->template _getProperty<tc::js::string>("usage"); }
	inline void _impl_js_jIntl_dResolvedCollatorOptions::usage(tc::js::string v) noexcept { this->template _setProperty("usage", v); }
	inline auto _impl_js_jIntl_dResolvedCollatorOptions::sensitivity() noexcept { return this->template _getProperty<tc::js::string>("sensitivity"); }
	inline void _impl_js_jIntl_dResolvedCollatorOptions::sensitivity(tc::js::string v) noexcept { this->template _setProperty("sensitivity", v); }
	inline auto _impl_js_jIntl_dResolvedCollatorOptions::ignorePunctuation() noexcept { return this->template _getProperty<bool /*false*/>("ignorePunctuation"); }
	inline void _impl_js_jIntl_dResolvedCollatorOptions::ignorePunctuation(bool /*false*/ v) noexcept { this->template _setProperty("ignorePunctuation", v); }
	inline auto _impl_js_jIntl_dResolvedCollatorOptions::collation() noexcept { return this->template _getProperty<tc::js::string>("collation"); }
	inline void _impl_js_jIntl_dResolvedCollatorOptions::collation(tc::js::string v) noexcept { this->template _setProperty("collation", v); }
	inline auto _impl_js_jIntl_dResolvedCollatorOptions::caseFirst() noexcept { return this->template _getProperty<tc::js::string>("caseFirst"); }
	inline void _impl_js_jIntl_dResolvedCollatorOptions::caseFirst(tc::js::string v) noexcept { this->template _setProperty("caseFirst", v); }
	inline auto _impl_js_jIntl_dResolvedCollatorOptions::numeric() noexcept { return this->template _getProperty<bool /*false*/>("numeric"); }
	inline void _impl_js_jIntl_dResolvedCollatorOptions::numeric(bool /*false*/ v) noexcept { this->template _setProperty("numeric", v); }
	inline auto _impl_js_jIntl_dResolvedDateTimeFormatOptions::locale() noexcept { return this->template _getProperty<tc::js::string>("locale"); }
	inline void _impl_js_jIntl_dResolvedDateTimeFormatOptions::locale(tc::js::string v) noexcept { this->template _setProperty("locale", v); }
	inline auto _impl_js_jIntl_dResolvedDateTimeFormatOptions::calendar() noexcept { return this->template _getProperty<tc::js::string>("calendar"); }
	inline void _impl_js_jIntl_dResolvedDateTimeFormatOptions::calendar(tc::js::string v) noexcept { this->template _setProperty("calendar", v); }
	inline auto _impl_js_jIntl_dResolvedDateTimeFormatOptions::numberingSystem() noexcept { return this->template _getProperty<tc::js::string>("numberingSystem"); }
	inline void _impl_js_jIntl_dResolvedDateTimeFormatOptions::numberingSystem(tc::js::string v) noexcept { this->template _setProperty("numberingSystem", v); }
	inline auto _impl_js_jIntl_dResolvedDateTimeFormatOptions::timeZone() noexcept { return this->template _getProperty<tc::js::string>("timeZone"); }
	inline void _impl_js_jIntl_dResolvedDateTimeFormatOptions::timeZone(tc::js::string v) noexcept { this->template _setProperty("timeZone", v); }
	inline auto _impl_js_jIntl_dResolvedDateTimeFormatOptions::hour12() noexcept { return this->template _getProperty<tc::jst::union_t<bool /*false*/, tc::js::undefined>>("hour12"); }
	inline void _impl_js_jIntl_dResolvedDateTimeFormatOptions::hour12(tc::jst::union_t<bool /*false*/, tc::js::undefined> v) noexcept { this->template _setProperty("hour12", v); }
	inline auto _impl_js_jIntl_dResolvedDateTimeFormatOptions::weekday() noexcept { return this->template _getProperty<tc::jst::union_t<tc::js::string, tc::js::undefined>>("weekday"); }
	inline void _impl_js_jIntl_dResolvedDateTimeFormatOptions::weekday(tc::jst::union_t<tc::js::string, tc::js::undefined> v) noexcept { this->template _setProperty("weekday", v); }
	inline auto _impl_js_jIntl_dResolvedDateTimeFormatOptions::era() noexcept { return this->template _getProperty<tc::jst::union_t<tc::js::string, tc::js::undefined>>("era"); }
	inline void _impl_js_jIntl_dResolvedDateTimeFormatOptions::era(tc::jst::union_t<tc::js::string, tc::js::undefined> v) noexcept { this->template _setProperty("era", v); }
	inline auto _impl_js_jIntl_dResolvedDateTimeFormatOptions::year() noexcept { return this->template _getProperty<tc::jst::union_t<tc::js::string, tc::js::undefined>>("year"); }
	inline void _impl_js_jIntl_dResolvedDateTimeFormatOptions::year(tc::jst::union_t<tc::js::string, tc::js::undefined> v) noexcept { this->template _setProperty("year", v); }
	inline auto _impl_js_jIntl_dResolvedDateTimeFormatOptions::month() noexcept { return this->template _getProperty<tc::jst::union_t<tc::js::string, tc::js::undefined>>("month"); }
	inline void _impl_js_jIntl_dResolvedDateTimeFormatOptions::month(tc::jst::union_t<tc::js::string, tc::js::undefined> v) noexcept { this->template _setProperty("month", v); }
	inline auto _impl_js_jIntl_dResolvedDateTimeFormatOptions::day() noexcept { return this->template _getProperty<tc::jst::union_t<tc::js::string, tc::js::undefined>>("day"); }
	inline void _impl_js_jIntl_dResolvedDateTimeFormatOptions::day(tc::jst::union_t<tc::js::string, tc::js::undefined> v) noexcept { this->template _setProperty("day", v); }
	inline auto _impl_js_jIntl_dResolvedDateTimeFormatOptions::hour() noexcept { return this->template _getProperty<tc::jst::union_t<tc::js::string, tc::js::undefined>>("hour"); }
	inline void _impl_js_jIntl_dResolvedDateTimeFormatOptions::hour(tc::jst::union_t<tc::js::string, tc::js::undefined> v) noexcept { this->template _setProperty("hour", v); }
	inline auto _impl_js_jIntl_dResolvedDateTimeFormatOptions::minute() noexcept { return this->template _getProperty<tc::jst::union_t<tc::js::string, tc::js::undefined>>("minute"); }
	inline void _impl_js_jIntl_dResolvedDateTimeFormatOptions::minute(tc::jst::union_t<tc::js::string, tc::js::undefined> v) noexcept { this->template _setProperty("minute", v); }
	inline auto _impl_js_jIntl_dResolvedDateTimeFormatOptions::second() noexcept { return this->template _getProperty<tc::jst::union_t<tc::js::string, tc::js::undefined>>("second"); }
	inline void _impl_js_jIntl_dResolvedDateTimeFormatOptions::second(tc::jst::union_t<tc::js::string, tc::js::undefined> v) noexcept { this->template _setProperty("second", v); }
	inline auto _impl_js_jIntl_dResolvedDateTimeFormatOptions::timeZoneName() noexcept { return this->template _getProperty<tc::jst::union_t<tc::js::string, tc::js::undefined>>("timeZoneName"); }
	inline void _impl_js_jIntl_dResolvedDateTimeFormatOptions::timeZoneName(tc::jst::union_t<tc::js::string, tc::js::undefined> v) noexcept { this->template _setProperty("timeZoneName", v); }
	inline auto _impl_js_jIntl_dResolvedNumberFormatOptions::locale() noexcept { return this->template _getProperty<tc::js::string>("locale"); }
	inline void _impl_js_jIntl_dResolvedNumberFormatOptions::locale(tc::js::string v) noexcept { this->template _setProperty("locale", v); }
	inline auto _impl_js_jIntl_dResolvedNumberFormatOptions::numberingSystem() noexcept { return this->template _getProperty<tc::js::string>("numberingSystem"); }
	inline void _impl_js_jIntl_dResolvedNumberFormatOptions::numberingSystem(tc::js::string v) noexcept { this->template _setProperty("numberingSystem", v); }
	inline auto _impl_js_jIntl_dResolvedNumberFormatOptions::style() noexcept { return this->template _getProperty<tc::js::string>("style"); }
	inline void _impl_js_jIntl_dResolvedNumberFormatOptions::style(tc::js::string v) noexcept { this->template _setProperty("style", v); }
	inline auto _impl_js_jIntl_dResolvedNumberFormatOptions::currency() noexcept { return this->template _getProperty<tc::jst::union_t<tc::js::string, tc::js::undefined>>("currency"); }
	inline void _impl_js_jIntl_dResolvedNumberFormatOptions::currency(tc::jst::union_t<tc::js::string, tc::js::undefined> v) noexcept { this->template _setProperty("currency", v); }
	inline auto _impl_js_jIntl_dResolvedNumberFormatOptions::currencyDisplay() noexcept { return this->template _getProperty<tc::jst::union_t<tc::js::string, tc::js::undefined>>("currencyDisplay"); }
	inline void _impl_js_jIntl_dResolvedNumberFormatOptions::currencyDisplay(tc::jst::union_t<tc::js::string, tc::js::undefined> v) noexcept { this->template _setProperty("currencyDisplay", v); }
	inline auto _impl_js_jIntl_dResolvedNumberFormatOptions::minimumIntegerDigits() noexcept { return this->template _getProperty<double>("minimumIntegerDigits"); }
	inline void _impl_js_jIntl_dResolvedNumberFormatOptions::minimumIntegerDigits(double v) noexcept { this->template _setProperty("minimumIntegerDigits", v); }
	inline auto _impl_js_jIntl_dResolvedNumberFormatOptions::minimumFractionDigits() noexcept { return this->template _getProperty<double>("minimumFractionDigits"); }
	inline void _impl_js_jIntl_dResolvedNumberFormatOptions::minimumFractionDigits(double v) noexcept { this->template _setProperty("minimumFractionDigits", v); }
	inline auto _impl_js_jIntl_dResolvedNumberFormatOptions::maximumFractionDigits() noexcept { return this->template _getProperty<double>("maximumFractionDigits"); }
	inline void _impl_js_jIntl_dResolvedNumberFormatOptions::maximumFractionDigits(double v) noexcept { this->template _setProperty("maximumFractionDigits", v); }
	inline auto _impl_js_jIntl_dResolvedNumberFormatOptions::minimumSignificantDigits() noexcept { return this->template _getProperty<tc::jst::union_t<double, tc::js::undefined>>("minimumSignificantDigits"); }
	inline void _impl_js_jIntl_dResolvedNumberFormatOptions::minimumSignificantDigits(tc::jst::union_t<double, tc::js::undefined> v) noexcept { this->template _setProperty("minimumSignificantDigits", v); }
	inline auto _impl_js_jIntl_dResolvedNumberFormatOptions::maximumSignificantDigits() noexcept { return this->template _getProperty<tc::jst::union_t<double, tc::js::undefined>>("maximumSignificantDigits"); }
	inline void _impl_js_jIntl_dResolvedNumberFormatOptions::maximumSignificantDigits(tc::jst::union_t<double, tc::js::undefined> v) noexcept { this->template _setProperty("maximumSignificantDigits", v); }
	inline auto _impl_js_jIntl_dResolvedNumberFormatOptions::useGrouping() noexcept { return this->template _getProperty<bool /*false*/>("useGrouping"); }
	inline void _impl_js_jIntl_dResolvedNumberFormatOptions::useGrouping(bool /*false*/ v) noexcept { this->template _setProperty("useGrouping", v); }
	inline auto _impl_js_jIntl_dResolvedPluralRulesOptions::locale() noexcept { return this->template _getProperty<tc::js::string>("locale"); }
	inline void _impl_js_jIntl_dResolvedPluralRulesOptions::locale(tc::js::string v) noexcept { this->template _setProperty("locale", v); }
	inline auto _impl_js_jIntl_dResolvedPluralRulesOptions::pluralCategories() noexcept { return this->template _getProperty<_js_jArray<tc::js::string /*"zero"*/>>("pluralCategories"); }
	inline void _impl_js_jIntl_dResolvedPluralRulesOptions::pluralCategories(_js_jArray<tc::js::string /*"zero"*/> v) noexcept { this->template _setProperty("pluralCategories", v); }
	inline auto _impl_js_jIntl_dResolvedPluralRulesOptions::type() noexcept { return this->template _getProperty<tc::js::string /*"cardinal"*/>("type"); }
	inline void _impl_js_jIntl_dResolvedPluralRulesOptions::type(tc::js::string /*"cardinal"*/ v) noexcept { this->template _setProperty("type", v); }
	inline auto _impl_js_jIntl_dResolvedPluralRulesOptions::minimumIntegerDigits() noexcept { return this->template _getProperty<double>("minimumIntegerDigits"); }
	inline void _impl_js_jIntl_dResolvedPluralRulesOptions::minimumIntegerDigits(double v) noexcept { this->template _setProperty("minimumIntegerDigits", v); }
	inline auto _impl_js_jIntl_dResolvedPluralRulesOptions::minimumFractionDigits() noexcept { return this->template _getProperty<double>("minimumFractionDigits"); }
	inline void _impl_js_jIntl_dResolvedPluralRulesOptions::minimumFractionDigits(double v) noexcept { this->template _setProperty("minimumFractionDigits", v); }
	inline auto _impl_js_jIntl_dResolvedPluralRulesOptions::maximumFractionDigits() noexcept { return this->template _getProperty<double>("maximumFractionDigits"); }
	inline void _impl_js_jIntl_dResolvedPluralRulesOptions::maximumFractionDigits(double v) noexcept { this->template _setProperty("maximumFractionDigits", v); }
	inline auto _impl_js_jIntl_dResolvedPluralRulesOptions::minimumSignificantDigits() noexcept { return this->template _getProperty<tc::jst::union_t<double, tc::js::undefined>>("minimumSignificantDigits"); }
	inline void _impl_js_jIntl_dResolvedPluralRulesOptions::minimumSignificantDigits(tc::jst::union_t<double, tc::js::undefined> v) noexcept { this->template _setProperty("minimumSignificantDigits", v); }
	inline auto _impl_js_jIntl_dResolvedPluralRulesOptions::maximumSignificantDigits() noexcept { return this->template _getProperty<tc::jst::union_t<double, tc::js::undefined>>("maximumSignificantDigits"); }
	inline void _impl_js_jIntl_dResolvedPluralRulesOptions::maximumSignificantDigits(tc::jst::union_t<double, tc::js::undefined> v) noexcept { this->template _setProperty("maximumSignificantDigits", v); }
	template<typename TReturn>
	inline auto _impl_js_jIteratorReturnResult<TReturn>::done() noexcept { return this->template _getProperty<bool /*true*/>("done"); }
	template<typename TReturn>
	inline void _impl_js_jIteratorReturnResult<TReturn>::done(bool /*true*/ v) noexcept { this->template _setProperty("done", v); }
	template<typename TReturn>
	inline auto _impl_js_jIteratorReturnResult<TReturn>::value() noexcept { return this->template _getProperty<TReturn>("value"); }
	template<typename TReturn>
	inline void _impl_js_jIteratorReturnResult<TReturn>::value(TReturn v) noexcept { this->template _setProperty("value", v); }
	template<typename TYield>
	inline auto _impl_js_jIteratorYieldResult<TYield>::done() noexcept { return this->template _getProperty<tc::jst::union_t<bool /*false*/, tc::js::undefined>>("done"); }
	template<typename TYield>
	inline void _impl_js_jIteratorYieldResult<TYield>::done(tc::jst::union_t<bool /*false*/, tc::js::undefined> v) noexcept { this->template _setProperty("done", v); }
	template<typename TYield>
	inline auto _impl_js_jIteratorYieldResult<TYield>::value() noexcept { return this->template _getProperty<TYield>("value"); }
	template<typename TYield>
	inline void _impl_js_jIteratorYieldResult<TYield>::value(TYield v) noexcept { this->template _setProperty("value", v); }
	inline auto _impl_js_jJSON::parse(tc::js::string text, tc::jst::union_t<tc::js::undefined, tc::jst::function<tc::js::any(tc::js::string, tc::js::any)>> reviver) noexcept {
		return this->template _call<tc::js::any>("parse", text, reviver);
	}
	inline auto _impl_js_jJSON::stringify(tc::js::any value, tc::jst::union_t<_js_jArray<tc::jst::union_t<double, tc::js::string>>, tc::js::null, tc::js::undefined> replacer, tc::jst::union_t<double, tc::js::string, tc::js::undefined> space) noexcept {
		return this->template _call<tc::js::string>("stringify", value, replacer, space);
	}
	inline auto _impl_js_jJSON::stringify(tc::js::any value, tc::jst::union_t<tc::js::undefined, tc::jst::function<tc::js::any(tc::js::string, tc::js::any)>> replacer, tc::jst::union_t<double, tc::js::string, tc::js::undefined> space) noexcept {
		return this->template _call<tc::js::string>("stringify", value, replacer, space);
	}
	template<typename K, typename V>
	inline auto _impl_js_jMap<K, V>::size() noexcept { return this->template _getProperty<double>("size"); }
	template<typename K, typename V>
	inline auto _impl_js_jMap<K, V>::_tcjs_construct() noexcept {
		return _js_jMap<K, V>(emscripten::val::global("Map").new_());
	}
	template<typename K, typename V>
	inline auto _impl_js_jMap<K, V>::_tcjs_construct(tc::jst::union_t<_js_jReadonlyArray<std::tuple<K, V>>, tc::js::null, tc::js::undefined> entries) noexcept {
		return _js_jMap<K, V>(emscripten::val::global("Map").new_(entries));
	}
	template<typename K, typename V>
	inline auto _impl_js_jMap<K, V>::_tcjs_construct(_js_jIterable<std::tuple<K, V>> iterable) noexcept {
		return _js_jMap<K, V>(emscripten::val::global("Map").new_(iterable));
	}
	template<typename K, typename V>
	inline auto _impl_js_jMap<K, V>::clear() noexcept {
		 this->template _call<void>("clear");
	}
	template<typename K, typename V>
	inline auto _impl_js_jMap<K, V>::delete_(K key) noexcept {
		return this->template _call<bool /*false*/>("delete", key);
	}
	template<typename K, typename V>
	inline auto _impl_js_jMap<K, V>::entries() noexcept {
		return this->template _call<_js_jIterableIterator<std::tuple<K, V>>>("entries");
	}
	template<typename K, typename V>
	inline auto _impl_js_jMap<K, V>::forEach(tc::jst::function<void(V, K, _js_jMap<K, V>)> callbackfn, tc::js::any thisArg) noexcept {
		 this->template _call<void>("forEach", callbackfn, thisArg);
	}
	template<typename K, typename V>
	inline auto _impl_js_jMap<K, V>::get(K key) noexcept {
		return this->template _call<tc::jst::union_t<V, tc::js::undefined>>("get", key);
	}
	template<typename K, typename V>
	inline auto _impl_js_jMap<K, V>::has(K key) noexcept {
		return this->template _call<bool /*false*/>("has", key);
	}
	template<typename K, typename V>
	inline auto _impl_js_jMap<K, V>::keys() noexcept {
		return this->template _call<_js_jIterableIterator<K>>("keys");
	}
	template<typename K, typename V>
	inline auto _impl_js_jMap<K, V>::set(K key, V value) noexcept {
		return this->template _call<_js_jMap>("set", key, value);
	}
	template<typename K, typename V>
	inline auto _impl_js_jMap<K, V>::values() noexcept {
		return this->template _call<_js_jIterableIterator<V>>("values");
	}
	inline auto _impl_js_jMapConstructor::prototype() noexcept { return this->template _getProperty<_js_jMap<tc::js::any, tc::js::any>>("prototype"); }
	inline auto _impl_js_jMath::E() noexcept { return this->template _getProperty<double>("E"); }
	inline auto _impl_js_jMath::LN10() noexcept { return this->template _getProperty<double>("LN10"); }
	inline auto _impl_js_jMath::LN2() noexcept { return this->template _getProperty<double>("LN2"); }
	inline auto _impl_js_jMath::LOG2E() noexcept { return this->template _getProperty<double>("LOG2E"); }
	inline auto _impl_js_jMath::LOG10E() noexcept { return this->template _getProperty<double>("LOG10E"); }
	inline auto _impl_js_jMath::PI() noexcept { return this->template _getProperty<double>("PI"); }
	inline auto _impl_js_jMath::SQRT1_2() noexcept { return this->template _getProperty<double>("SQRT1_2"); }
	inline auto _impl_js_jMath::SQRT2() noexcept { return this->template _getProperty<double>("SQRT2"); }
	inline auto _impl_js_jMath::abs(double x) noexcept {
		return this->template _call<double>("abs", x);
	}
	inline auto _impl_js_jMath::acos(double x) noexcept {
		return this->template _call<double>("acos", x);
	}
	inline auto _impl_js_jMath::acosh(double x) noexcept {
		return this->template _call<double>("acosh", x);
	}
	inline auto _impl_js_jMath::asin(double x) noexcept {
		return this->template _call<double>("asin", x);
	}
	inline auto _impl_js_jMath::asinh(double x) noexcept {
		return this->template _call<double>("asinh", x);
	}
	inline auto _impl_js_jMath::atan(double x) noexcept {
		return this->template _call<double>("atan", x);
	}
	inline auto _impl_js_jMath::atan2(double y, double x) noexcept {
		return this->template _call<double>("atan2", y, x);
	}
	inline auto _impl_js_jMath::atanh(double x) noexcept {
		return this->template _call<double>("atanh", x);
	}
	inline auto _impl_js_jMath::cbrt(double x) noexcept {
		return this->template _call<double>("cbrt", x);
	}
	inline auto _impl_js_jMath::ceil(double x) noexcept {
		return this->template _call<double>("ceil", x);
	}
	inline auto _impl_js_jMath::clz32(double x) noexcept {
		return this->template _call<double>("clz32", x);
	}
	inline auto _impl_js_jMath::cos(double x) noexcept {
		return this->template _call<double>("cos", x);
	}
	inline auto _impl_js_jMath::cosh(double x) noexcept {
		return this->template _call<double>("cosh", x);
	}
	inline auto _impl_js_jMath::exp(double x) noexcept {
		return this->template _call<double>("exp", x);
	}
	inline auto _impl_js_jMath::expm1(double x) noexcept {
		return this->template _call<double>("expm1", x);
	}
	inline auto _impl_js_jMath::floor(double x) noexcept {
		return this->template _call<double>("floor", x);
	}
	inline auto _impl_js_jMath::fround(double x) noexcept {
		return this->template _call<double>("fround", x);
	}
	inline auto _impl_js_jMath::hypot(tc::jst::variadic_arg_t, double values) noexcept {
		return this->template _call<double>("hypot", values);
	}
	inline auto _impl_js_jMath::imul(double x, double y) noexcept {
		return this->template _call<double>("imul", x, y);
	}
	inline auto _impl_js_jMath::log(double x) noexcept {
		return this->template _call<double>("log", x);
	}
	inline auto _impl_js_jMath::log10(double x) noexcept {
		return this->template _call<double>("log10", x);
	}
	inline auto _impl_js_jMath::log1p(double x) noexcept {
		return this->template _call<double>("log1p", x);
	}
	inline auto _impl_js_jMath::log2(double x) noexcept {
		return this->template _call<double>("log2", x);
	}
	inline auto _impl_js_jMath::max(tc::jst::variadic_arg_t, double values) noexcept {
		return this->template _call<double>("max", values);
	}
	inline auto _impl_js_jMath::min(tc::jst::variadic_arg_t, double values) noexcept {
		return this->template _call<double>("min", values);
	}
	inline auto _impl_js_jMath::pow(double x, double y) noexcept {
		return this->template _call<double>("pow", x, y);
	}
	inline auto _impl_js_jMath::random() noexcept {
		return this->template _call<double>("random");
	}
	inline auto _impl_js_jMath::round(double x) noexcept {
		return this->template _call<double>("round", x);
	}
	inline auto _impl_js_jMath::sign(double x) noexcept {
		return this->template _call<double>("sign", x);
	}
	inline auto _impl_js_jMath::sin(double x) noexcept {
		return this->template _call<double>("sin", x);
	}
	inline auto _impl_js_jMath::sinh(double x) noexcept {
		return this->template _call<double>("sinh", x);
	}
	inline auto _impl_js_jMath::sqrt(double x) noexcept {
		return this->template _call<double>("sqrt", x);
	}
	inline auto _impl_js_jMath::tan(double x) noexcept {
		return this->template _call<double>("tan", x);
	}
	inline auto _impl_js_jMath::tanh(double x) noexcept {
		return this->template _call<double>("tanh", x);
	}
	inline auto _impl_js_jMath::trunc(double x) noexcept {
		return this->template _call<double>("trunc", x);
	}
	template<typename T>
	inline auto _impl_js_jNewableFunction::apply(T thisArg) noexcept {
		 this->template _call<void>("apply", thisArg);
	}
	template<typename T, typename A>
	inline auto _impl_js_jNewableFunction::apply(T thisArg, A args) noexcept {
		 this->template _call<void>("apply", thisArg, args);
	}
	template<typename T>
	inline auto _impl_js_jNewableFunction::bind(tc::js::any thisArg) noexcept {
		return this->template _call<T>("bind", thisArg);
	}
	template<typename A0, typename A, typename R>
	inline auto _impl_js_jNewableFunction::bind(tc::js::any thisArg, A0 arg0) noexcept {
		return this->template _call<tc::js::any /*AnonymousTypeWithLiteralType=new (...args: A) => R[__new]*/>("bind", thisArg, arg0);
	}
	template<typename A0, typename A1, typename A, typename R>
	inline auto _impl_js_jNewableFunction::bind(tc::js::any thisArg, A0 arg0, A1 arg1) noexcept {
		return this->template _call<tc::js::any /*AnonymousTypeWithLiteralType=new (...args: A) => R[__new]*/>("bind", thisArg, arg0, arg1);
	}
	template<typename A0, typename A1, typename A2, typename A, typename R>
	inline auto _impl_js_jNewableFunction::bind(tc::js::any thisArg, A0 arg0, A1 arg1, A2 arg2) noexcept {
		return this->template _call<tc::js::any /*AnonymousTypeWithLiteralType=new (...args: A) => R[__new]*/>("bind", thisArg, arg0, arg1, arg2);
	}
	template<typename A0, typename A1, typename A2, typename A3, typename A, typename R>
	inline auto _impl_js_jNewableFunction::bind(tc::js::any thisArg, A0 arg0, A1 arg1, A2 arg2, A3 arg3) noexcept {
		return this->template _call<tc::js::any /*AnonymousTypeWithLiteralType=new (...args: A) => R[__new]*/>("bind", thisArg, arg0, arg1, arg2, arg3);
	}
	template<typename AX, typename R>
	inline auto _impl_js_jNewableFunction::bind(tc::js::any thisArg, tc::jst::variadic_arg_t, AX args) noexcept {
		return this->template _call<tc::js::any /*AnonymousTypeWithLiteralType=new (...args: AX[]) => R[__new]*/>("bind", thisArg, args);
	}
	template<typename T, typename A>
	inline auto _impl_js_jNewableFunction::call(T thisArg, A args) noexcept {
		 this->template _call<void>("call", thisArg, args);
	}
	inline auto _impl_js_jNumber::_tcjs_construct(tc::js::any value) noexcept {
		return _js_jNumber(emscripten::val::global("Number").new_(value));
	}
	inline auto _impl_js_jNumber::toExponential(tc::jst::union_t<double, tc::js::undefined> fractionDigits) noexcept {
		return this->template _call<tc::js::string>("toExponential", fractionDigits);
	}
	inline auto _impl_js_jNumber::toFixed(tc::jst::union_t<double, tc::js::undefined> fractionDigits) noexcept {
		return this->template _call<tc::js::string>("toFixed", fractionDigits);
	}
	inline auto _impl_js_jNumber::toLocaleString(tc::jst::union_t<_js_jArray<tc::js::string>, tc::js::string, tc::js::undefined> locales, tc::jst::union_t<_js_jIntl_dNumberFormatOptions, tc::js::undefined> options) noexcept {
		return this->template _call<tc::js::string>("toLocaleString", locales, options);
	}
	inline auto _impl_js_jNumber::toPrecision(tc::jst::union_t<double, tc::js::undefined> precision) noexcept {
		return this->template _call<tc::js::string>("toPrecision", precision);
	}
	inline auto _impl_js_jNumber::toString(tc::jst::union_t<double, tc::js::undefined> radix) noexcept {
		return this->template _call<tc::js::string>("toString", radix);
	}
	inline auto _impl_js_jNumber::valueOf() noexcept {
		return this->template _call<double>("valueOf");
	}
	inline auto _impl_js_jNumberConstructor::prototype() noexcept { return this->template _getProperty<_js_jNumber>("prototype"); }
	inline auto _impl_js_jNumberConstructor::MAX_VALUE() noexcept { return this->template _getProperty<double>("MAX_VALUE"); }
	inline auto _impl_js_jNumberConstructor::MIN_VALUE() noexcept { return this->template _getProperty<double>("MIN_VALUE"); }
	inline auto _impl_js_jNumberConstructor::NaN() noexcept { return this->template _getProperty<double>("NaN"); }
	inline auto _impl_js_jNumberConstructor::NEGATIVE_INFINITY() noexcept { return this->template _getProperty<double>("NEGATIVE_INFINITY"); }
	inline auto _impl_js_jNumberConstructor::POSITIVE_INFINITY() noexcept { return this->template _getProperty<double>("POSITIVE_INFINITY"); }
	inline auto _impl_js_jNumberConstructor::EPSILON() noexcept { return this->template _getProperty<double>("EPSILON"); }
	inline auto _impl_js_jNumberConstructor::MAX_SAFE_INTEGER() noexcept { return this->template _getProperty<double>("MAX_SAFE_INTEGER"); }
	inline auto _impl_js_jNumberConstructor::MIN_SAFE_INTEGER() noexcept { return this->template _getProperty<double>("MIN_SAFE_INTEGER"); }
	inline auto _impl_js_jNumberConstructor::isFinite(tc::js::any number) noexcept {
		return this->template _call<bool /*false*/>("isFinite", number);
	}
	inline auto _impl_js_jNumberConstructor::isInteger(tc::js::any number) noexcept {
		return this->template _call<bool /*false*/>("isInteger", number);
	}
	inline auto _impl_js_jNumberConstructor::isNaN(tc::js::any number) noexcept {
		return this->template _call<bool /*false*/>("isNaN", number);
	}
	inline auto _impl_js_jNumberConstructor::isSafeInteger(tc::js::any number) noexcept {
		return this->template _call<bool /*false*/>("isSafeInteger", number);
	}
	inline auto _impl_js_jNumberConstructor::parseFloat(tc::js::string string) noexcept {
		return this->template _call<double>("parseFloat", string);
	}
	inline auto _impl_js_jNumberConstructor::parseInt(tc::js::string string, tc::jst::union_t<double, tc::js::undefined> radix) noexcept {
		return this->template _call<double>("parseInt", string, radix);
	}
	inline auto _impl_js_jObject::constructor() noexcept { return this->template _getProperty<_js_jFunction>("constructor"); }
	inline void _impl_js_jObject::constructor(_js_jFunction v) noexcept { this->template _setProperty("constructor", v); }
	inline auto _impl_js_jObject::_tcjs_construct(tc::js::any value) noexcept {
		return _js_jObject(emscripten::val::global("Object").new_(value));
	}
	inline auto _impl_js_jObject::hasOwnProperty(tc::js::any /*UnionWithUnsupportedSubtype=[tc::js::string,double,tc::js::any]*/ v) noexcept {
		return this->template _call<bool /*false*/>("hasOwnProperty", v);
	}
	inline auto _impl_js_jObject::isPrototypeOf(_js_jObject v) noexcept {
		return this->template _call<bool /*false*/>("isPrototypeOf", v);
	}
	inline auto _impl_js_jObject::propertyIsEnumerable(tc::js::any /*UnionWithUnsupportedSubtype=[tc::js::string,double,tc::js::any]*/ v) noexcept {
		return this->template _call<bool /*false*/>("propertyIsEnumerable", v);
	}
	inline auto _impl_js_jObject::toLocaleString() noexcept {
		return this->template _call<tc::js::string>("toLocaleString");
	}
	inline auto _impl_js_jObject::toString() noexcept {
		return this->template _call<tc::js::string>("toString");
	}
	inline auto _impl_js_jObject::valueOf() noexcept {
		return this->template _call<_js_jObject>("valueOf");
	}
	inline auto _impl_js_jObjectConstructor::prototype() noexcept { return this->template _getProperty<_js_jObject>("prototype"); }
	template<typename T, typename U>
	inline auto _impl_js_jObjectConstructor::assign(T target, U source) noexcept {
		return this->template _call<tc::js::any /* unsupported type flags =2097152: T & U*/>("assign", target, source);
	}
	template<typename T, typename U, typename V>
	inline auto _impl_js_jObjectConstructor::assign(T target, U source1, V source2) noexcept {
		return this->template _call<tc::js::any /* unsupported type flags =2097152: T & U & V*/>("assign", target, source1, source2);
	}
	template<typename T, typename U, typename V, typename W>
	inline auto _impl_js_jObjectConstructor::assign(T target, U source1, V source2, W source3) noexcept {
		return this->template _call<tc::js::any /* unsupported type flags =2097152: T & U & V & W*/>("assign", target, source1, source2, source3);
	}
	inline auto _impl_js_jObjectConstructor::assign(tc::js::any /* unsupported type flags =67108864: object*/ target, tc::jst::variadic_arg_t, tc::js::any sources) noexcept {
		return this->template _call<tc::js::any>("assign", target, sources);
	}
	inline auto _impl_js_jObjectConstructor::create(tc::js::any /*UnionWithUnsupportedSubtype=[tc::js::null,tc::js::any]*/ o) noexcept {
		return this->template _call<tc::js::any>("create", o);
	}
	inline auto _impl_js_jObjectConstructor::create(tc::js::any /*UnionWithUnsupportedSubtype=[tc::js::null,tc::js::any]*/ o, tc::js::any /* unsupported type flags =2097152: PropertyDescriptorMap & ThisType<any>*/ properties) noexcept {
		return this->template _call<tc::js::any>("create", o, properties);
	}
	template<typename T>
	inline auto _impl_js_jObjectConstructor::defineProperties(T o, tc::js::any /* unsupported type flags =2097152: PropertyDescriptorMap & ThisType<any>*/ properties) noexcept {
		return this->template _call<T>("defineProperties", o, properties);
	}
	template<typename T>
	inline auto _impl_js_jObjectConstructor::defineProperty(T o, tc::js::any /*UnionWithUnsupportedSubtype=[tc::js::string,double,tc::js::any]*/ p, tc::js::any /* unsupported type flags =2097152: PropertyDescriptor & ThisType<any>*/ attributes) noexcept {
		return this->template _call<T>("defineProperty", o, p, attributes);
	}
	template<typename T>
	inline auto _impl_js_jObjectConstructor::entries(tc::js::any /*UnionWithUnsupportedSubtype=[tc::js::any,_js_jArrayLike<T>]*/ o) noexcept {
		return this->template _call<_js_jArray<std::tuple<tc::js::string, T>>>("entries", o);
	}
	template<typename T>
	inline auto _impl_js_jObjectConstructor::freeze(T f) noexcept {
		return this->template _call<T>("freeze", f);
	}
	template<typename T>
	inline auto _impl_js_jObjectConstructor::freeze(_js_jArray<T> a) noexcept {
		return this->template _call<_js_jReadonlyArray<T>>("freeze", a);
	}
	inline auto _impl_js_jObjectConstructor::getOwnPropertyDescriptor(tc::js::any o, tc::js::any /*UnionWithUnsupportedSubtype=[tc::js::string,double,tc::js::any]*/ p) noexcept {
		return this->template _call<tc::jst::union_t<_js_jPropertyDescriptor, tc::js::undefined>>("getOwnPropertyDescriptor", o, p);
	}
	template<typename T>
	inline auto _impl_js_jObjectConstructor::getOwnPropertyDescriptors(T o) noexcept {
		return this->template _call<tc::js::any /* unsupported type flags =2097152: { [P in keyof T]: TypedPropertyDescriptor<T[P]>; } & { [x: string]: PropertyDescriptor; }*/>("getOwnPropertyDescriptors", o);
	}
	inline auto _impl_js_jObjectConstructor::getOwnPropertyNames(tc::js::any o) noexcept {
		return this->template _call<_js_jArray<tc::js::string>>("getOwnPropertyNames", o);
	}
	inline auto _impl_js_jObjectConstructor::getOwnPropertySymbols(tc::js::any o) noexcept {
		return this->template _call<_js_jArray<tc::js::any /* unsupported type flags =4096: symbol*/>>("getOwnPropertySymbols", o);
	}
	inline auto _impl_js_jObjectConstructor::getPrototypeOf(tc::js::any o) noexcept {
		return this->template _call<tc::js::any>("getPrototypeOf", o);
	}
	inline auto _impl_js_jObjectConstructor::is(tc::js::any value1, tc::js::any value2) noexcept {
		return this->template _call<bool /*false*/>("is", value1, value2);
	}
	inline auto _impl_js_jObjectConstructor::isExtensible(tc::js::any o) noexcept {
		return this->template _call<bool /*false*/>("isExtensible", o);
	}
	inline auto _impl_js_jObjectConstructor::isFrozen(tc::js::any o) noexcept {
		return this->template _call<bool /*false*/>("isFrozen", o);
	}
	inline auto _impl_js_jObjectConstructor::isSealed(tc::js::any o) noexcept {
		return this->template _call<bool /*false*/>("isSealed", o);
	}
	inline auto _impl_js_jObjectConstructor::keys(tc::js::any /*AnonymousType={}*/ o) noexcept {
		return this->template _call<_js_jArray<tc::js::string>>("keys", o);
	}
	template<typename T>
	inline auto _impl_js_jObjectConstructor::preventExtensions(T o) noexcept {
		return this->template _call<T>("preventExtensions", o);
	}
	template<typename T>
	inline auto _impl_js_jObjectConstructor::seal(T o) noexcept {
		return this->template _call<T>("seal", o);
	}
	inline auto _impl_js_jObjectConstructor::setPrototypeOf(tc::js::any o, tc::js::any /*UnionWithUnsupportedSubtype=[tc::js::null,tc::js::any]*/ proto) noexcept {
		return this->template _call<tc::js::any>("setPrototypeOf", o, proto);
	}
	inline auto _impl_js_jObjectConstructor::values(tc::js::any /*AnonymousType={}*/ o) noexcept {
		return this->template _call<_js_jArray<tc::js::any>>("values", o);
	}
	inline auto _impl_js_jPromiseConstructor::prototype() noexcept { return this->template _getProperty<_js_jPromise<tc::js::any>>("prototype"); }
	template<typename T>
	inline auto _impl_js_jPromiseConstructor::all(_js_jIterable<tc::jst::union_t<T, _js_jPromiseLike<T>>> values) noexcept {
		return this->template _call<_js_jPromise<_js_jArray<T>>>("all", values);
	}
	template<typename T>
	inline auto _impl_js_jPromiseConstructor::all(_js_jReadonlyArray<tc::jst::union_t<T, _js_jPromiseLike<T>>> values) noexcept {
		return this->template _call<_js_jPromise<_js_jArray<T>>>("all", values);
	}
	template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10>
	inline auto _impl_js_jPromiseConstructor::all(std::tuple<tc::jst::union_t<T1, _js_jPromiseLike<T1>>, tc::jst::union_t<T2, _js_jPromiseLike<T2>>, tc::jst::union_t<T3, _js_jPromiseLike<T3>>, tc::jst::union_t<T4, _js_jPromiseLike<T4>>, tc::jst::union_t<T5, _js_jPromiseLike<T5>>, tc::jst::union_t<T6, _js_jPromiseLike<T6>>, tc::jst::union_t<T7, _js_jPromiseLike<T7>>, tc::jst::union_t<T8, _js_jPromiseLike<T8>>, tc::jst::union_t<T9, _js_jPromiseLike<T9>>, tc::jst::union_t<T10, _js_jPromiseLike<T10>>> values) noexcept {
		return this->template _call<_js_jPromise<std::tuple<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10>>>("all", values);
	}
	template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9>
	inline auto _impl_js_jPromiseConstructor::all(std::tuple<tc::jst::union_t<T1, _js_jPromiseLike<T1>>, tc::jst::union_t<T2, _js_jPromiseLike<T2>>, tc::jst::union_t<T3, _js_jPromiseLike<T3>>, tc::jst::union_t<T4, _js_jPromiseLike<T4>>, tc::jst::union_t<T5, _js_jPromiseLike<T5>>, tc::jst::union_t<T6, _js_jPromiseLike<T6>>, tc::jst::union_t<T7, _js_jPromiseLike<T7>>, tc::jst::union_t<T8, _js_jPromiseLike<T8>>, tc::jst::union_t<T9, _js_jPromiseLike<T9>>> values) noexcept {
		return this->template _call<_js_jPromise<std::tuple<T1, T2, T3, T4, T5, T6, T7, T8, T9>>>("all", values);
	}
	template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
	inline auto _impl_js_jPromiseConstructor::all(std::tuple<tc::jst::union_t<T1, _js_jPromiseLike<T1>>, tc::jst::union_t<T2, _js_jPromiseLike<T2>>, tc::jst::union_t<T3, _js_jPromiseLike<T3>>, tc::jst::union_t<T4, _js_jPromiseLike<T4>>, tc::jst::union_t<T5, _js_jPromiseLike<T5>>, tc::jst::union_t<T6, _js_jPromiseLike<T6>>, tc::jst::union_t<T7, _js_jPromiseLike<T7>>, tc::jst::union_t<T8, _js_jPromiseLike<T8>>> values) noexcept {
		return this->template _call<_js_jPromise<std::tuple<T1, T2, T3, T4, T5, T6, T7, T8>>>("all", values);
	}
	template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
	inline auto _impl_js_jPromiseConstructor::all(std::tuple<tc::jst::union_t<T1, _js_jPromiseLike<T1>>, tc::jst::union_t<T2, _js_jPromiseLike<T2>>, tc::jst::union_t<T3, _js_jPromiseLike<T3>>, tc::jst::union_t<T4, _js_jPromiseLike<T4>>, tc::jst::union_t<T5, _js_jPromiseLike<T5>>, tc::jst::union_t<T6, _js_jPromiseLike<T6>>, tc::jst::union_t<T7, _js_jPromiseLike<T7>>> values) noexcept {
		return this->template _call<_js_jPromise<std::tuple<T1, T2, T3, T4, T5, T6, T7>>>("all", values);
	}
	template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
	inline auto _impl_js_jPromiseConstructor::all(std::tuple<tc::jst::union_t<T1, _js_jPromiseLike<T1>>, tc::jst::union_t<T2, _js_jPromiseLike<T2>>, tc::jst::union_t<T3, _js_jPromiseLike<T3>>, tc::jst::union_t<T4, _js_jPromiseLike<T4>>, tc::jst::union_t<T5, _js_jPromiseLike<T5>>, tc::jst::union_t<T6, _js_jPromiseLike<T6>>> values) noexcept {
		return this->template _call<_js_jPromise<std::tuple<T1, T2, T3, T4, T5, T6>>>("all", values);
	}
	template<typename T1, typename T2, typename T3, typename T4, typename T5>
	inline auto _impl_js_jPromiseConstructor::all(std::tuple<tc::jst::union_t<T1, _js_jPromiseLike<T1>>, tc::jst::union_t<T2, _js_jPromiseLike<T2>>, tc::jst::union_t<T3, _js_jPromiseLike<T3>>, tc::jst::union_t<T4, _js_jPromiseLike<T4>>, tc::jst::union_t<T5, _js_jPromiseLike<T5>>> values) noexcept {
		return this->template _call<_js_jPromise<std::tuple<T1, T2, T3, T4, T5>>>("all", values);
	}
	template<typename T1, typename T2, typename T3, typename T4>
	inline auto _impl_js_jPromiseConstructor::all(std::tuple<tc::jst::union_t<T1, _js_jPromiseLike<T1>>, tc::jst::union_t<T2, _js_jPromiseLike<T2>>, tc::jst::union_t<T3, _js_jPromiseLike<T3>>, tc::jst::union_t<T4, _js_jPromiseLike<T4>>> values) noexcept {
		return this->template _call<_js_jPromise<std::tuple<T1, T2, T3, T4>>>("all", values);
	}
	template<typename T1, typename T2, typename T3>
	inline auto _impl_js_jPromiseConstructor::all(std::tuple<tc::jst::union_t<T1, _js_jPromiseLike<T1>>, tc::jst::union_t<T2, _js_jPromiseLike<T2>>, tc::jst::union_t<T3, _js_jPromiseLike<T3>>> values) noexcept {
		return this->template _call<_js_jPromise<std::tuple<T1, T2, T3>>>("all", values);
	}
	template<typename T1, typename T2>
	inline auto _impl_js_jPromiseConstructor::all(std::tuple<tc::jst::union_t<T1, _js_jPromiseLike<T1>>, tc::jst::union_t<T2, _js_jPromiseLike<T2>>> values) noexcept {
		return this->template _call<_js_jPromise<std::tuple<T1, T2>>>("all", values);
	}
	template<typename T>
	inline auto _impl_js_jPromiseConstructor::race(_js_jIterable<T> values) noexcept {
		return this->template _call<_js_jPromise<tc::js::any /* unsupported type flags =16777216: T extends PromiseLike<infer U> ? U : T*/>>("race", values);
	}
	template<typename T>
	inline auto _impl_js_jPromiseConstructor::race(_js_jIterable<tc::jst::union_t<T, _js_jPromiseLike<T>>> values) noexcept {
		return this->template _call<_js_jPromise<T>>("race", values);
	}
	template<typename T>
	inline auto _impl_js_jPromiseConstructor::race(_js_jReadonlyArray<T> values) noexcept {
		return this->template _call<_js_jPromise<tc::js::any /* unsupported type flags =16777216: T extends PromiseLike<infer U> ? U : T*/>>("race", values);
	}
	template<typename T>
	inline auto _impl_js_jPromiseConstructor::reject(tc::js::any reason) noexcept {
		return this->template _call<_js_jPromise<T>>("reject", reason);
	}
	inline auto _impl_js_jPromiseConstructor::resolve() noexcept {
		return this->template _call<_js_jPromise<void>>("resolve");
	}
	template<typename T>
	inline auto _impl_js_jPromiseConstructor::resolve(tc::jst::union_t<T, _js_jPromiseLike<T>> value) noexcept {
		return this->template _call<_js_jPromise<T>>("resolve", value);
	}
	template<typename T>
	template<typename TResult1, typename TResult2>
	inline auto _impl_js_jPromiseLike<T>::then(tc::jst::union_t<tc::js::null, tc::js::undefined, tc::jst::function<tc::jst::union_t<TResult1, _js_jPromiseLike<TResult1>>(T)>> onfulfilled, tc::jst::union_t<tc::js::null, tc::js::undefined, tc::jst::function<tc::jst::union_t<TResult2, _js_jPromiseLike<TResult2>>(tc::js::any)>> onrejected) noexcept {
		return this->template _call<_js_jPromiseLike<tc::jst::union_t<TResult1, TResult2>>>("then", onfulfilled, onrejected);
	}
	inline auto _impl_js_jPropertyDescriptor::configurable() noexcept { return this->template _getProperty<tc::jst::union_t<bool /*false*/, tc::js::undefined>>("configurable"); }
	inline void _impl_js_jPropertyDescriptor::configurable(tc::jst::union_t<bool /*false*/, tc::js::undefined> v) noexcept { this->template _setProperty("configurable", v); }
	inline auto _impl_js_jPropertyDescriptor::enumerable() noexcept { return this->template _getProperty<tc::jst::union_t<bool /*false*/, tc::js::undefined>>("enumerable"); }
	inline void _impl_js_jPropertyDescriptor::enumerable(tc::jst::union_t<bool /*false*/, tc::js::undefined> v) noexcept { this->template _setProperty("enumerable", v); }
	inline auto _impl_js_jPropertyDescriptor::value() noexcept { return this->template _getProperty<tc::js::any>("value"); }
	inline void _impl_js_jPropertyDescriptor::value(tc::js::any v) noexcept { this->template _setProperty("value", v); }
	inline auto _impl_js_jPropertyDescriptor::writable() noexcept { return this->template _getProperty<tc::jst::union_t<bool /*false*/, tc::js::undefined>>("writable"); }
	inline void _impl_js_jPropertyDescriptor::writable(tc::jst::union_t<bool /*false*/, tc::js::undefined> v) noexcept { this->template _setProperty("writable", v); }
	inline auto _impl_js_jPropertyDescriptorMap::_tcjs_construct() noexcept {
		return emscripten::val::object();
	}
	inline auto _impl_js_jPropertyDescriptorMap::operator[](tc::js::string s) noexcept {
		return this->template _getProperty<_js_jPropertyDescriptor>(s);
	}
	template<typename T>
	inline auto _impl_js_jProxyConstructor::revocable(T target, _js_jProxyHandler<T> handler) noexcept {
		return this->template _call<tc::js::any /*AnonymousTypeWithLiteralType={ proxy: T; revoke: () => void; }[proxy, revoke]*/>("revocable", target, handler);
	}
	inline auto _impl_js_jRangeError::_tcjs_construct(tc::jst::union_t<tc::js::string, tc::js::undefined> message) noexcept {
		return _js_jRangeError(emscripten::val::global("RangeError").new_(message));
	}
	inline auto _impl_js_jRangeErrorConstructor::prototype() noexcept { return this->template _getProperty<_js_jRangeError>("prototype"); }
	template<typename T>
	inline auto _impl_js_jReadonlyArray<T>::length() noexcept { return this->template _getProperty<double>("length"); }
	template<typename T>
	inline auto _impl_js_jReadonlyArray<T>::concat(tc::jst::variadic_arg_t, _js_jConcatArray<T> items) noexcept {
		return this->template _call<_js_jArray<T>>("concat", items);
	}
	template<typename T>
	inline auto _impl_js_jReadonlyArray<T>::concat(tc::jst::variadic_arg_t, tc::jst::union_t<T, _js_jConcatArray<T>> items) noexcept {
		return this->template _call<_js_jArray<T>>("concat", items);
	}
	template<typename T>
	inline auto _impl_js_jReadonlyArray<T>::entries() noexcept {
		return this->template _call<_js_jIterableIterator<std::tuple<double, T>>>("entries");
	}
	template<typename T>
	template<typename S>
	inline auto _impl_js_jReadonlyArray<T>::every(tc::jst::function<bool /*false*/(T, double, _js_jReadonlyArray<T>)> predicate, tc::js::any thisArg) noexcept {
		std::optional<_js_jReadonlyArray<S>> result;
		if(this->template _call<bool /*false*/>("every", predicate, thisArg)) {
			result.emplace(_this<tc::js::any>());
		}
		return result;
	}
	template<typename T>
	inline auto _impl_js_jReadonlyArray<T>::every(tc::jst::function<tc::js::any(T, double, _js_jReadonlyArray<T>)> predicate, tc::js::any thisArg) noexcept {
		return this->template _call<bool /*false*/>("every", predicate, thisArg);
	}
	template<typename T>
	template<typename S>
	inline auto _impl_js_jReadonlyArray<T>::filter(tc::jst::function<bool /*false*/(T, double, _js_jReadonlyArray<T>)> predicate, tc::js::any thisArg) noexcept {
		return this->template _call<_js_jArray<S>>("filter", predicate, thisArg);
	}
	template<typename T>
	inline auto _impl_js_jReadonlyArray<T>::filter(tc::jst::function<tc::js::any(T, double, _js_jReadonlyArray<T>)> predicate, tc::js::any thisArg) noexcept {
		return this->template _call<_js_jArray<T>>("filter", predicate, thisArg);
	}
	template<typename T>
	template<typename S>
	inline auto _impl_js_jReadonlyArray<T>::find(tc::jst::function<bool /*false*/(T, double, _js_jReadonlyArray<T>)> predicate, tc::js::any thisArg) noexcept {
		return this->template _call<tc::jst::union_t<S, tc::js::undefined>>("find", predicate, thisArg);
	}
	template<typename T>
	inline auto _impl_js_jReadonlyArray<T>::find(tc::jst::function<tc::js::any(T, double, _js_jReadonlyArray<T>)> predicate, tc::js::any thisArg) noexcept {
		return this->template _call<tc::jst::union_t<T, tc::js::undefined>>("find", predicate, thisArg);
	}
	template<typename T>
	inline auto _impl_js_jReadonlyArray<T>::findIndex(tc::jst::function<tc::js::any(T, double, _js_jReadonlyArray<T>)> predicate, tc::js::any thisArg) noexcept {
		return this->template _call<double>("findIndex", predicate, thisArg);
	}
	template<typename T>
	inline auto _impl_js_jReadonlyArray<T>::forEach(tc::jst::function<void(T, double, _js_jReadonlyArray<T>)> callbackfn, tc::js::any thisArg) noexcept {
		 this->template _call<void>("forEach", callbackfn, thisArg);
	}
	template<typename T>
	inline auto _impl_js_jReadonlyArray<T>::includes(T searchElement, tc::jst::union_t<double, tc::js::undefined> fromIndex) noexcept {
		return this->template _call<bool /*false*/>("includes", searchElement, fromIndex);
	}
	template<typename T>
	inline auto _impl_js_jReadonlyArray<T>::indexOf(T searchElement, tc::jst::union_t<double, tc::js::undefined> fromIndex) noexcept {
		return this->template _call<double>("indexOf", searchElement, fromIndex);
	}
	template<typename T>
	inline auto _impl_js_jReadonlyArray<T>::join(tc::jst::union_t<tc::js::string, tc::js::undefined> separator) noexcept {
		return this->template _call<tc::js::string>("join", separator);
	}
	template<typename T>
	inline auto _impl_js_jReadonlyArray<T>::keys() noexcept {
		return this->template _call<_js_jIterableIterator<double>>("keys");
	}
	template<typename T>
	inline auto _impl_js_jReadonlyArray<T>::lastIndexOf(T searchElement, tc::jst::union_t<double, tc::js::undefined> fromIndex) noexcept {
		return this->template _call<double>("lastIndexOf", searchElement, fromIndex);
	}
	template<typename T>
	template<typename U>
	inline auto _impl_js_jReadonlyArray<T>::map(tc::jst::function<U(T, double, _js_jReadonlyArray<T>)> callbackfn, tc::js::any thisArg) noexcept {
		return this->template _call<_js_jArray<U>>("map", callbackfn, thisArg);
	}
	template<typename T>
	inline auto _impl_js_jReadonlyArray<T>::operator[](double n) noexcept {
		return this->template _getProperty<T>(n);
	}
	template<typename T>
	inline auto _impl_js_jReadonlyArray<T>::reduce(tc::jst::function<T(T, T, double, _js_jReadonlyArray<T>)> callbackfn) noexcept {
		return this->template _call<T>("reduce", callbackfn);
	}
	template<typename T>
	inline auto _impl_js_jReadonlyArray<T>::reduce(tc::jst::function<T(T, T, double, _js_jReadonlyArray<T>)> callbackfn, T initialValue) noexcept {
		return this->template _call<T>("reduce", callbackfn, initialValue);
	}
	template<typename T>
	template<typename U>
	inline auto _impl_js_jReadonlyArray<T>::reduce(tc::jst::function<U(U, T, double, _js_jReadonlyArray<T>)> callbackfn, U initialValue) noexcept {
		return this->template _call<U>("reduce", callbackfn, initialValue);
	}
	template<typename T>
	inline auto _impl_js_jReadonlyArray<T>::reduceRight(tc::jst::function<T(T, T, double, _js_jReadonlyArray<T>)> callbackfn) noexcept {
		return this->template _call<T>("reduceRight", callbackfn);
	}
	template<typename T>
	inline auto _impl_js_jReadonlyArray<T>::reduceRight(tc::jst::function<T(T, T, double, _js_jReadonlyArray<T>)> callbackfn, T initialValue) noexcept {
		return this->template _call<T>("reduceRight", callbackfn, initialValue);
	}
	template<typename T>
	template<typename U>
	inline auto _impl_js_jReadonlyArray<T>::reduceRight(tc::jst::function<U(U, T, double, _js_jReadonlyArray<T>)> callbackfn, U initialValue) noexcept {
		return this->template _call<U>("reduceRight", callbackfn, initialValue);
	}
	template<typename T>
	inline auto _impl_js_jReadonlyArray<T>::slice(tc::jst::union_t<double, tc::js::undefined> start, tc::jst::union_t<double, tc::js::undefined> end) noexcept {
		return this->template _call<_js_jArray<T>>("slice", start, end);
	}
	template<typename T>
	inline auto _impl_js_jReadonlyArray<T>::some(tc::jst::function<tc::js::any(T, double, _js_jReadonlyArray<T>)> predicate, tc::js::any thisArg) noexcept {
		return this->template _call<bool /*false*/>("some", predicate, thisArg);
	}
	template<typename T>
	inline auto _impl_js_jReadonlyArray<T>::toLocaleString() noexcept {
		return this->template _call<tc::js::string>("toLocaleString");
	}
	template<typename T>
	inline auto _impl_js_jReadonlyArray<T>::toString() noexcept {
		return this->template _call<tc::js::string>("toString");
	}
	template<typename T>
	inline auto _impl_js_jReadonlyArray<T>::values() noexcept {
		return this->template _call<_js_jIterableIterator<T>>("values");
	}
	template<typename K, typename V>
	inline auto _impl_js_jReadonlyMap<K, V>::size() noexcept { return this->template _getProperty<double>("size"); }
	template<typename K, typename V>
	inline auto _impl_js_jReadonlyMap<K, V>::entries() noexcept {
		return this->template _call<_js_jIterableIterator<std::tuple<K, V>>>("entries");
	}
	template<typename K, typename V>
	inline auto _impl_js_jReadonlyMap<K, V>::forEach(tc::jst::function<void(V, K, _js_jReadonlyMap<K, V>)> callbackfn, tc::js::any thisArg) noexcept {
		 this->template _call<void>("forEach", callbackfn, thisArg);
	}
	template<typename K, typename V>
	inline auto _impl_js_jReadonlyMap<K, V>::get(K key) noexcept {
		return this->template _call<tc::jst::union_t<V, tc::js::undefined>>("get", key);
	}
	template<typename K, typename V>
	inline auto _impl_js_jReadonlyMap<K, V>::has(K key) noexcept {
		return this->template _call<bool /*false*/>("has", key);
	}
	template<typename K, typename V>
	inline auto _impl_js_jReadonlyMap<K, V>::keys() noexcept {
		return this->template _call<_js_jIterableIterator<K>>("keys");
	}
	template<typename K, typename V>
	inline auto _impl_js_jReadonlyMap<K, V>::values() noexcept {
		return this->template _call<_js_jIterableIterator<V>>("values");
	}
	template<typename T>
	inline auto _impl_js_jReadonlySet<T>::size() noexcept { return this->template _getProperty<double>("size"); }
	template<typename T>
	inline auto _impl_js_jReadonlySet<T>::entries() noexcept {
		return this->template _call<_js_jIterableIterator<std::tuple<T, T>>>("entries");
	}
	template<typename T>
	inline auto _impl_js_jReadonlySet<T>::forEach(tc::jst::function<void(T, T, _js_jReadonlySet<T>)> callbackfn, tc::js::any thisArg) noexcept {
		 this->template _call<void>("forEach", callbackfn, thisArg);
	}
	template<typename T>
	inline auto _impl_js_jReadonlySet<T>::has(T value) noexcept {
		return this->template _call<bool /*false*/>("has", value);
	}
	template<typename T>
	inline auto _impl_js_jReadonlySet<T>::keys() noexcept {
		return this->template _call<_js_jIterableIterator<T>>("keys");
	}
	template<typename T>
	inline auto _impl_js_jReadonlySet<T>::values() noexcept {
		return this->template _call<_js_jIterableIterator<T>>("values");
	}
	inline auto _impl_js_jReferenceError::_tcjs_construct(tc::jst::union_t<tc::js::string, tc::js::undefined> message) noexcept {
		return _js_jReferenceError(emscripten::val::global("ReferenceError").new_(message));
	}
	inline auto _impl_js_jReferenceErrorConstructor::prototype() noexcept { return this->template _getProperty<_js_jReferenceError>("prototype"); }
	inline auto _impl_js_jReflect::_tcjs_definitions::apply(_js_jFunction target, tc::js::any thisArgument, _js_jArrayLike<tc::js::any> argumentsList) noexcept {
		return emscripten::val::global("Reflect")["apply"](target, thisArgument, argumentsList).template as<tc::js::any>();
	}
	inline auto _impl_js_jReflect::_tcjs_definitions::construct(_js_jFunction target, _js_jArrayLike<tc::js::any> argumentsList, tc::jst::union_t<_js_jFunction, tc::js::undefined> newTarget) noexcept {
		return emscripten::val::global("Reflect")["construct"](target, argumentsList, newTarget).template as<tc::js::any>();
	}
	inline auto _impl_js_jReflect::_tcjs_definitions::defineProperty(tc::js::any /* unsupported type flags =67108864: object*/ target, tc::js::any /*UnionWithUnsupportedSubtype=[tc::js::string,double,tc::js::any]*/ propertyKey, _js_jPropertyDescriptor attributes) noexcept {
		return emscripten::val::global("Reflect")["defineProperty"](target, propertyKey, attributes).template as<bool /*false*/>();
	}
	inline auto _impl_js_jReflect::_tcjs_definitions::deleteProperty(tc::js::any /* unsupported type flags =67108864: object*/ target, tc::js::any /*UnionWithUnsupportedSubtype=[tc::js::string,double,tc::js::any]*/ propertyKey) noexcept {
		return emscripten::val::global("Reflect")["deleteProperty"](target, propertyKey).template as<bool /*false*/>();
	}
	inline auto _impl_js_jReflect::_tcjs_definitions::get(tc::js::any /* unsupported type flags =67108864: object*/ target, tc::js::any /*UnionWithUnsupportedSubtype=[tc::js::string,double,tc::js::any]*/ propertyKey, tc::js::any receiver) noexcept {
		return emscripten::val::global("Reflect")["get"](target, propertyKey, receiver).template as<tc::js::any>();
	}
	inline auto _impl_js_jReflect::_tcjs_definitions::getOwnPropertyDescriptor(tc::js::any /* unsupported type flags =67108864: object*/ target, tc::js::any /*UnionWithUnsupportedSubtype=[tc::js::string,double,tc::js::any]*/ propertyKey) noexcept {
		return emscripten::val::global("Reflect")["getOwnPropertyDescriptor"](target, propertyKey).template as<tc::jst::union_t<_js_jPropertyDescriptor, tc::js::undefined>>();
	}
	inline auto _impl_js_jReflect::_tcjs_definitions::getPrototypeOf(tc::js::any /* unsupported type flags =67108864: object*/ target) noexcept {
		return emscripten::val::global("Reflect")["getPrototypeOf"](target).template as<tc::js::any /*UnionWithUnsupportedSubtype=[tc::js::null,tc::js::any]*/>();
	}
	inline auto _impl_js_jReflect::_tcjs_definitions::has(tc::js::any /* unsupported type flags =67108864: object*/ target, tc::js::any /*UnionWithUnsupportedSubtype=[tc::js::string,double,tc::js::any]*/ propertyKey) noexcept {
		return emscripten::val::global("Reflect")["has"](target, propertyKey).template as<bool /*false*/>();
	}
	inline auto _impl_js_jReflect::_tcjs_definitions::isExtensible(tc::js::any /* unsupported type flags =67108864: object*/ target) noexcept {
		return emscripten::val::global("Reflect")["isExtensible"](target).template as<bool /*false*/>();
	}
	inline auto _impl_js_jReflect::_tcjs_definitions::ownKeys(tc::js::any /* unsupported type flags =67108864: object*/ target) noexcept {
		return emscripten::val::global("Reflect")["ownKeys"](target).template as<_js_jArray<tc::js::any /*UnionWithUnsupportedSubtype=[tc::js::string,tc::js::any]*/>>();
	}
	inline auto _impl_js_jReflect::_tcjs_definitions::preventExtensions(tc::js::any /* unsupported type flags =67108864: object*/ target) noexcept {
		return emscripten::val::global("Reflect")["preventExtensions"](target).template as<bool /*false*/>();
	}
	inline auto _impl_js_jReflect::_tcjs_definitions::set(tc::js::any /* unsupported type flags =67108864: object*/ target, tc::js::any /*UnionWithUnsupportedSubtype=[tc::js::string,double,tc::js::any]*/ propertyKey, tc::js::any value, tc::js::any receiver) noexcept {
		return emscripten::val::global("Reflect")["set"](target, propertyKey, value, receiver).template as<bool /*false*/>();
	}
	inline auto _impl_js_jReflect::_tcjs_definitions::setPrototypeOf(tc::js::any /* unsupported type flags =67108864: object*/ target, tc::js::any /*UnionWithUnsupportedSubtype=[tc::js::null,tc::js::any]*/ proto) noexcept {
		return emscripten::val::global("Reflect")["setPrototypeOf"](target, proto).template as<bool /*false*/>();
	}
	inline auto _impl_js_jRegExp::dotAll() noexcept { return this->template _getProperty<bool /*false*/>("dotAll"); }
	inline auto _impl_js_jRegExp::source() noexcept { return this->template _getProperty<tc::js::string>("source"); }
	inline auto _impl_js_jRegExp::global() noexcept { return this->template _getProperty<bool /*false*/>("global"); }
	inline auto _impl_js_jRegExp::ignoreCase() noexcept { return this->template _getProperty<bool /*false*/>("ignoreCase"); }
	inline auto _impl_js_jRegExp::multiline() noexcept { return this->template _getProperty<bool /*false*/>("multiline"); }
	inline auto _impl_js_jRegExp::lastIndex() noexcept { return this->template _getProperty<double>("lastIndex"); }
	inline void _impl_js_jRegExp::lastIndex(double v) noexcept { this->template _setProperty("lastIndex", v); }
	inline auto _impl_js_jRegExp::flags() noexcept { return this->template _getProperty<tc::js::string>("flags"); }
	inline auto _impl_js_jRegExp::sticky() noexcept { return this->template _getProperty<bool /*false*/>("sticky"); }
	inline auto _impl_js_jRegExp::unicode() noexcept { return this->template _getProperty<bool /*false*/>("unicode"); }
	inline auto _impl_js_jRegExp::_tcjs_construct(tc::jst::union_t<_js_jRegExp, tc::js::string> pattern) noexcept {
		return _js_jRegExp(emscripten::val::global("RegExp").new_(pattern));
	}
	inline auto _impl_js_jRegExp::_tcjs_construct(tc::js::string pattern, tc::jst::union_t<tc::js::string, tc::js::undefined> flags) noexcept {
		return _js_jRegExp(emscripten::val::global("RegExp").new_(pattern, flags));
	}
	inline auto _impl_js_jRegExp::_tcjs_construct(tc::jst::union_t<_js_jRegExp, tc::js::string> pattern, tc::jst::union_t<tc::js::string, tc::js::undefined> flags) noexcept {
		return _js_jRegExp(emscripten::val::global("RegExp").new_(pattern, flags));
	}
	inline auto _impl_js_jRegExp::compile() noexcept {
		return this->template _call<_js_jRegExp>("compile");
	}
	inline auto _impl_js_jRegExp::exec(tc::js::string string) noexcept {
		return this->template _call<tc::jst::union_t<_js_jRegExpExecArray, tc::js::null>>("exec", string);
	}
	inline auto _impl_js_jRegExp::test(tc::js::string string) noexcept {
		return this->template _call<bool /*false*/>("test", string);
	}
	inline auto _impl_js_jRegExpConstructor::prototype() noexcept { return this->template _getProperty<_js_jRegExp>("prototype"); }
	inline auto _impl_js_jRegExpConstructor::_1() noexcept { return this->template _getProperty<tc::js::string>("$1"); }
	inline void _impl_js_jRegExpConstructor::_1(tc::js::string v) noexcept { this->template _setProperty("$1", v); }
	inline auto _impl_js_jRegExpConstructor::_2() noexcept { return this->template _getProperty<tc::js::string>("$2"); }
	inline void _impl_js_jRegExpConstructor::_2(tc::js::string v) noexcept { this->template _setProperty("$2", v); }
	inline auto _impl_js_jRegExpConstructor::_3() noexcept { return this->template _getProperty<tc::js::string>("$3"); }
	inline void _impl_js_jRegExpConstructor::_3(tc::js::string v) noexcept { this->template _setProperty("$3", v); }
	inline auto _impl_js_jRegExpConstructor::_4() noexcept { return this->template _getProperty<tc::js::string>("$4"); }
	inline void _impl_js_jRegExpConstructor::_4(tc::js::string v) noexcept { this->template _setProperty("$4", v); }
	inline auto _impl_js_jRegExpConstructor::_5() noexcept { return this->template _getProperty<tc::js::string>("$5"); }
	inline void _impl_js_jRegExpConstructor::_5(tc::js::string v) noexcept { this->template _setProperty("$5", v); }
	inline auto _impl_js_jRegExpConstructor::_6() noexcept { return this->template _getProperty<tc::js::string>("$6"); }
	inline void _impl_js_jRegExpConstructor::_6(tc::js::string v) noexcept { this->template _setProperty("$6", v); }
	inline auto _impl_js_jRegExpConstructor::_7() noexcept { return this->template _getProperty<tc::js::string>("$7"); }
	inline void _impl_js_jRegExpConstructor::_7(tc::js::string v) noexcept { this->template _setProperty("$7", v); }
	inline auto _impl_js_jRegExpConstructor::_8() noexcept { return this->template _getProperty<tc::js::string>("$8"); }
	inline void _impl_js_jRegExpConstructor::_8(tc::js::string v) noexcept { this->template _setProperty("$8", v); }
	inline auto _impl_js_jRegExpConstructor::_9() noexcept { return this->template _getProperty<tc::js::string>("$9"); }
	inline void _impl_js_jRegExpConstructor::_9(tc::js::string v) noexcept { this->template _setProperty("$9", v); }
	inline auto _impl_js_jRegExpConstructor::lastMatch() noexcept { return this->template _getProperty<tc::js::string>("lastMatch"); }
	inline void _impl_js_jRegExpConstructor::lastMatch(tc::js::string v) noexcept { this->template _setProperty("lastMatch", v); }
	inline auto _impl_js_jRegExpExecArray::groups() noexcept { return this->template _getProperty<tc::js::any /*UnionWithUnsupportedSubtype=[tc::js::undefined,tc::js::any]*/>("groups"); }
	inline void _impl_js_jRegExpExecArray::groups(tc::js::any /*UnionWithUnsupportedSubtype=[tc::js::undefined,tc::js::any]*/ v) noexcept { this->template _setProperty("groups", v); }
	inline auto _impl_js_jRegExpExecArray::index() noexcept { return this->template _getProperty<double>("index"); }
	inline void _impl_js_jRegExpExecArray::index(double v) noexcept { this->template _setProperty("index", v); }
	inline auto _impl_js_jRegExpExecArray::input() noexcept { return this->template _getProperty<tc::js::string>("input"); }
	inline void _impl_js_jRegExpExecArray::input(tc::js::string v) noexcept { this->template _setProperty("input", v); }
	inline auto _impl_js_jRegExpMatchArray::groups() noexcept { return this->template _getProperty<tc::js::any /*UnionWithUnsupportedSubtype=[tc::js::undefined,tc::js::any]*/>("groups"); }
	inline void _impl_js_jRegExpMatchArray::groups(tc::js::any /*UnionWithUnsupportedSubtype=[tc::js::undefined,tc::js::any]*/ v) noexcept { this->template _setProperty("groups", v); }
	inline auto _impl_js_jRegExpMatchArray::index() noexcept { return this->template _getProperty<tc::jst::union_t<double, tc::js::undefined>>("index"); }
	inline void _impl_js_jRegExpMatchArray::index(tc::jst::union_t<double, tc::js::undefined> v) noexcept { this->template _setProperty("index", v); }
	inline auto _impl_js_jRegExpMatchArray::input() noexcept { return this->template _getProperty<tc::jst::union_t<tc::js::string, tc::js::undefined>>("input"); }
	inline void _impl_js_jRegExpMatchArray::input(tc::jst::union_t<tc::js::string, tc::js::undefined> v) noexcept { this->template _setProperty("input", v); }
	template<typename T>
	inline auto _impl_js_jSet<T>::size() noexcept { return this->template _getProperty<double>("size"); }
	template<typename T>
	inline auto _impl_js_jSet<T>::_tcjs_construct(tc::jst::union_t<_js_jReadonlyArray<T>, tc::js::null, tc::js::undefined> values) noexcept {
		return _js_jSet<T>(emscripten::val::global("Set").new_(values));
	}
	template<typename T>
	inline auto _impl_js_jSet<T>::_tcjs_construct(tc::jst::union_t<_js_jIterable<T>, tc::js::null, tc::js::undefined> iterable) noexcept {
		return _js_jSet<T>(emscripten::val::global("Set").new_(iterable));
	}
	template<typename T>
	inline auto _impl_js_jSet<T>::add(T value) noexcept {
		return this->template _call<_js_jSet>("add", value);
	}
	template<typename T>
	inline auto _impl_js_jSet<T>::clear() noexcept {
		 this->template _call<void>("clear");
	}
	template<typename T>
	inline auto _impl_js_jSet<T>::delete_(T value) noexcept {
		return this->template _call<bool /*false*/>("delete", value);
	}
	template<typename T>
	inline auto _impl_js_jSet<T>::entries() noexcept {
		return this->template _call<_js_jIterableIterator<std::tuple<T, T>>>("entries");
	}
	template<typename T>
	inline auto _impl_js_jSet<T>::forEach(tc::jst::function<void(T, T, _js_jSet<T>)> callbackfn, tc::js::any thisArg) noexcept {
		 this->template _call<void>("forEach", callbackfn, thisArg);
	}
	template<typename T>
	inline auto _impl_js_jSet<T>::has(T value) noexcept {
		return this->template _call<bool /*false*/>("has", value);
	}
	template<typename T>
	inline auto _impl_js_jSet<T>::keys() noexcept {
		return this->template _call<_js_jIterableIterator<T>>("keys");
	}
	template<typename T>
	inline auto _impl_js_jSet<T>::values() noexcept {
		return this->template _call<_js_jIterableIterator<T>>("values");
	}
	inline auto _impl_js_jSetConstructor::prototype() noexcept { return this->template _getProperty<_js_jSet<tc::js::any>>("prototype"); }
	inline auto _impl_js_jString::length() noexcept { return this->template _getProperty<double>("length"); }
	inline auto _impl_js_jString::_tcjs_construct(tc::js::any value) noexcept {
		return _js_jString(emscripten::val::global("String").new_(value));
	}
	inline auto _impl_js_jString::anchor(tc::js::string name) noexcept {
		return this->template _call<tc::js::string>("anchor", name);
	}
	inline auto _impl_js_jString::big() noexcept {
		return this->template _call<tc::js::string>("big");
	}
	inline auto _impl_js_jString::blink() noexcept {
		return this->template _call<tc::js::string>("blink");
	}
	inline auto _impl_js_jString::bold() noexcept {
		return this->template _call<tc::js::string>("bold");
	}
	inline auto _impl_js_jString::charAt(double pos) noexcept {
		return this->template _call<tc::js::string>("charAt", pos);
	}
	inline auto _impl_js_jString::charCodeAt(double index) noexcept {
		return this->template _call<double>("charCodeAt", index);
	}
	inline auto _impl_js_jString::codePointAt(double pos) noexcept {
		return this->template _call<tc::jst::union_t<double, tc::js::undefined>>("codePointAt", pos);
	}
	inline auto _impl_js_jString::concat(tc::jst::variadic_arg_t, tc::js::string strings) noexcept {
		return this->template _call<tc::js::string>("concat", strings);
	}
	inline auto _impl_js_jString::endsWith(tc::js::string searchString, tc::jst::union_t<double, tc::js::undefined> endPosition) noexcept {
		return this->template _call<bool /*false*/>("endsWith", searchString, endPosition);
	}
	inline auto _impl_js_jString::fixed() noexcept {
		return this->template _call<tc::js::string>("fixed");
	}
	inline auto _impl_js_jString::fontcolor(tc::js::string color) noexcept {
		return this->template _call<tc::js::string>("fontcolor", color);
	}
	inline auto _impl_js_jString::fontsize(double size) noexcept {
		return this->template _call<tc::js::string>("fontsize", size);
	}
	inline auto _impl_js_jString::fontsize(tc::js::string size) noexcept {
		return this->template _call<tc::js::string>("fontsize", size);
	}
	inline auto _impl_js_jString::includes(tc::js::string searchString, tc::jst::union_t<double, tc::js::undefined> position) noexcept {
		return this->template _call<bool /*false*/>("includes", searchString, position);
	}
	inline auto _impl_js_jString::indexOf(tc::js::string searchString, tc::jst::union_t<double, tc::js::undefined> position) noexcept {
		return this->template _call<double>("indexOf", searchString, position);
	}
	inline auto _impl_js_jString::italics() noexcept {
		return this->template _call<tc::js::string>("italics");
	}
	inline auto _impl_js_jString::lastIndexOf(tc::js::string searchString, tc::jst::union_t<double, tc::js::undefined> position) noexcept {
		return this->template _call<double>("lastIndexOf", searchString, position);
	}
	inline auto _impl_js_jString::link(tc::js::string url) noexcept {
		return this->template _call<tc::js::string>("link", url);
	}
	inline auto _impl_js_jString::localeCompare(tc::js::string that) noexcept {
		return this->template _call<double>("localeCompare", that);
	}
	inline auto _impl_js_jString::localeCompare(tc::js::string that, tc::jst::union_t<_js_jArray<tc::js::string>, tc::js::string, tc::js::undefined> locales, tc::jst::union_t<_js_jIntl_dCollatorOptions, tc::js::undefined> options) noexcept {
		return this->template _call<double>("localeCompare", that, locales, options);
	}
	inline auto _impl_js_jString::match(tc::js::any /*AnonymousTypeWithLiteralType={ [Symbol.match](string: string): RegExpMatchArray | null; }[]*/ matcher) noexcept {
		return this->template _call<tc::jst::union_t<_js_jRegExpMatchArray, tc::js::null>>("match", matcher);
	}
	inline auto _impl_js_jString::match(tc::jst::union_t<_js_jRegExp, tc::js::string> regexp) noexcept {
		return this->template _call<tc::jst::union_t<_js_jRegExpMatchArray, tc::js::null>>("match", regexp);
	}
	inline auto _impl_js_jString::normalize(tc::js::string /*"NFC"*/ form) noexcept {
		return this->template _call<tc::js::string>("normalize", form);
	}
	inline auto _impl_js_jString::normalize(tc::jst::union_t<tc::js::string, tc::js::undefined> form) noexcept {
		return this->template _call<tc::js::string>("normalize", form);
	}
	inline auto _impl_js_jString::operator[](double index) noexcept {
		return this->template _getProperty<tc::js::string>(index);
	}
	inline auto _impl_js_jString::padEnd(double maxLength, tc::jst::union_t<tc::js::string, tc::js::undefined> fillString) noexcept {
		return this->template _call<tc::js::string>("padEnd", maxLength, fillString);
	}
	inline auto _impl_js_jString::padStart(double maxLength, tc::jst::union_t<tc::js::string, tc::js::undefined> fillString) noexcept {
		return this->template _call<tc::js::string>("padStart", maxLength, fillString);
	}
	inline auto _impl_js_jString::repeat(double count) noexcept {
		return this->template _call<tc::js::string>("repeat", count);
	}
	inline auto _impl_js_jString::replace(tc::js::any /*AnonymousTypeWithLiteralType={ [Symbol.replace](string: string, replaceValue: string): string; }[]*/ searchValue, tc::js::string replaceValue) noexcept {
		return this->template _call<tc::js::string>("replace", searchValue, replaceValue);
	}
	inline auto _impl_js_jString::replace(tc::js::any /*AnonymousTypeWithLiteralType={ [Symbol.replace](string: string, replacer: (substring: string, ...args: any[]) => string): string; }[]*/ searchValue, tc::jst::function<tc::js::string(tc::js::string, _js_jArray<tc::js::any>)> replacer) noexcept {
		return this->template _call<tc::js::string>("replace", searchValue, replacer);
	}
	inline auto _impl_js_jString::replace(tc::jst::union_t<_js_jRegExp, tc::js::string> searchValue, tc::js::string replaceValue) noexcept {
		return this->template _call<tc::js::string>("replace", searchValue, replaceValue);
	}
	inline auto _impl_js_jString::replace(tc::jst::union_t<_js_jRegExp, tc::js::string> searchValue, tc::jst::function<tc::js::string(tc::js::string, _js_jArray<tc::js::any>)> replacer) noexcept {
		return this->template _call<tc::js::string>("replace", searchValue, replacer);
	}
	inline auto _impl_js_jString::search(tc::js::any /*AnonymousTypeWithLiteralType={ [Symbol.search](string: string): number; }[]*/ searcher) noexcept {
		return this->template _call<double>("search", searcher);
	}
	inline auto _impl_js_jString::search(tc::jst::union_t<_js_jRegExp, tc::js::string> regexp) noexcept {
		return this->template _call<double>("search", regexp);
	}
	inline auto _impl_js_jString::slice(tc::jst::union_t<double, tc::js::undefined> start, tc::jst::union_t<double, tc::js::undefined> end) noexcept {
		return this->template _call<tc::js::string>("slice", start, end);
	}
	inline auto _impl_js_jString::small() noexcept {
		return this->template _call<tc::js::string>("small");
	}
	inline auto _impl_js_jString::split(tc::js::any /*AnonymousTypeWithLiteralType={ [Symbol.split](string: string, limit?: number | undefined): string[]; }[]*/ splitter, tc::jst::union_t<double, tc::js::undefined> limit) noexcept {
		return this->template _call<_js_jArray<tc::js::string>>("split", splitter, limit);
	}
	inline auto _impl_js_jString::split(tc::jst::union_t<_js_jRegExp, tc::js::string> separator, tc::jst::union_t<double, tc::js::undefined> limit) noexcept {
		return this->template _call<_js_jArray<tc::js::string>>("split", separator, limit);
	}
	inline auto _impl_js_jString::startsWith(tc::js::string searchString, tc::jst::union_t<double, tc::js::undefined> position) noexcept {
		return this->template _call<bool /*false*/>("startsWith", searchString, position);
	}
	inline auto _impl_js_jString::strike() noexcept {
		return this->template _call<tc::js::string>("strike");
	}
	inline auto _impl_js_jString::sub() noexcept {
		return this->template _call<tc::js::string>("sub");
	}
	inline auto _impl_js_jString::substr(double from, tc::jst::union_t<double, tc::js::undefined> length) noexcept {
		return this->template _call<tc::js::string>("substr", from, length);
	}
	inline auto _impl_js_jString::substring(double start, tc::jst::union_t<double, tc::js::undefined> end) noexcept {
		return this->template _call<tc::js::string>("substring", start, end);
	}
	inline auto _impl_js_jString::sup() noexcept {
		return this->template _call<tc::js::string>("sup");
	}
	inline auto _impl_js_jString::toLocaleLowerCase(tc::jst::union_t<_js_jArray<tc::js::string>, tc::js::string, tc::js::undefined> locales) noexcept {
		return this->template _call<tc::js::string>("toLocaleLowerCase", locales);
	}
	inline auto _impl_js_jString::toLocaleUpperCase(tc::jst::union_t<_js_jArray<tc::js::string>, tc::js::string, tc::js::undefined> locales) noexcept {
		return this->template _call<tc::js::string>("toLocaleUpperCase", locales);
	}
	inline auto _impl_js_jString::toLowerCase() noexcept {
		return this->template _call<tc::js::string>("toLowerCase");
	}
	inline auto _impl_js_jString::toString() noexcept {
		return this->template _call<tc::js::string>("toString");
	}
	inline auto _impl_js_jString::toUpperCase() noexcept {
		return this->template _call<tc::js::string>("toUpperCase");
	}
	inline auto _impl_js_jString::trim() noexcept {
		return this->template _call<tc::js::string>("trim");
	}
	inline auto _impl_js_jString::trimLeft() noexcept {
		return this->template _call<tc::js::string>("trimLeft");
	}
	inline auto _impl_js_jString::trimRight() noexcept {
		return this->template _call<tc::js::string>("trimRight");
	}
	inline auto _impl_js_jString::valueOf() noexcept {
		return this->template _call<tc::js::string>("valueOf");
	}
	inline auto _impl_js_jStringConstructor::prototype() noexcept { return this->template _getProperty<_js_jString>("prototype"); }
	inline auto _impl_js_jStringConstructor::fromCharCode(tc::jst::variadic_arg_t, double codes) noexcept {
		return this->template _call<tc::js::string>("fromCharCode", codes);
	}
	inline auto _impl_js_jStringConstructor::fromCodePoint(tc::jst::variadic_arg_t, double codePoints) noexcept {
		return this->template _call<tc::js::string>("fromCodePoint", codePoints);
	}
	inline auto _impl_js_jStringConstructor::raw(tc::js::any /*AnonymousTypeWithLiteralType={ raw: readonly string[] | ArrayLike<string>; }[raw]*/ template_, tc::jst::variadic_arg_t, tc::js::any substitutions) noexcept {
		return this->template _call<tc::js::string>("raw", template_, substitutions);
	}
	inline auto _impl_js_jSymbol::toString() noexcept {
		return this->template _call<tc::js::string>("toString");
	}
	inline auto _impl_js_jSymbol::valueOf() noexcept {
		return this->template _call<tc::js::any /* unsupported type flags =4096: symbol*/>("valueOf");
	}
	inline auto _impl_js_jSymbolConstructor::prototype() noexcept { return this->template _getProperty<_js_jSymbol>("prototype"); }
	inline auto _impl_js_jSymbolConstructor::asyncIterator() noexcept { return this->template _getProperty<tc::js::any /* unsupported type flags =8192: unique symbol*/>("asyncIterator"); }
	inline auto _impl_js_jSymbolConstructor::iterator() noexcept { return this->template _getProperty<tc::js::any /* unsupported type flags =8192: unique symbol*/>("iterator"); }
	inline auto _impl_js_jSymbolConstructor::hasInstance() noexcept { return this->template _getProperty<tc::js::any /* unsupported type flags =8192: unique symbol*/>("hasInstance"); }
	inline auto _impl_js_jSymbolConstructor::isConcatSpreadable() noexcept { return this->template _getProperty<tc::js::any /* unsupported type flags =8192: unique symbol*/>("isConcatSpreadable"); }
	inline auto _impl_js_jSymbolConstructor::match() noexcept { return this->template _getProperty<tc::js::any /* unsupported type flags =8192: unique symbol*/>("match"); }
	inline auto _impl_js_jSymbolConstructor::replace() noexcept { return this->template _getProperty<tc::js::any /* unsupported type flags =8192: unique symbol*/>("replace"); }
	inline auto _impl_js_jSymbolConstructor::search() noexcept { return this->template _getProperty<tc::js::any /* unsupported type flags =8192: unique symbol*/>("search"); }
	inline auto _impl_js_jSymbolConstructor::species() noexcept { return this->template _getProperty<tc::js::any /* unsupported type flags =8192: unique symbol*/>("species"); }
	inline auto _impl_js_jSymbolConstructor::split() noexcept { return this->template _getProperty<tc::js::any /* unsupported type flags =8192: unique symbol*/>("split"); }
	inline auto _impl_js_jSymbolConstructor::toPrimitive() noexcept { return this->template _getProperty<tc::js::any /* unsupported type flags =8192: unique symbol*/>("toPrimitive"); }
	inline auto _impl_js_jSymbolConstructor::toStringTag() noexcept { return this->template _getProperty<tc::js::any /* unsupported type flags =8192: unique symbol*/>("toStringTag"); }
	inline auto _impl_js_jSymbolConstructor::unscopables() noexcept { return this->template _getProperty<tc::js::any /* unsupported type flags =8192: unique symbol*/>("unscopables"); }
	inline auto _impl_js_jSymbolConstructor::for_(tc::js::string key) noexcept {
		return this->template _call<tc::js::any /* unsupported type flags =4096: symbol*/>("for", key);
	}
	inline auto _impl_js_jSymbolConstructor::keyFor(tc::js::any /* unsupported type flags =4096: symbol*/ sym) noexcept {
		return this->template _call<tc::jst::union_t<tc::js::string, tc::js::undefined>>("keyFor", sym);
	}
	inline auto _impl_js_jSyntaxError::_tcjs_construct(tc::jst::union_t<tc::js::string, tc::js::undefined> message) noexcept {
		return _js_jSyntaxError(emscripten::val::global("SyntaxError").new_(message));
	}
	inline auto _impl_js_jSyntaxErrorConstructor::prototype() noexcept { return this->template _getProperty<_js_jSyntaxError>("prototype"); }
	inline auto _impl_js_jTemplateStringsArray::raw() noexcept { return this->template _getProperty<_js_jReadonlyArray<tc::js::string>>("raw"); }
	template<typename T>
	inline auto _impl_js_jThisType<T>::_tcjs_construct() noexcept {
		return emscripten::val::object();
	}
	inline auto _impl_js_jTypeError::_tcjs_construct(tc::jst::union_t<tc::js::string, tc::js::undefined> message) noexcept {
		return _js_jTypeError(emscripten::val::global("TypeError").new_(message));
	}
	inline auto _impl_js_jTypeErrorConstructor::prototype() noexcept { return this->template _getProperty<_js_jTypeError>("prototype"); }
	template<typename T>
	inline auto _impl_js_jTypedPropertyDescriptor<T>::enumerable() noexcept { return this->template _getProperty<tc::jst::union_t<bool /*false*/, tc::js::undefined>>("enumerable"); }
	template<typename T>
	inline void _impl_js_jTypedPropertyDescriptor<T>::enumerable(tc::jst::union_t<bool /*false*/, tc::js::undefined> v) noexcept { this->template _setProperty("enumerable", v); }
	template<typename T>
	inline auto _impl_js_jTypedPropertyDescriptor<T>::configurable() noexcept { return this->template _getProperty<tc::jst::union_t<bool /*false*/, tc::js::undefined>>("configurable"); }
	template<typename T>
	inline void _impl_js_jTypedPropertyDescriptor<T>::configurable(tc::jst::union_t<bool /*false*/, tc::js::undefined> v) noexcept { this->template _setProperty("configurable", v); }
	template<typename T>
	inline auto _impl_js_jTypedPropertyDescriptor<T>::writable() noexcept { return this->template _getProperty<tc::jst::union_t<bool /*false*/, tc::js::undefined>>("writable"); }
	template<typename T>
	inline void _impl_js_jTypedPropertyDescriptor<T>::writable(tc::jst::union_t<bool /*false*/, tc::js::undefined> v) noexcept { this->template _setProperty("writable", v); }
	template<typename T>
	inline auto _impl_js_jTypedPropertyDescriptor<T>::value() noexcept { return this->template _getProperty<tc::jst::union_t<T, tc::js::undefined>>("value"); }
	template<typename T>
	inline void _impl_js_jTypedPropertyDescriptor<T>::value(tc::jst::union_t<T, tc::js::undefined> v) noexcept { this->template _setProperty("value", v); }
	template<typename T>
	inline auto _impl_js_jTypedPropertyDescriptor<T>::get() noexcept { return this->template _getProperty<tc::jst::union_t<tc::js::undefined, tc::jst::function<T()>>>("get"); }
	template<typename T>
	inline void _impl_js_jTypedPropertyDescriptor<T>::get(tc::jst::union_t<tc::js::undefined, tc::jst::function<T()>> v) noexcept { this->template _setProperty("get", v); }
	template<typename T>
	inline auto _impl_js_jTypedPropertyDescriptor<T>::set() noexcept { return this->template _getProperty<tc::jst::union_t<tc::js::undefined, tc::jst::function<void(T)>>>("set"); }
	template<typename T>
	inline void _impl_js_jTypedPropertyDescriptor<T>::set(tc::jst::union_t<tc::js::undefined, tc::jst::function<void(T)>> v) noexcept { this->template _setProperty("set", v); }
	template<typename T>
	inline auto _impl_js_jTypedPropertyDescriptor<T>::_tcjs_construct() noexcept {
		return emscripten::val::object();
	}
	inline auto _impl_js_jURIError::_tcjs_construct(tc::jst::union_t<tc::js::string, tc::js::undefined> message) noexcept {
		return _js_jURIError(emscripten::val::global("URIError").new_(message));
	}
	inline auto _impl_js_jURIErrorConstructor::prototype() noexcept { return this->template _getProperty<_js_jURIError>("prototype"); }
	inline auto _impl_js_jUint16Array::BYTES_PER_ELEMENT() noexcept { return this->template _getProperty<double>("BYTES_PER_ELEMENT"); }
	inline auto _impl_js_jUint16Array::buffer() noexcept { return this->template _getProperty<_js_jArrayBufferLike>("buffer"); }
	inline auto _impl_js_jUint16Array::byteLength() noexcept { return this->template _getProperty<double>("byteLength"); }
	inline auto _impl_js_jUint16Array::byteOffset() noexcept { return this->template _getProperty<double>("byteOffset"); }
	inline auto _impl_js_jUint16Array::length() noexcept { return this->template _getProperty<double>("length"); }
	inline auto _impl_js_jUint16Array::_tcjs_construct() noexcept {
		return _js_jUint16Array(emscripten::val::global("Uint16Array").new_());
	}
	inline auto _impl_js_jUint16Array::_tcjs_construct(double length) noexcept {
		return _js_jUint16Array(emscripten::val::global("Uint16Array").new_(length));
	}
	inline auto _impl_js_jUint16Array::_tcjs_construct(tc::js::any /*UnionWithUnsupportedSubtype=[_js_jArrayBuffer,tc::js::any,_js_jArrayLike<double>]*/ array) noexcept {
		return _js_jUint16Array(emscripten::val::global("Uint16Array").new_(array));
	}
	inline auto _impl_js_jUint16Array::_tcjs_construct(_js_jArrayBufferLike buffer, tc::jst::union_t<double, tc::js::undefined> byteOffset, tc::jst::union_t<double, tc::js::undefined> length) noexcept {
		return _js_jUint16Array(emscripten::val::global("Uint16Array").new_(buffer, byteOffset, length));
	}
	inline auto _impl_js_jUint16Array::_tcjs_construct(_js_jIterable<double> elements) noexcept {
		return _js_jUint16Array(emscripten::val::global("Uint16Array").new_(elements));
	}
	inline auto _impl_js_jUint16Array::copyWithin(double target, double start, tc::jst::union_t<double, tc::js::undefined> end) noexcept {
		return this->template _call<_js_jUint16Array>("copyWithin", target, start, end);
	}
	inline auto _impl_js_jUint16Array::entries() noexcept {
		return this->template _call<_js_jIterableIterator<std::tuple<double, double>>>("entries");
	}
	inline auto _impl_js_jUint16Array::every(tc::jst::function<tc::js::any(double, double, _js_jUint16Array)> predicate, tc::js::any thisArg) noexcept {
		return this->template _call<bool /*false*/>("every", predicate, thisArg);
	}
	inline auto _impl_js_jUint16Array::fill(double value, tc::jst::union_t<double, tc::js::undefined> start, tc::jst::union_t<double, tc::js::undefined> end) noexcept {
		return this->template _call<_js_jUint16Array>("fill", value, start, end);
	}
	inline auto _impl_js_jUint16Array::filter(tc::jst::function<tc::js::any(double, double, _js_jUint16Array)> predicate, tc::js::any thisArg) noexcept {
		return this->template _call<_js_jUint16Array>("filter", predicate, thisArg);
	}
	inline auto _impl_js_jUint16Array::find(tc::jst::function<bool /*false*/(double, double, _js_jUint16Array)> predicate, tc::js::any thisArg) noexcept {
		return this->template _call<tc::jst::union_t<double, tc::js::undefined>>("find", predicate, thisArg);
	}
	inline auto _impl_js_jUint16Array::findIndex(tc::jst::function<bool /*false*/(double, double, _js_jUint16Array)> predicate, tc::js::any thisArg) noexcept {
		return this->template _call<double>("findIndex", predicate, thisArg);
	}
	inline auto _impl_js_jUint16Array::forEach(tc::jst::function<void(double, double, _js_jUint16Array)> callbackfn, tc::js::any thisArg) noexcept {
		 this->template _call<void>("forEach", callbackfn, thisArg);
	}
	inline auto _impl_js_jUint16Array::includes(double searchElement, tc::jst::union_t<double, tc::js::undefined> fromIndex) noexcept {
		return this->template _call<bool /*false*/>("includes", searchElement, fromIndex);
	}
	inline auto _impl_js_jUint16Array::indexOf(double searchElement, tc::jst::union_t<double, tc::js::undefined> fromIndex) noexcept {
		return this->template _call<double>("indexOf", searchElement, fromIndex);
	}
	inline auto _impl_js_jUint16Array::join(tc::jst::union_t<tc::js::string, tc::js::undefined> separator) noexcept {
		return this->template _call<tc::js::string>("join", separator);
	}
	inline auto _impl_js_jUint16Array::keys() noexcept {
		return this->template _call<_js_jIterableIterator<double>>("keys");
	}
	inline auto _impl_js_jUint16Array::lastIndexOf(double searchElement, tc::jst::union_t<double, tc::js::undefined> fromIndex) noexcept {
		return this->template _call<double>("lastIndexOf", searchElement, fromIndex);
	}
	inline auto _impl_js_jUint16Array::map(tc::jst::function<double(double, double, _js_jUint16Array)> callbackfn, tc::js::any thisArg) noexcept {
		return this->template _call<_js_jUint16Array>("map", callbackfn, thisArg);
	}
	inline auto _impl_js_jUint16Array::operator[](double index) noexcept {
		return this->template _getProperty<double>(index);
	}
	template<typename U>
	inline auto _impl_js_jUint16Array::reduce(tc::jst::function<U(U, double, double, _js_jUint16Array)> callbackfn, U initialValue) noexcept {
		return this->template _call<U>("reduce", callbackfn, initialValue);
	}
	inline auto _impl_js_jUint16Array::reduce(tc::jst::function<double(double, double, double, _js_jUint16Array)> callbackfn) noexcept {
		return this->template _call<double>("reduce", callbackfn);
	}
	inline auto _impl_js_jUint16Array::reduce(tc::jst::function<double(double, double, double, _js_jUint16Array)> callbackfn, double initialValue) noexcept {
		return this->template _call<double>("reduce", callbackfn, initialValue);
	}
	template<typename U>
	inline auto _impl_js_jUint16Array::reduceRight(tc::jst::function<U(U, double, double, _js_jUint16Array)> callbackfn, U initialValue) noexcept {
		return this->template _call<U>("reduceRight", callbackfn, initialValue);
	}
	inline auto _impl_js_jUint16Array::reduceRight(tc::jst::function<double(double, double, double, _js_jUint16Array)> callbackfn) noexcept {
		return this->template _call<double>("reduceRight", callbackfn);
	}
	inline auto _impl_js_jUint16Array::reduceRight(tc::jst::function<double(double, double, double, _js_jUint16Array)> callbackfn, double initialValue) noexcept {
		return this->template _call<double>("reduceRight", callbackfn, initialValue);
	}
	inline auto _impl_js_jUint16Array::reverse() noexcept {
		return this->template _call<_js_jUint16Array>("reverse");
	}
	inline auto _impl_js_jUint16Array::set(_js_jArrayLike<double> array, tc::jst::union_t<double, tc::js::undefined> offset) noexcept {
		 this->template _call<void>("set", array, offset);
	}
	inline auto _impl_js_jUint16Array::slice(tc::jst::union_t<double, tc::js::undefined> start, tc::jst::union_t<double, tc::js::undefined> end) noexcept {
		return this->template _call<_js_jUint16Array>("slice", start, end);
	}
	inline auto _impl_js_jUint16Array::some(tc::jst::function<tc::js::any(double, double, _js_jUint16Array)> predicate, tc::js::any thisArg) noexcept {
		return this->template _call<bool /*false*/>("some", predicate, thisArg);
	}
	inline auto _impl_js_jUint16Array::sort(tc::jst::union_t<tc::js::undefined, tc::jst::function<double(double, double)>> compareFn) noexcept {
		return this->template _call<_js_jUint16Array>("sort", compareFn);
	}
	inline auto _impl_js_jUint16Array::subarray(tc::jst::union_t<double, tc::js::undefined> begin, tc::jst::union_t<double, tc::js::undefined> end) noexcept {
		return this->template _call<_js_jUint16Array>("subarray", begin, end);
	}
	inline auto _impl_js_jUint16Array::toLocaleString() noexcept {
		return this->template _call<tc::js::string>("toLocaleString");
	}
	inline auto _impl_js_jUint16Array::toString() noexcept {
		return this->template _call<tc::js::string>("toString");
	}
	inline auto _impl_js_jUint16Array::valueOf() noexcept {
		return this->template _call<_js_jUint16Array>("valueOf");
	}
	inline auto _impl_js_jUint16Array::values() noexcept {
		return this->template _call<_js_jIterableIterator<double>>("values");
	}
	inline auto _impl_js_jUint16ArrayConstructor::prototype() noexcept { return this->template _getProperty<_js_jUint16Array>("prototype"); }
	inline auto _impl_js_jUint16ArrayConstructor::BYTES_PER_ELEMENT() noexcept { return this->template _getProperty<double>("BYTES_PER_ELEMENT"); }
	template<typename T>
	inline auto _impl_js_jUint16ArrayConstructor::from(_js_jArrayLike<T> arrayLike, tc::jst::function<double(T, double)> mapfn, tc::js::any thisArg) noexcept {
		return this->template _call<_js_jUint16Array>("from", arrayLike, mapfn, thisArg);
	}
	inline auto _impl_js_jUint16ArrayConstructor::from(_js_jArrayLike<double> arrayLike) noexcept {
		return this->template _call<_js_jUint16Array>("from", arrayLike);
	}
	inline auto _impl_js_jUint16ArrayConstructor::from(_js_jIterable<double> arrayLike, tc::jst::union_t<tc::js::undefined, tc::jst::function<double(double, double)>> mapfn, tc::js::any thisArg) noexcept {
		return this->template _call<_js_jUint16Array>("from", arrayLike, mapfn, thisArg);
	}
	inline auto _impl_js_jUint16ArrayConstructor::of(tc::jst::variadic_arg_t, double items) noexcept {
		return this->template _call<_js_jUint16Array>("of", items);
	}
	inline auto _impl_js_jUint32Array::BYTES_PER_ELEMENT() noexcept { return this->template _getProperty<double>("BYTES_PER_ELEMENT"); }
	inline auto _impl_js_jUint32Array::buffer() noexcept { return this->template _getProperty<_js_jArrayBufferLike>("buffer"); }
	inline auto _impl_js_jUint32Array::byteLength() noexcept { return this->template _getProperty<double>("byteLength"); }
	inline auto _impl_js_jUint32Array::byteOffset() noexcept { return this->template _getProperty<double>("byteOffset"); }
	inline auto _impl_js_jUint32Array::length() noexcept { return this->template _getProperty<double>("length"); }
	inline auto _impl_js_jUint32Array::_tcjs_construct() noexcept {
		return _js_jUint32Array(emscripten::val::global("Uint32Array").new_());
	}
	inline auto _impl_js_jUint32Array::_tcjs_construct(double length) noexcept {
		return _js_jUint32Array(emscripten::val::global("Uint32Array").new_(length));
	}
	inline auto _impl_js_jUint32Array::_tcjs_construct(tc::js::any /*UnionWithUnsupportedSubtype=[_js_jArrayBuffer,tc::js::any,_js_jArrayLike<double>]*/ array) noexcept {
		return _js_jUint32Array(emscripten::val::global("Uint32Array").new_(array));
	}
	inline auto _impl_js_jUint32Array::_tcjs_construct(_js_jArrayBufferLike buffer, tc::jst::union_t<double, tc::js::undefined> byteOffset, tc::jst::union_t<double, tc::js::undefined> length) noexcept {
		return _js_jUint32Array(emscripten::val::global("Uint32Array").new_(buffer, byteOffset, length));
	}
	inline auto _impl_js_jUint32Array::_tcjs_construct(_js_jIterable<double> elements) noexcept {
		return _js_jUint32Array(emscripten::val::global("Uint32Array").new_(elements));
	}
	inline auto _impl_js_jUint32Array::copyWithin(double target, double start, tc::jst::union_t<double, tc::js::undefined> end) noexcept {
		return this->template _call<_js_jUint32Array>("copyWithin", target, start, end);
	}
	inline auto _impl_js_jUint32Array::entries() noexcept {
		return this->template _call<_js_jIterableIterator<std::tuple<double, double>>>("entries");
	}
	inline auto _impl_js_jUint32Array::every(tc::jst::function<tc::js::any(double, double, _js_jUint32Array)> predicate, tc::js::any thisArg) noexcept {
		return this->template _call<bool /*false*/>("every", predicate, thisArg);
	}
	inline auto _impl_js_jUint32Array::fill(double value, tc::jst::union_t<double, tc::js::undefined> start, tc::jst::union_t<double, tc::js::undefined> end) noexcept {
		return this->template _call<_js_jUint32Array>("fill", value, start, end);
	}
	inline auto _impl_js_jUint32Array::filter(tc::jst::function<tc::js::any(double, double, _js_jUint32Array)> predicate, tc::js::any thisArg) noexcept {
		return this->template _call<_js_jUint32Array>("filter", predicate, thisArg);
	}
	inline auto _impl_js_jUint32Array::find(tc::jst::function<bool /*false*/(double, double, _js_jUint32Array)> predicate, tc::js::any thisArg) noexcept {
		return this->template _call<tc::jst::union_t<double, tc::js::undefined>>("find", predicate, thisArg);
	}
	inline auto _impl_js_jUint32Array::findIndex(tc::jst::function<bool /*false*/(double, double, _js_jUint32Array)> predicate, tc::js::any thisArg) noexcept {
		return this->template _call<double>("findIndex", predicate, thisArg);
	}
	inline auto _impl_js_jUint32Array::forEach(tc::jst::function<void(double, double, _js_jUint32Array)> callbackfn, tc::js::any thisArg) noexcept {
		 this->template _call<void>("forEach", callbackfn, thisArg);
	}
	inline auto _impl_js_jUint32Array::includes(double searchElement, tc::jst::union_t<double, tc::js::undefined> fromIndex) noexcept {
		return this->template _call<bool /*false*/>("includes", searchElement, fromIndex);
	}
	inline auto _impl_js_jUint32Array::indexOf(double searchElement, tc::jst::union_t<double, tc::js::undefined> fromIndex) noexcept {
		return this->template _call<double>("indexOf", searchElement, fromIndex);
	}
	inline auto _impl_js_jUint32Array::join(tc::jst::union_t<tc::js::string, tc::js::undefined> separator) noexcept {
		return this->template _call<tc::js::string>("join", separator);
	}
	inline auto _impl_js_jUint32Array::keys() noexcept {
		return this->template _call<_js_jIterableIterator<double>>("keys");
	}
	inline auto _impl_js_jUint32Array::lastIndexOf(double searchElement, tc::jst::union_t<double, tc::js::undefined> fromIndex) noexcept {
		return this->template _call<double>("lastIndexOf", searchElement, fromIndex);
	}
	inline auto _impl_js_jUint32Array::map(tc::jst::function<double(double, double, _js_jUint32Array)> callbackfn, tc::js::any thisArg) noexcept {
		return this->template _call<_js_jUint32Array>("map", callbackfn, thisArg);
	}
	inline auto _impl_js_jUint32Array::operator[](double index) noexcept {
		return this->template _getProperty<double>(index);
	}
	template<typename U>
	inline auto _impl_js_jUint32Array::reduce(tc::jst::function<U(U, double, double, _js_jUint32Array)> callbackfn, U initialValue) noexcept {
		return this->template _call<U>("reduce", callbackfn, initialValue);
	}
	inline auto _impl_js_jUint32Array::reduce(tc::jst::function<double(double, double, double, _js_jUint32Array)> callbackfn) noexcept {
		return this->template _call<double>("reduce", callbackfn);
	}
	inline auto _impl_js_jUint32Array::reduce(tc::jst::function<double(double, double, double, _js_jUint32Array)> callbackfn, double initialValue) noexcept {
		return this->template _call<double>("reduce", callbackfn, initialValue);
	}
	template<typename U>
	inline auto _impl_js_jUint32Array::reduceRight(tc::jst::function<U(U, double, double, _js_jUint32Array)> callbackfn, U initialValue) noexcept {
		return this->template _call<U>("reduceRight", callbackfn, initialValue);
	}
	inline auto _impl_js_jUint32Array::reduceRight(tc::jst::function<double(double, double, double, _js_jUint32Array)> callbackfn) noexcept {
		return this->template _call<double>("reduceRight", callbackfn);
	}
	inline auto _impl_js_jUint32Array::reduceRight(tc::jst::function<double(double, double, double, _js_jUint32Array)> callbackfn, double initialValue) noexcept {
		return this->template _call<double>("reduceRight", callbackfn, initialValue);
	}
	inline auto _impl_js_jUint32Array::reverse() noexcept {
		return this->template _call<_js_jUint32Array>("reverse");
	}
	inline auto _impl_js_jUint32Array::set(_js_jArrayLike<double> array, tc::jst::union_t<double, tc::js::undefined> offset) noexcept {
		 this->template _call<void>("set", array, offset);
	}
	inline auto _impl_js_jUint32Array::slice(tc::jst::union_t<double, tc::js::undefined> start, tc::jst::union_t<double, tc::js::undefined> end) noexcept {
		return this->template _call<_js_jUint32Array>("slice", start, end);
	}
	inline auto _impl_js_jUint32Array::some(tc::jst::function<tc::js::any(double, double, _js_jUint32Array)> predicate, tc::js::any thisArg) noexcept {
		return this->template _call<bool /*false*/>("some", predicate, thisArg);
	}
	inline auto _impl_js_jUint32Array::sort(tc::jst::union_t<tc::js::undefined, tc::jst::function<double(double, double)>> compareFn) noexcept {
		return this->template _call<_js_jUint32Array>("sort", compareFn);
	}
	inline auto _impl_js_jUint32Array::subarray(tc::jst::union_t<double, tc::js::undefined> begin, tc::jst::union_t<double, tc::js::undefined> end) noexcept {
		return this->template _call<_js_jUint32Array>("subarray", begin, end);
	}
	inline auto _impl_js_jUint32Array::toLocaleString() noexcept {
		return this->template _call<tc::js::string>("toLocaleString");
	}
	inline auto _impl_js_jUint32Array::toString() noexcept {
		return this->template _call<tc::js::string>("toString");
	}
	inline auto _impl_js_jUint32Array::valueOf() noexcept {
		return this->template _call<_js_jUint32Array>("valueOf");
	}
	inline auto _impl_js_jUint32Array::values() noexcept {
		return this->template _call<_js_jIterableIterator<double>>("values");
	}
	inline auto _impl_js_jUint32ArrayConstructor::prototype() noexcept { return this->template _getProperty<_js_jUint32Array>("prototype"); }
	inline auto _impl_js_jUint32ArrayConstructor::BYTES_PER_ELEMENT() noexcept { return this->template _getProperty<double>("BYTES_PER_ELEMENT"); }
	template<typename T>
	inline auto _impl_js_jUint32ArrayConstructor::from(_js_jArrayLike<T> arrayLike, tc::jst::function<double(T, double)> mapfn, tc::js::any thisArg) noexcept {
		return this->template _call<_js_jUint32Array>("from", arrayLike, mapfn, thisArg);
	}
	inline auto _impl_js_jUint32ArrayConstructor::from(_js_jArrayLike<double> arrayLike) noexcept {
		return this->template _call<_js_jUint32Array>("from", arrayLike);
	}
	inline auto _impl_js_jUint32ArrayConstructor::from(_js_jIterable<double> arrayLike, tc::jst::union_t<tc::js::undefined, tc::jst::function<double(double, double)>> mapfn, tc::js::any thisArg) noexcept {
		return this->template _call<_js_jUint32Array>("from", arrayLike, mapfn, thisArg);
	}
	inline auto _impl_js_jUint32ArrayConstructor::of(tc::jst::variadic_arg_t, double items) noexcept {
		return this->template _call<_js_jUint32Array>("of", items);
	}
	inline auto _impl_js_jUint8Array::BYTES_PER_ELEMENT() noexcept { return this->template _getProperty<double>("BYTES_PER_ELEMENT"); }
	inline auto _impl_js_jUint8Array::buffer() noexcept { return this->template _getProperty<_js_jArrayBufferLike>("buffer"); }
	inline auto _impl_js_jUint8Array::byteLength() noexcept { return this->template _getProperty<double>("byteLength"); }
	inline auto _impl_js_jUint8Array::byteOffset() noexcept { return this->template _getProperty<double>("byteOffset"); }
	inline auto _impl_js_jUint8Array::length() noexcept { return this->template _getProperty<double>("length"); }
	inline auto _impl_js_jUint8Array::_tcjs_construct() noexcept {
		return _js_jUint8Array(emscripten::val::global("Uint8Array").new_());
	}
	inline auto _impl_js_jUint8Array::_tcjs_construct(double length) noexcept {
		return _js_jUint8Array(emscripten::val::global("Uint8Array").new_(length));
	}
	inline auto _impl_js_jUint8Array::_tcjs_construct(tc::js::any /*UnionWithUnsupportedSubtype=[_js_jArrayBuffer,tc::js::any,_js_jArrayLike<double>]*/ array) noexcept {
		return _js_jUint8Array(emscripten::val::global("Uint8Array").new_(array));
	}
	inline auto _impl_js_jUint8Array::_tcjs_construct(_js_jArrayBufferLike buffer, tc::jst::union_t<double, tc::js::undefined> byteOffset, tc::jst::union_t<double, tc::js::undefined> length) noexcept {
		return _js_jUint8Array(emscripten::val::global("Uint8Array").new_(buffer, byteOffset, length));
	}
	inline auto _impl_js_jUint8Array::_tcjs_construct(_js_jIterable<double> elements) noexcept {
		return _js_jUint8Array(emscripten::val::global("Uint8Array").new_(elements));
	}
	inline auto _impl_js_jUint8Array::copyWithin(double target, double start, tc::jst::union_t<double, tc::js::undefined> end) noexcept {
		return this->template _call<_js_jUint8Array>("copyWithin", target, start, end);
	}
	inline auto _impl_js_jUint8Array::entries() noexcept {
		return this->template _call<_js_jIterableIterator<std::tuple<double, double>>>("entries");
	}
	inline auto _impl_js_jUint8Array::every(tc::jst::function<tc::js::any(double, double, _js_jUint8Array)> predicate, tc::js::any thisArg) noexcept {
		return this->template _call<bool /*false*/>("every", predicate, thisArg);
	}
	inline auto _impl_js_jUint8Array::fill(double value, tc::jst::union_t<double, tc::js::undefined> start, tc::jst::union_t<double, tc::js::undefined> end) noexcept {
		return this->template _call<_js_jUint8Array>("fill", value, start, end);
	}
	inline auto _impl_js_jUint8Array::filter(tc::jst::function<tc::js::any(double, double, _js_jUint8Array)> predicate, tc::js::any thisArg) noexcept {
		return this->template _call<_js_jUint8Array>("filter", predicate, thisArg);
	}
	inline auto _impl_js_jUint8Array::find(tc::jst::function<bool /*false*/(double, double, _js_jUint8Array)> predicate, tc::js::any thisArg) noexcept {
		return this->template _call<tc::jst::union_t<double, tc::js::undefined>>("find", predicate, thisArg);
	}
	inline auto _impl_js_jUint8Array::findIndex(tc::jst::function<bool /*false*/(double, double, _js_jUint8Array)> predicate, tc::js::any thisArg) noexcept {
		return this->template _call<double>("findIndex", predicate, thisArg);
	}
	inline auto _impl_js_jUint8Array::forEach(tc::jst::function<void(double, double, _js_jUint8Array)> callbackfn, tc::js::any thisArg) noexcept {
		 this->template _call<void>("forEach", callbackfn, thisArg);
	}
	inline auto _impl_js_jUint8Array::includes(double searchElement, tc::jst::union_t<double, tc::js::undefined> fromIndex) noexcept {
		return this->template _call<bool /*false*/>("includes", searchElement, fromIndex);
	}
	inline auto _impl_js_jUint8Array::indexOf(double searchElement, tc::jst::union_t<double, tc::js::undefined> fromIndex) noexcept {
		return this->template _call<double>("indexOf", searchElement, fromIndex);
	}
	inline auto _impl_js_jUint8Array::join(tc::jst::union_t<tc::js::string, tc::js::undefined> separator) noexcept {
		return this->template _call<tc::js::string>("join", separator);
	}
	inline auto _impl_js_jUint8Array::keys() noexcept {
		return this->template _call<_js_jIterableIterator<double>>("keys");
	}
	inline auto _impl_js_jUint8Array::lastIndexOf(double searchElement, tc::jst::union_t<double, tc::js::undefined> fromIndex) noexcept {
		return this->template _call<double>("lastIndexOf", searchElement, fromIndex);
	}
	inline auto _impl_js_jUint8Array::map(tc::jst::function<double(double, double, _js_jUint8Array)> callbackfn, tc::js::any thisArg) noexcept {
		return this->template _call<_js_jUint8Array>("map", callbackfn, thisArg);
	}
	inline auto _impl_js_jUint8Array::operator[](double index) noexcept {
		return this->template _getProperty<double>(index);
	}
	template<typename U>
	inline auto _impl_js_jUint8Array::reduce(tc::jst::function<U(U, double, double, _js_jUint8Array)> callbackfn, U initialValue) noexcept {
		return this->template _call<U>("reduce", callbackfn, initialValue);
	}
	inline auto _impl_js_jUint8Array::reduce(tc::jst::function<double(double, double, double, _js_jUint8Array)> callbackfn) noexcept {
		return this->template _call<double>("reduce", callbackfn);
	}
	inline auto _impl_js_jUint8Array::reduce(tc::jst::function<double(double, double, double, _js_jUint8Array)> callbackfn, double initialValue) noexcept {
		return this->template _call<double>("reduce", callbackfn, initialValue);
	}
	template<typename U>
	inline auto _impl_js_jUint8Array::reduceRight(tc::jst::function<U(U, double, double, _js_jUint8Array)> callbackfn, U initialValue) noexcept {
		return this->template _call<U>("reduceRight", callbackfn, initialValue);
	}
	inline auto _impl_js_jUint8Array::reduceRight(tc::jst::function<double(double, double, double, _js_jUint8Array)> callbackfn) noexcept {
		return this->template _call<double>("reduceRight", callbackfn);
	}
	inline auto _impl_js_jUint8Array::reduceRight(tc::jst::function<double(double, double, double, _js_jUint8Array)> callbackfn, double initialValue) noexcept {
		return this->template _call<double>("reduceRight", callbackfn, initialValue);
	}
	inline auto _impl_js_jUint8Array::reverse() noexcept {
		return this->template _call<_js_jUint8Array>("reverse");
	}
	inline auto _impl_js_jUint8Array::set(_js_jArrayLike<double> array, tc::jst::union_t<double, tc::js::undefined> offset) noexcept {
		 this->template _call<void>("set", array, offset);
	}
	inline auto _impl_js_jUint8Array::slice(tc::jst::union_t<double, tc::js::undefined> start, tc::jst::union_t<double, tc::js::undefined> end) noexcept {
		return this->template _call<_js_jUint8Array>("slice", start, end);
	}
	inline auto _impl_js_jUint8Array::some(tc::jst::function<tc::js::any(double, double, _js_jUint8Array)> predicate, tc::js::any thisArg) noexcept {
		return this->template _call<bool /*false*/>("some", predicate, thisArg);
	}
	inline auto _impl_js_jUint8Array::sort(tc::jst::union_t<tc::js::undefined, tc::jst::function<double(double, double)>> compareFn) noexcept {
		return this->template _call<_js_jUint8Array>("sort", compareFn);
	}
	inline auto _impl_js_jUint8Array::subarray(tc::jst::union_t<double, tc::js::undefined> begin, tc::jst::union_t<double, tc::js::undefined> end) noexcept {
		return this->template _call<_js_jUint8Array>("subarray", begin, end);
	}
	inline auto _impl_js_jUint8Array::toLocaleString() noexcept {
		return this->template _call<tc::js::string>("toLocaleString");
	}
	inline auto _impl_js_jUint8Array::toString() noexcept {
		return this->template _call<tc::js::string>("toString");
	}
	inline auto _impl_js_jUint8Array::valueOf() noexcept {
		return this->template _call<_js_jUint8Array>("valueOf");
	}
	inline auto _impl_js_jUint8Array::values() noexcept {
		return this->template _call<_js_jIterableIterator<double>>("values");
	}
	inline auto _impl_js_jUint8ArrayConstructor::prototype() noexcept { return this->template _getProperty<_js_jUint8Array>("prototype"); }
	inline auto _impl_js_jUint8ArrayConstructor::BYTES_PER_ELEMENT() noexcept { return this->template _getProperty<double>("BYTES_PER_ELEMENT"); }
	template<typename T>
	inline auto _impl_js_jUint8ArrayConstructor::from(_js_jArrayLike<T> arrayLike, tc::jst::function<double(T, double)> mapfn, tc::js::any thisArg) noexcept {
		return this->template _call<_js_jUint8Array>("from", arrayLike, mapfn, thisArg);
	}
	inline auto _impl_js_jUint8ArrayConstructor::from(_js_jArrayLike<double> arrayLike) noexcept {
		return this->template _call<_js_jUint8Array>("from", arrayLike);
	}
	inline auto _impl_js_jUint8ArrayConstructor::from(_js_jIterable<double> arrayLike, tc::jst::union_t<tc::js::undefined, tc::jst::function<double(double, double)>> mapfn, tc::js::any thisArg) noexcept {
		return this->template _call<_js_jUint8Array>("from", arrayLike, mapfn, thisArg);
	}
	inline auto _impl_js_jUint8ArrayConstructor::of(tc::jst::variadic_arg_t, double items) noexcept {
		return this->template _call<_js_jUint8Array>("of", items);
	}
	inline auto _impl_js_jUint8ClampedArray::BYTES_PER_ELEMENT() noexcept { return this->template _getProperty<double>("BYTES_PER_ELEMENT"); }
	inline auto _impl_js_jUint8ClampedArray::buffer() noexcept { return this->template _getProperty<_js_jArrayBufferLike>("buffer"); }
	inline auto _impl_js_jUint8ClampedArray::byteLength() noexcept { return this->template _getProperty<double>("byteLength"); }
	inline auto _impl_js_jUint8ClampedArray::byteOffset() noexcept { return this->template _getProperty<double>("byteOffset"); }
	inline auto _impl_js_jUint8ClampedArray::length() noexcept { return this->template _getProperty<double>("length"); }
	inline auto _impl_js_jUint8ClampedArray::_tcjs_construct() noexcept {
		return _js_jUint8ClampedArray(emscripten::val::global("Uint8ClampedArray").new_());
	}
	inline auto _impl_js_jUint8ClampedArray::_tcjs_construct(double length) noexcept {
		return _js_jUint8ClampedArray(emscripten::val::global("Uint8ClampedArray").new_(length));
	}
	inline auto _impl_js_jUint8ClampedArray::_tcjs_construct(tc::js::any /*UnionWithUnsupportedSubtype=[_js_jArrayBuffer,tc::js::any,_js_jArrayLike<double>]*/ array) noexcept {
		return _js_jUint8ClampedArray(emscripten::val::global("Uint8ClampedArray").new_(array));
	}
	inline auto _impl_js_jUint8ClampedArray::_tcjs_construct(_js_jArrayBufferLike buffer, tc::jst::union_t<double, tc::js::undefined> byteOffset, tc::jst::union_t<double, tc::js::undefined> length) noexcept {
		return _js_jUint8ClampedArray(emscripten::val::global("Uint8ClampedArray").new_(buffer, byteOffset, length));
	}
	inline auto _impl_js_jUint8ClampedArray::_tcjs_construct(_js_jIterable<double> elements) noexcept {
		return _js_jUint8ClampedArray(emscripten::val::global("Uint8ClampedArray").new_(elements));
	}
	inline auto _impl_js_jUint8ClampedArray::copyWithin(double target, double start, tc::jst::union_t<double, tc::js::undefined> end) noexcept {
		return this->template _call<_js_jUint8ClampedArray>("copyWithin", target, start, end);
	}
	inline auto _impl_js_jUint8ClampedArray::entries() noexcept {
		return this->template _call<_js_jIterableIterator<std::tuple<double, double>>>("entries");
	}
	inline auto _impl_js_jUint8ClampedArray::every(tc::jst::function<tc::js::any(double, double, _js_jUint8ClampedArray)> predicate, tc::js::any thisArg) noexcept {
		return this->template _call<bool /*false*/>("every", predicate, thisArg);
	}
	inline auto _impl_js_jUint8ClampedArray::fill(double value, tc::jst::union_t<double, tc::js::undefined> start, tc::jst::union_t<double, tc::js::undefined> end) noexcept {
		return this->template _call<_js_jUint8ClampedArray>("fill", value, start, end);
	}
	inline auto _impl_js_jUint8ClampedArray::filter(tc::jst::function<tc::js::any(double, double, _js_jUint8ClampedArray)> predicate, tc::js::any thisArg) noexcept {
		return this->template _call<_js_jUint8ClampedArray>("filter", predicate, thisArg);
	}
	inline auto _impl_js_jUint8ClampedArray::find(tc::jst::function<bool /*false*/(double, double, _js_jUint8ClampedArray)> predicate, tc::js::any thisArg) noexcept {
		return this->template _call<tc::jst::union_t<double, tc::js::undefined>>("find", predicate, thisArg);
	}
	inline auto _impl_js_jUint8ClampedArray::findIndex(tc::jst::function<bool /*false*/(double, double, _js_jUint8ClampedArray)> predicate, tc::js::any thisArg) noexcept {
		return this->template _call<double>("findIndex", predicate, thisArg);
	}
	inline auto _impl_js_jUint8ClampedArray::forEach(tc::jst::function<void(double, double, _js_jUint8ClampedArray)> callbackfn, tc::js::any thisArg) noexcept {
		 this->template _call<void>("forEach", callbackfn, thisArg);
	}
	inline auto _impl_js_jUint8ClampedArray::includes(double searchElement, tc::jst::union_t<double, tc::js::undefined> fromIndex) noexcept {
		return this->template _call<bool /*false*/>("includes", searchElement, fromIndex);
	}
	inline auto _impl_js_jUint8ClampedArray::indexOf(double searchElement, tc::jst::union_t<double, tc::js::undefined> fromIndex) noexcept {
		return this->template _call<double>("indexOf", searchElement, fromIndex);
	}
	inline auto _impl_js_jUint8ClampedArray::join(tc::jst::union_t<tc::js::string, tc::js::undefined> separator) noexcept {
		return this->template _call<tc::js::string>("join", separator);
	}
	inline auto _impl_js_jUint8ClampedArray::keys() noexcept {
		return this->template _call<_js_jIterableIterator<double>>("keys");
	}
	inline auto _impl_js_jUint8ClampedArray::lastIndexOf(double searchElement, tc::jst::union_t<double, tc::js::undefined> fromIndex) noexcept {
		return this->template _call<double>("lastIndexOf", searchElement, fromIndex);
	}
	inline auto _impl_js_jUint8ClampedArray::map(tc::jst::function<double(double, double, _js_jUint8ClampedArray)> callbackfn, tc::js::any thisArg) noexcept {
		return this->template _call<_js_jUint8ClampedArray>("map", callbackfn, thisArg);
	}
	inline auto _impl_js_jUint8ClampedArray::operator[](double index) noexcept {
		return this->template _getProperty<double>(index);
	}
	template<typename U>
	inline auto _impl_js_jUint8ClampedArray::reduce(tc::jst::function<U(U, double, double, _js_jUint8ClampedArray)> callbackfn, U initialValue) noexcept {
		return this->template _call<U>("reduce", callbackfn, initialValue);
	}
	inline auto _impl_js_jUint8ClampedArray::reduce(tc::jst::function<double(double, double, double, _js_jUint8ClampedArray)> callbackfn) noexcept {
		return this->template _call<double>("reduce", callbackfn);
	}
	inline auto _impl_js_jUint8ClampedArray::reduce(tc::jst::function<double(double, double, double, _js_jUint8ClampedArray)> callbackfn, double initialValue) noexcept {
		return this->template _call<double>("reduce", callbackfn, initialValue);
	}
	template<typename U>
	inline auto _impl_js_jUint8ClampedArray::reduceRight(tc::jst::function<U(U, double, double, _js_jUint8ClampedArray)> callbackfn, U initialValue) noexcept {
		return this->template _call<U>("reduceRight", callbackfn, initialValue);
	}
	inline auto _impl_js_jUint8ClampedArray::reduceRight(tc::jst::function<double(double, double, double, _js_jUint8ClampedArray)> callbackfn) noexcept {
		return this->template _call<double>("reduceRight", callbackfn);
	}
	inline auto _impl_js_jUint8ClampedArray::reduceRight(tc::jst::function<double(double, double, double, _js_jUint8ClampedArray)> callbackfn, double initialValue) noexcept {
		return this->template _call<double>("reduceRight", callbackfn, initialValue);
	}
	inline auto _impl_js_jUint8ClampedArray::reverse() noexcept {
		return this->template _call<_js_jUint8ClampedArray>("reverse");
	}
	inline auto _impl_js_jUint8ClampedArray::set(_js_jArrayLike<double> array, tc::jst::union_t<double, tc::js::undefined> offset) noexcept {
		 this->template _call<void>("set", array, offset);
	}
	inline auto _impl_js_jUint8ClampedArray::slice(tc::jst::union_t<double, tc::js::undefined> start, tc::jst::union_t<double, tc::js::undefined> end) noexcept {
		return this->template _call<_js_jUint8ClampedArray>("slice", start, end);
	}
	inline auto _impl_js_jUint8ClampedArray::some(tc::jst::function<tc::js::any(double, double, _js_jUint8ClampedArray)> predicate, tc::js::any thisArg) noexcept {
		return this->template _call<bool /*false*/>("some", predicate, thisArg);
	}
	inline auto _impl_js_jUint8ClampedArray::sort(tc::jst::union_t<tc::js::undefined, tc::jst::function<double(double, double)>> compareFn) noexcept {
		return this->template _call<_js_jUint8ClampedArray>("sort", compareFn);
	}
	inline auto _impl_js_jUint8ClampedArray::subarray(tc::jst::union_t<double, tc::js::undefined> begin, tc::jst::union_t<double, tc::js::undefined> end) noexcept {
		return this->template _call<_js_jUint8ClampedArray>("subarray", begin, end);
	}
	inline auto _impl_js_jUint8ClampedArray::toLocaleString() noexcept {
		return this->template _call<tc::js::string>("toLocaleString");
	}
	inline auto _impl_js_jUint8ClampedArray::toString() noexcept {
		return this->template _call<tc::js::string>("toString");
	}
	inline auto _impl_js_jUint8ClampedArray::valueOf() noexcept {
		return this->template _call<_js_jUint8ClampedArray>("valueOf");
	}
	inline auto _impl_js_jUint8ClampedArray::values() noexcept {
		return this->template _call<_js_jIterableIterator<double>>("values");
	}
	inline auto _impl_js_jUint8ClampedArrayConstructor::prototype() noexcept { return this->template _getProperty<_js_jUint8ClampedArray>("prototype"); }
	inline auto _impl_js_jUint8ClampedArrayConstructor::BYTES_PER_ELEMENT() noexcept { return this->template _getProperty<double>("BYTES_PER_ELEMENT"); }
	template<typename T>
	inline auto _impl_js_jUint8ClampedArrayConstructor::from(_js_jArrayLike<T> arrayLike, tc::jst::function<double(T, double)> mapfn, tc::js::any thisArg) noexcept {
		return this->template _call<_js_jUint8ClampedArray>("from", arrayLike, mapfn, thisArg);
	}
	inline auto _impl_js_jUint8ClampedArrayConstructor::from(_js_jArrayLike<double> arrayLike) noexcept {
		return this->template _call<_js_jUint8ClampedArray>("from", arrayLike);
	}
	inline auto _impl_js_jUint8ClampedArrayConstructor::from(_js_jIterable<double> arrayLike, tc::jst::union_t<tc::js::undefined, tc::jst::function<double(double, double)>> mapfn, tc::js::any thisArg) noexcept {
		return this->template _call<_js_jUint8ClampedArray>("from", arrayLike, mapfn, thisArg);
	}
	inline auto _impl_js_jUint8ClampedArrayConstructor::of(tc::jst::variadic_arg_t, double items) noexcept {
		return this->template _call<_js_jUint8ClampedArray>("of", items);
	}
	template<typename K, typename V>
	inline auto _impl_js_jWeakMap<K, V>::_tcjs_construct(tc::jst::union_t<_js_jReadonlyArray<std::tuple<K, V>>, tc::js::null, tc::js::undefined> entries) noexcept {
		return _js_jWeakMap<K, V>(emscripten::val::global("WeakMap").new_(entries));
	}
	template<typename K, typename V>
	inline auto _impl_js_jWeakMap<K, V>::_tcjs_construct(_js_jIterable<std::tuple<K, V>> iterable) noexcept {
		return _js_jWeakMap<K, V>(emscripten::val::global("WeakMap").new_(iterable));
	}
	template<typename K, typename V>
	inline auto _impl_js_jWeakMap<K, V>::delete_(K key) noexcept {
		return this->template _call<bool /*false*/>("delete", key);
	}
	template<typename K, typename V>
	inline auto _impl_js_jWeakMap<K, V>::get(K key) noexcept {
		return this->template _call<tc::jst::union_t<V, tc::js::undefined>>("get", key);
	}
	template<typename K, typename V>
	inline auto _impl_js_jWeakMap<K, V>::has(K key) noexcept {
		return this->template _call<bool /*false*/>("has", key);
	}
	template<typename K, typename V>
	inline auto _impl_js_jWeakMap<K, V>::set(K key, V value) noexcept {
		return this->template _call<_js_jWeakMap>("set", key, value);
	}
	inline auto _impl_js_jWeakMapConstructor::prototype() noexcept { return this->template _getProperty<_js_jWeakMap<tc::js::any /* unsupported type flags =67108864: object*/, tc::js::any>>("prototype"); }
	template<typename T>
	inline auto _impl_js_jWeakSet<T>::_tcjs_construct(tc::jst::union_t<_js_jReadonlyArray<T>, tc::js::null, tc::js::undefined> values) noexcept {
		return _js_jWeakSet<T>(emscripten::val::global("WeakSet").new_(values));
	}
	template<typename T>
	inline auto _impl_js_jWeakSet<T>::_tcjs_construct(_js_jIterable<T> iterable) noexcept {
		return _js_jWeakSet<T>(emscripten::val::global("WeakSet").new_(iterable));
	}
	template<typename T>
	inline auto _impl_js_jWeakSet<T>::add(T value) noexcept {
		return this->template _call<_js_jWeakSet>("add", value);
	}
	template<typename T>
	inline auto _impl_js_jWeakSet<T>::delete_(T value) noexcept {
		return this->template _call<bool /*false*/>("delete", value);
	}
	template<typename T>
	inline auto _impl_js_jWeakSet<T>::has(T value) noexcept {
		return this->template _call<bool /*false*/>("has", value);
	}
	inline auto _impl_js_jWeakSetConstructor::prototype() noexcept { return this->template _getProperty<_js_jWeakSet<tc::js::any /* unsupported type flags =67108864: object*/>>("prototype"); }
	inline auto decodeURI(tc::js::string encodedURI) noexcept {
		return emscripten::val::global("decodeURI")(encodedURI).template as<tc::js::string>();
	}
	inline auto decodeURIComponent(tc::js::string encodedURIComponent) noexcept {
		return emscripten::val::global("decodeURIComponent")(encodedURIComponent).template as<tc::js::string>();
	}
	inline auto encodeURI(tc::js::string uri) noexcept {
		return emscripten::val::global("encodeURI")(uri).template as<tc::js::string>();
	}
	inline auto encodeURIComponent(tc::jst::union_t<bool /*false*/, double, tc::js::string> uriComponent) noexcept {
		return emscripten::val::global("encodeURIComponent")(uriComponent).template as<tc::js::string>();
	}
	inline auto escape(tc::js::string string) noexcept {
		return emscripten::val::global("escape")(string).template as<tc::js::string>();
	}
	inline auto eval(tc::js::string x) noexcept {
		return emscripten::val::global("eval")(x).template as<tc::js::any>();
	}
	inline auto isFinite(double number) noexcept {
		return emscripten::val::global("isFinite")(number).template as<bool /*false*/>();
	}
	inline auto isNaN(double number) noexcept {
		return emscripten::val::global("isNaN")(number).template as<bool /*false*/>();
	}
	inline auto parseFloat(tc::js::string string) noexcept {
		return emscripten::val::global("parseFloat")(string).template as<double>();
	}
	inline auto parseInt(tc::js::string string, tc::jst::union_t<double, tc::js::undefined> radix) noexcept {
		return emscripten::val::global("parseInt")(string, radix).template as<double>();
	}
	inline auto unescape(tc::js::string string) noexcept {
		return emscripten::val::global("unescape")(string).template as<tc::js::string>();
	}
	inline auto Infinity() noexcept { return emscripten::val::global("Infinity").template as<double>(); }
	inline void Infinity(double v) noexcept { emscripten::val::global().set("Infinity", v); }
	inline auto NaN() noexcept { return emscripten::val::global("NaN").template as<double>(); }
	inline void NaN(double v) noexcept { emscripten::val::global().set("NaN", v); }
	inline auto Proxy() noexcept { return emscripten::val::global("Proxy").template as<_js_jProxyConstructor>(); }
	inline void Proxy(_js_jProxyConstructor v) noexcept { emscripten::val::global().set("Proxy", v); }
}; // namespace tc::js_defs
namespace tc::js {
			template<typename T>
			using Array = js_defs::_js_jArray<T>;
			using ArrayBuffer = js_defs::_js_jArrayBuffer;
			using ArrayBufferConstructor = js_defs::_js_jArrayBufferConstructor;
			using ArrayBufferTypes = js_defs::_js_jArrayBufferTypes;
			using ArrayBufferView = js_defs::_js_jArrayBufferView;
			using ArrayConstructor = js_defs::_js_jArrayConstructor;
			template<typename T>
			using ArrayLike = js_defs::_js_jArrayLike<T>;
			using Boolean = js_defs::_js_jBoolean;
			using BooleanConstructor = js_defs::_js_jBooleanConstructor;
			using CallableFunction = js_defs::_js_jCallableFunction;
			template<typename T>
			using ConcatArray = js_defs::_js_jConcatArray<T>;
			using DataView = js_defs::_js_jDataView;
			using DataViewConstructor = js_defs::_js_jDataViewConstructor;
			using Date = js_defs::_js_jDate;
			using DateConstructor = js_defs::_js_jDateConstructor;
			using Error = js_defs::_js_jError;
			using ErrorConstructor = js_defs::_js_jErrorConstructor;
			using EvalError = js_defs::_js_jEvalError;
			using EvalErrorConstructor = js_defs::_js_jEvalErrorConstructor;
			using Float32Array = js_defs::_js_jFloat32Array;
			using Float32ArrayConstructor = js_defs::_js_jFloat32ArrayConstructor;
			using Float64Array = js_defs::_js_jFloat64Array;
			using Float64ArrayConstructor = js_defs::_js_jFloat64ArrayConstructor;
			using Function = js_defs::_js_jFunction;
			using FunctionConstructor = js_defs::_js_jFunctionConstructor;
			template<typename T, typename TReturn, typename TNext>
			using Generator = js_defs::_js_jGenerator<T, TReturn, TNext>;
			using GeneratorFunction = js_defs::_js_jGeneratorFunction;
			using GeneratorFunctionConstructor = js_defs::_js_jGeneratorFunctionConstructor;
			using IArguments = js_defs::_js_jIArguments;
			using ImportMeta = js_defs::_js_jImportMeta;
			using Int16Array = js_defs::_js_jInt16Array;
			using Int16ArrayConstructor = js_defs::_js_jInt16ArrayConstructor;
			using Int32Array = js_defs::_js_jInt32Array;
			using Int32ArrayConstructor = js_defs::_js_jInt32ArrayConstructor;
			using Int8Array = js_defs::_js_jInt8Array;
			using Int8ArrayConstructor = js_defs::_js_jInt8ArrayConstructor;
			using Intl = js_defs::_js_jIntl;
			template<typename T>
			using Iterable = js_defs::_js_jIterable<T>;
			template<typename T>
			using IterableIterator = js_defs::_js_jIterableIterator<T>;
			template<typename T, typename TReturn, typename TNext>
			using Iterator = js_defs::_js_jIterator<T, TReturn, TNext>;
			template<typename TReturn>
			using IteratorReturnResult = js_defs::_js_jIteratorReturnResult<TReturn>;
			template<typename TYield>
			using IteratorYieldResult = js_defs::_js_jIteratorYieldResult<TYield>;
			using JSON = js_defs::_js_jJSON;
			template<typename K, typename V>
			using Map = js_defs::_js_jMap<K, V>;
			using MapConstructor = js_defs::_js_jMapConstructor;
			using Math = js_defs::_js_jMath;
			using NewableFunction = js_defs::_js_jNewableFunction;
			using Number = js_defs::_js_jNumber;
			using NumberConstructor = js_defs::_js_jNumberConstructor;
			using Object = js_defs::_js_jObject;
			using ObjectConstructor = js_defs::_js_jObjectConstructor;
			using PromiseConstructor = js_defs::_js_jPromiseConstructor;
			template<typename T>
			using PromiseLike = js_defs::_js_jPromiseLike<T>;
			using PropertyDescriptor = js_defs::_js_jPropertyDescriptor;
			using PropertyDescriptorMap = js_defs::_js_jPropertyDescriptorMap;
			using ProxyConstructor = js_defs::_js_jProxyConstructor;
			template<typename T>
			using ProxyHandler = js_defs::_js_jProxyHandler<T>;
			using RangeError = js_defs::_js_jRangeError;
			using RangeErrorConstructor = js_defs::_js_jRangeErrorConstructor;
			template<typename T>
			using ReadonlyArray = js_defs::_js_jReadonlyArray<T>;
			template<typename K, typename V>
			using ReadonlyMap = js_defs::_js_jReadonlyMap<K, V>;
			template<typename T>
			using ReadonlySet = js_defs::_js_jReadonlySet<T>;
			using ReferenceError = js_defs::_js_jReferenceError;
			using ReferenceErrorConstructor = js_defs::_js_jReferenceErrorConstructor;
			using Reflect = js_defs::_js_jReflect;
			using RegExp = js_defs::_js_jRegExp;
			using RegExpConstructor = js_defs::_js_jRegExpConstructor;
			using RegExpExecArray = js_defs::_js_jRegExpExecArray;
			using RegExpMatchArray = js_defs::_js_jRegExpMatchArray;
			template<typename T>
			using Set = js_defs::_js_jSet<T>;
			using SetConstructor = js_defs::_js_jSetConstructor;
			using String = js_defs::_js_jString;
			using StringConstructor = js_defs::_js_jStringConstructor;
			using Symbol = js_defs::_js_jSymbol;
			using SymbolConstructor = js_defs::_js_jSymbolConstructor;
			using SyntaxError = js_defs::_js_jSyntaxError;
			using SyntaxErrorConstructor = js_defs::_js_jSyntaxErrorConstructor;
			using TemplateStringsArray = js_defs::_js_jTemplateStringsArray;
			template<typename T>
			using ThisType = js_defs::_js_jThisType<T>;
			using TypeError = js_defs::_js_jTypeError;
			using TypeErrorConstructor = js_defs::_js_jTypeErrorConstructor;
			template<typename T>
			using TypedPropertyDescriptor = js_defs::_js_jTypedPropertyDescriptor<T>;
			using URIError = js_defs::_js_jURIError;
			using URIErrorConstructor = js_defs::_js_jURIErrorConstructor;
			using Uint16Array = js_defs::_js_jUint16Array;
			using Uint16ArrayConstructor = js_defs::_js_jUint16ArrayConstructor;
			using Uint32Array = js_defs::_js_jUint32Array;
			using Uint32ArrayConstructor = js_defs::_js_jUint32ArrayConstructor;
			using Uint8Array = js_defs::_js_jUint8Array;
			using Uint8ArrayConstructor = js_defs::_js_jUint8ArrayConstructor;
			using Uint8ClampedArray = js_defs::_js_jUint8ClampedArray;
			using Uint8ClampedArrayConstructor = js_defs::_js_jUint8ClampedArrayConstructor;
			template<typename K, typename V>
			using WeakMap = js_defs::_js_jWeakMap<K, V>;
			using WeakMapConstructor = js_defs::_js_jWeakMapConstructor;
			template<typename T>
			using WeakSet = js_defs::_js_jWeakSet<T>;
			using WeakSetConstructor = js_defs::_js_jWeakSetConstructor;
			using ArrayBufferLike = js_defs::_js_jArrayBufferLike;
			template<typename S>
			using Capitalize = js_defs::_js_jCapitalize<S>;
			template<typename T, typename TReturn>
			using IteratorResult = js_defs::_js_jIteratorResult<T, TReturn>;
			template<typename S>
			using Lowercase = js_defs::_js_jLowercase<S>;
			template<typename T, typename K>
			using Omit = js_defs::_js_jOmit<T, K>;
			using ParameterDecorator = js_defs::_js_jParameterDecorator;
			template<typename T>
			using Partial = js_defs::_js_jPartial<T>;
			template<typename T, typename K>
			using Pick = js_defs::_js_jPick<T, K>;
			using PromiseConstructorLike = js_defs::_js_jPromiseConstructorLike;
			using PropertyDecorator = js_defs::_js_jPropertyDecorator;
			template<typename T>
			using Readonly = js_defs::_js_jReadonly<T>;
			template<typename T>
			using Required = js_defs::_js_jRequired<T>;
			template<typename S>
			using Uncapitalize = js_defs::_js_jUncapitalize<S>;
			template<typename S>
			using Uppercase = js_defs::_js_jUppercase<S>;
	using js_defs::decodeURI;
	using js_defs::decodeURIComponent;
	using js_defs::encodeURI;
	using js_defs::encodeURIComponent;
	using js_defs::escape;
	using js_defs::eval;
	using js_defs::isFinite;
	using js_defs::isNaN;
	using js_defs::parseFloat;
	using js_defs::parseInt;
	using js_defs::unescape;
	using js_defs::Infinity;
	using js_defs::NaN;
	using js_defs::Proxy;
} // namespace tc::js
#pragma pop_macro("assert")
