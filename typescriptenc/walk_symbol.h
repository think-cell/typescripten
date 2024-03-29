#pragma once

#include <vector>

enum ERecurse
{
	erecurseCONTINUE,
	erecurseSKIP,
	erecurseSTOP
};

template<typename Func>
tc::break_or_continue ForEachChildTypeNode(tc::js::ts::Node jnode, Func fn) noexcept {
	switch ([&]() noexcept {
		if (auto ojtypenode = tc::js::ts::isTypeNode(jnode)) {
			return fn(*ojtypenode);
		} else {
			return erecurseCONTINUE;
		}
	}()) {
		case erecurseCONTINUE: {
			tc::break_or_continue bc = tc::continue_;
			tc::js::ts::forEachChild(jnode, tc::jst::lambda([&](tc::js::ts::Node jnodeInner) noexcept -> tc::jst::union_t<double, tc::js::undefined> {
				if (tc::continue_ == bc) {
					bc = ForEachChildTypeNode(jnodeInner, fn);
				}
				return tc::js::undefined();
			}));
			return bc;
		}
		case erecurseSKIP:
			return tc::continue_;
		case erecurseSTOP:
			return tc::break_;
	}
}

void PrintSymbolTree(int nOffset, tc::js::ts::Symbol jsymType) noexcept;
