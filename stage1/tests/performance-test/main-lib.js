
mergeInto(LibraryManager.library, {
    raw_js_call__postset: '_raw_js_call();',
    raw_js_call: function() {
        var callCount = 0;
        _raw_js_call = function() {
            return ++callCount;
        };
    }
});