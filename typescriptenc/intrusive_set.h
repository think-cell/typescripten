#pragma once

#include <boost/intrusive/set.hpp>
#include <tc/algorithm.h>
#include <tc/return_decltype.h>

namespace tc {
	template< template<typename> class RangeReturn, typename Arg, typename Key, typename ...Options  >
	auto cont_find(boost::intrusive::set<Key, Options...>& cont, Arg&& arg)
		return_decltype_MAYTHROW( cont_find_detail::cont_find_impl<RangeReturn>(cont, cont.find(std::forward<Arg>(arg), cont.key_comp())) )
	
	template< template<typename> class RangeReturn, typename Arg, typename Key, typename ...Options  >
	auto cont_find(boost::intrusive::set<Key, Options...> const& cont, Arg&& arg)
		return_decltype_MAYTHROW( cont_find_detail::cont_find_impl<RangeReturn>(cont, cont.find(std::forward<Arg>(arg), cont.key_comp())) )
}
