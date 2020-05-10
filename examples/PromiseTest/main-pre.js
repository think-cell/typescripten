Module.assertEquals = function(got, expected) {
   if (got !== expected) throw new Error('Got ' + got + ', expected ' + expected);
}

Module.successfulPromise = function(val) {
    return new Promise(function(resolve) { resolve(val); });
}

Module.failingPromise = function(val) {
    return new Promise(function(resolve, reject) { reject(val); });
}
