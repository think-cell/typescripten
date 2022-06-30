global.ts = require('typescript');
global.tsvfs = require('@typescript/vfs');

Module.CreateParseConfigHost = function() {
    return {
        useCaseSensitiveFileNames: global.ts.sys.useCaseSensitiveFileNames,
        readDirectory: global.ts.sys.readDirectory,
        fileExists: global.ts.sys.fileExists,
        readFile: global.ts.sys.readFile,
        trace: global.ts.sys.trace,
        getCurrentDirectory: global.ts.sys.getCurrentDirectory,
        onUnRecoverableConfigFileDiagnostic: function() {}
    };
}