#ifndef PRECOMPILED_H_
#define PRECOMPILED_H_

#include <string>
#include <iostream>
#include <vector>
#include <utility>
#include <optional>
#include "conditional_range.h"
#include "explicit_cast.h"
#include "range_defines.h"
#include "range.h"
#include "join_adaptor.h"
#include "js_bootstrap.h"

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

#endif // PRECOMPILED_H_
