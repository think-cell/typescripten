Module.assertEquals = function(got, expected) {
   if (got !== expected) throw new Error('Got ' + got + ', expected ' + expected);
}

Module.SomeJsClass = function(val) {
    this.intValue = val;
}

Module.TestSum = function(sum) {
    Module.assertEquals(sum(10, 20), 30);
}

Module.TestStringPass = function(callback) {
    Module.assertEquals(callback("hello"), "hello world");
}

Module.TestPassThis = 
Module.TestPassAllArguments =
Module.TestPassThisPassAllArguments = function(callback) {
    const obj = new Module.SomeJsClass(10);
    obj.method = callback;
    Module.assertEquals(obj.method(1, "message", undefined), undefined);
}

Module.TestPassThisAndReturn = 
Module.TestPassAllArgumentsAndReturn =
Module.TestPassThisPassAllArgumentsAndReturn = function(callback) {
    const obj = new Module.SomeJsClass(10);
    obj.method = callback;
    Module.assertEquals(obj.method(1, "message", undefined).intValue, 123);
}
