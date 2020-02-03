Module.print = function() {}
Module.printErr = function() {}

Module.onRuntimeInitialized = function() {
	function expectAssertionFailed(strFunc) {
		console.log('Ensuring that an assertion inside ' + strFunc + ' fails...');
		try {
			Module[strFunc]();
			throw new Error(strFunc + ' did not throw an error');
		} catch (e) {
			if (e.name === 'RuntimeError' && e.message.indexOf('Assertion failed:') !== -1) {
				// Do nothing, as expected.
			} else if (e.name === 'TypeError') {
				// Do nothing, as expected.
			} else {
				throw e;
			}
		}
	}
	expectAssertionFailed('derefUndefinedDoubleGet');
	expectAssertionFailed('derefNullDoubleGet');
	expectAssertionFailed('derefUndefinedStringGet');
	expectAssertionFailed('derefNullStringGet');
	expectAssertionFailed('derefUndefinedDoubleGet');
	expectAssertionFailed('derefNullDoubleGet');
	expectAssertionFailed('derefUndefinedBoolGet');
	expectAssertionFailed('derefNullBoolGet');
};
