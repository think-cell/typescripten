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

#include "assert_defs.h"
#include "explicit_cast.h"
#include "js_bootstrap.h"
#include "js_callback.h"
#include "js_types.h"
#include <emscripten/val.h>
#include <optional>

#include "ts.d.inl"

// Manual fixes
namespace tc::js::ts_ext {
	// TODO: Unclear why this is ok, this is not defined on TypeParameter
	struct _js_TypeParameter : virtual tc::js_defs::_impl_js_jts_dTypeParameter {
		auto constraint() noexcept { return _getProperty<tc::jst::union_t<tc::js::undefined, tc::js::ts::Type>>("constraint"); }
	};
	using TypeParameter = tc::jst::ref<_js_TypeParameter>;

	// TODO: Generics
	struct _js_SymbolTable : virtual tc::jst::object_base {
		struct _tcjs_definitions {
			using value_type = tc::js::ts::Symbol;
		};

		template<typename Fn>
		auto operator()(Fn fn) noexcept {
			return _call<void>("forEach", tc::jst::lambda([&](tc::js::ts::Symbol value, tc::js::any, tc::js::any) noexcept {
				fn(tc_move(value));
			}));
		}
	};
	using SymbolTable = tc::jst::ref<_js_SymbolTable>;

	template<typename T>
	ReadonlyArray<T> MakeReadOnlyArray(tc::js::any unk) noexcept {
		return ReadonlyArray<T>(unk);
	}

	struct _js_HeritageClause : virtual tc::js_defs::_impl_js_jts_dHeritageClause {
		auto types() noexcept { return _getProperty<ReadonlyArray<tc::js::ts::Node>>("types"); }
	};
	using HeritageClause = tc::jst::ref<_js_HeritageClause>;

	struct _js_Symbol : virtual tc::js_defs::_impl_js_jts_dSymbol {
		inline auto parent() noexcept { return _getProperty<tc::jst::optional<tc::js::ts::Symbol>>("parent"); }
		inline auto members() noexcept { return _getProperty<tc::jst::optional<tc::js::ts_ext::SymbolTable>>("members"); }
		inline auto exports() noexcept { return _getProperty<tc::jst::optional<tc::js::ts_ext::SymbolTable>>("exports"); }
		inline auto globalExports() noexcept { return _getProperty<tc::jst::optional<tc::js::ts_ext::SymbolTable>>("globalExports"); }
	};
	using Symbol = tc::jst::ref<_js_Symbol>;

	struct _js_ClassLikeDeclaration : virtual tc::js_defs::_impl_js_jts_dClassDeclaration {
		auto heritageClauses() noexcept { return _getProperty<tc::jst::optional<ReadonlyArray<tc::js::ts::HeritageClause>>>("heritageClauses"); }
	};
	using ClassLikeDeclaration = tc::jst::ref<_js_ClassLikeDeclaration>;
} // namespace tc::js::ts_ext

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
} // namespace tc::js_defs
