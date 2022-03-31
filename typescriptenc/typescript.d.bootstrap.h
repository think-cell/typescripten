#pragma once
/**
 * This file re-uses parts of TypeScript compiler API definitions which are
 * available at https://github.com/microsoft/TypeScript/blob/v3.5.3/lib/typescript.d.ts
 * Lots of modifications and original additions includes.
 * Original copyright for parts of the file is retained below.
 */

/*! *****************************************************************************
Copyright (c) Microsoft Corporation. All rights reserved.
Licensed under the Apache License, Version 2.0 (the "License"); you may not use
this file except in compliance with the License. You may obtain a copy of the
License at http://www.apache.org/licenses/LICENSE-2.0

THIS CODE IS PROVIDED ON AN *AS IS* BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION ANY IMPLIED
WARRANTIES OR CONDITIONS OF TITLE, FITNESS FOR A PARTICULAR PURPOSE,
MERCHANTABLITY OR NON-INFRINGEMENT.

See the Apache Version 2.0 License for specific language governing permissions
and limitations under the License.
***************************************************************************** */

#include <tc/assert_defs.h>
#include <tc/explicit_cast.h>
#include <typescripten/typescripten.h>
#include <typescripten/callback.h>
#include <typescripten/types.h>

#include <emscripten/val.h>
#include <optional>

#include "lib.es2015.d.h"
#include "ts.d.inl"
#include <typescripten/util.h>

namespace tc::js_defs {
	// Manually implemented instead of DEFINE_CONTIGUOUS_ENUM + enumset to avoid manually checking that all enums are indeed contiguous.
	constexpr tc::js::ts::SymbolFlags operator|(tc::js::ts::SymbolFlags a, tc::js::ts::SymbolFlags b) { return static_cast<tc::js::ts::SymbolFlags>(static_cast<int>(a) | static_cast<int>(b)); }
	constexpr tc::js::ts::SymbolFlags operator&(tc::js::ts::SymbolFlags a, tc::js::ts::SymbolFlags b) { return static_cast<tc::js::ts::SymbolFlags>(static_cast<int>(a) & static_cast<int>(b)); }
	constexpr tc::js::ts::SymbolFlags operator~(tc::js::ts::SymbolFlags a) { return static_cast<tc::js::ts::SymbolFlags>(~static_cast<int>(a)); }

	constexpr tc::js::ts::TypeFlags operator|(tc::js::ts::TypeFlags a, tc::js::ts::TypeFlags b) { return static_cast<tc::js::ts::TypeFlags>(static_cast<int>(a) | static_cast<int>(b)); }
	constexpr tc::js::ts::TypeFlags operator&(tc::js::ts::TypeFlags a, tc::js::ts::TypeFlags b) { return static_cast<tc::js::ts::TypeFlags>(static_cast<int>(a) & static_cast<int>(b)); }

	constexpr tc::js::ts::ObjectFlags operator|(tc::js::ts::ObjectFlags a, tc::js::ts::ObjectFlags b) { return static_cast<tc::js::ts::ObjectFlags>(static_cast<int>(a) | static_cast<int>(b)); }
	constexpr tc::js::ts::ObjectFlags operator&(tc::js::ts::ObjectFlags a, tc::js::ts::ObjectFlags b) { return static_cast<tc::js::ts::ObjectFlags>(static_cast<int>(a) & static_cast<int>(b)); }

	constexpr tc::js::ts::ModifierFlags operator|(tc::js::ts::ModifierFlags a, tc::js::ts::ModifierFlags b) { return static_cast<tc::js::ts::ModifierFlags>(static_cast<int>(a) | static_cast<int>(b)); }
	constexpr tc::js::ts::ModifierFlags operator&(tc::js::ts::ModifierFlags a, tc::js::ts::ModifierFlags b) { return static_cast<tc::js::ts::ModifierFlags>(static_cast<int>(a) & static_cast<int>(b)); }
	constexpr tc::js::ts::ModifierFlags operator~(tc::js::ts::ModifierFlags a) { return static_cast<tc::js::ts::ModifierFlags>(~static_cast<int>(a)); }

	constexpr tc::js::ts::NodeFlags operator|(tc::js::ts::NodeFlags a, tc::js::ts::NodeFlags b) { return static_cast<tc::js::ts::NodeFlags>(static_cast<int>(a) | static_cast<int>(b)); }
	constexpr tc::js::ts::NodeFlags operator&(tc::js::ts::NodeFlags a, tc::js::ts::NodeFlags b) { return static_cast<tc::js::ts::NodeFlags>(static_cast<int>(a) & static_cast<int>(b)); }
} // namespace tc::js_defs

JS_RANGE_WITH_ITERATORS_TMPL(tc::js::ts, NodeArray)

namespace tc::js::ts_ext {
	// TODO: Anonymous union type https://github.com/think-cell/tcjs/issues/16
	template<typename T>
	struct _js_IteratorValue : virtual tc::jst::object_base {
		inline auto done() noexcept { return _getProperty<bool>("done"); }
		inline auto value() noexcept { return _getProperty<T>("value"); }
	};	
	template<typename T> 
	using IteratorValue = tc::jst::ref<_js_IteratorValue<T>>;
}

namespace tc::no_adl {
	template< typename Map, typename T >
	struct range_value< Map, tc::js::ts::UnderscoreEscapedMap<T>, void > final {
		using type = T;
	};
}

namespace tc::for_each_adl {
	template<typename T, typename Sink>
	auto for_each_impl(adl_tag_t, tc::js::ts::UnderscoreEscapedMap<T> const& map, Sink&& sink) noexcept {
		auto it = map->values();
		for(;;) {
			auto iv = tc::js::ts_ext::IteratorValue<T>(it->next());
			if(iv->done()) {
				return tc::continue_;
			}
			RETURN_IF_BREAK(tc::continue_if_not_break(sink, iv->value()))
		}
	}
}
