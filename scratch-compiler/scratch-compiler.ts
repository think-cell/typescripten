import * as assert from "assert";
import * as ts from "typescript";

const compilerOptions = {
    strict: true,
    target: ts.ScriptTarget.ES5,
    module: ts.ModuleKind.CommonJS
};

const sourceFileName = "scratch-module.ts";

const program = ts.createProgram([sourceFileName], compilerOptions);

export const typeChecker = program.getTypeChecker();
{
    const errors = ts.getPreEmitDiagnostics(program);
    if (!errors.length) {
        console.error(ts.formatDiagnosticsWithColorAndContext(errors, ts.createCompilerHost(compilerOptions)));
    }
}

const sourceFile = program.getSourceFiles().filter((sourceFile) => sourceFile.fileName == sourceFileName)[0];

const myModule = typeChecker.getSymbolAtLocation(sourceFile);
assert(myModule.name == "\"scratch-module\"");

const symMembers: ts.Symbol[] = [];
typeChecker.getExportsOfModule(myModule).forEach(exp => {
    exp.members.forEach((symMember) => { symMembers.push(symMember); });
});

symMembers.forEach(sym => {
    const tc = typeChecker, ts0 = ts; // Ensure these are captured in the debugger.
    console.log(sym);
    const decls = sym.getDeclarations();
    console.log(decls);
});

// Make sure symbols are not garbage collected.
console.log(typeChecker === undefined);
console.log(ts === undefined);
