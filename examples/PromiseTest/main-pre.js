var completedTests = 0;

Module.completePromiseTest = function() {
    completedTests++;
}

Module.successfulPromise = function(val) {
    return new Promise(function(resolve) { resolve(val); });
}

Module.failingPromise = function(val) {
    return new Promise(function(resolve, reject) { reject(val); });
}

process.on('exit', (code) => {
    if (completedTests !== 3) {
        throw new Error("Some tests were not completed");
    }
});
