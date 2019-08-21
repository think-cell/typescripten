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
