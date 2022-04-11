#pragma once

#include <cstdint>
#include <emscripten/val.h>

namespace tc::jst {
	namespace callback_detail {
		void EnsureJsCallbackCppIsLinked();

		using FirstArgument = void*;
		using FunctionPointer = emscripten::val (*)(FirstArgument, emscripten::val const& emvalThis, emscripten::val const& emvalArgs) noexcept;
		using PointerNumber = std::uintptr_t;
	}
} // namespace tc::jst
