#ifndef WALK_SYMBOL_H_
#define WALK_SYMBOL_H_

#include <vector>
#include "typescript.d.bootstrap.h"

enum ERecurse {
    erecurseCONTINUE,
    erecurseSKIP,
    erecurseSTOP
};

template<typename Func>
tc::break_or_continue ForEachChildTypeNode(tc::js::ts::Node jnode, Func fn) noexcept {
    switch([&]() noexcept {
        if(auto ojtypenode = tc::js::ts_ext::isTypeNode(jnode)) {
            return fn(*ojtypenode);
        } else {
            return erecurseCONTINUE;
        }
    }()) {
        case erecurseCONTINUE:
        {
            tc::break_or_continue bc = tc::continue_;
            tc::js::ts::forEachChild(jnode, tc::jst::js_lambda_wrap([&](tc::js::ts::Node jnodeInner) noexcept -> tc::jst::js_unknown { 
                if(tc::continue_==bc) {
                    bc = ForEachChildTypeNode(jnodeInner, fn); 
                }
                return tc::jst::js_undefined();
            }));
            return bc;
        }
        case erecurseSKIP:
            return tc::continue_;
        case erecurseSTOP:
            return tc::break_;
    }
}

void PrintSymbolTree(tc::js::ts::TypeChecker jtsTypeChecker, int nOffset, tc::js::ts::Symbol jsymType) noexcept;

#endif // WALK_SYMBOL_H_
