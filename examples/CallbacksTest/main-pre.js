Module.assertEquals = function(got, expected) {
   if (got !== expected) throw new Error('Got ' + got + ', expected ' + expected);
}

Module.TestSum = function(sum) {
    Module.assertEquals(sum(10, 20), 30);
}

Module.TestPrint = function(print) {
    Module.assertEquals(print("hello"), undefined);
}

Module.TestPassThis = 
Module.TestPassAllArguments =
Module.TestPassThisPassAllArguments = function(callback) {
    const obj = {method: callback, secretField: "secretValue"};
    Module.assertEquals(obj.method(1, "message", null), undefined);
}

Module.TestPassThisAndReturn = 
Module.TestPassAllArgumentsAndReturn =
Module.TestPassThisPassAllArgumentsAndReturn = function(callback) {
    const obj = {method: callback, secretField: "secretValue"};
    Module.assertEquals(obj.method(1, "message", null), "returnValue");
}

Module.TestHeapCallback = function(callback) {
    Module.assertEquals(callback(2), undefined);
    Module.assertEquals(callback(1), undefined);
    Module.assertEquals(callback(0), undefined);
}

Module.TestHeapCallbackWithReturn = function(callback) {
    Module.assertEquals(callback(2), 12);
    Module.assertEquals(callback(1), 11);
    Module.assertEquals(callback(0), 10);
}

Module.TestHeapCallbackPassAllArguments = function(callback) {
    Module.assertEquals(callback(2, null), undefined);
    Module.assertEquals(callback(1, null), undefined);
    Module.assertEquals(callback(0, null), undefined);
}
