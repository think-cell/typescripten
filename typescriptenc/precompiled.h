#pragma once

#include <chrono>
#include <cmath>
#include <string>
#include <iostream>
#include <variant>
#include <vector>
#include <utility>
#include <optional>

#include <tc/conditional_range.h>
#include <tc/explicit_cast.h>
#include <tc/assert_defs.h>
#include <tc/range.h>
#include <tc/join_adaptor.h>

#include <typescripten/typescripten.h>

namespace tc {
template<>
struct appender_type<std::ostream> {
	using sink_value_type = char;

	appender_type(std::ostream& os) noexcept : m_os(os) {}

	void operator()(char c) const& noexcept {
		m_os.put(c);
	}

private:
	std::ostream& m_os;
};
}; // namespace tc
