Module.tc_js_callback_detail_js_CreateJsFunction = function(iFunctionPtr, iArgumentPtr) {
    // console.log('create callback', iFunctionPtr, iArgumentPtr);
    const fnWrapper = function() {
        if (iFunctionPtr === null) {
            console.error('A detached C++ callback is called, it is now no-op.');
            return;
        }

        return Module.tc_js_callback_detail_js_Call(iFunctionPtr, iArgumentPtr, this, arguments);
    };
    fnWrapper.detach = function() {
        // console.log('detach callback', iFunctionPtr, iArgumentPtr);
        iFunctionPtr = null;
        iArgumentPtr = null;
    }
    fnWrapper.delete = function() {
        console.error("Callback's lifetime is managed from C++ only, delete() from JS is ignored");
        // TODO: should we allow passing ownership to JavaScript sometimes?
    }
    return fnWrapper;
}
