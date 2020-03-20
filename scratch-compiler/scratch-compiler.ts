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

const [myInterface] = typeChecker.getExportsOfModule(myModule);
assert(myInterface.name == "MyInterface");

const symMembers: ts.Symbol[] = [];
myInterface.members.forEach((symMember) => { symMembers.push(symMember); });

const [symMember] = symMembers;
console.log(symMember.name, symMember.flags);

assert(symMember.flags & ts.SymbolFlags.Property);
const [declaration] = symMember.declarations;
const type = typeChecker.getTypeOfSymbolAtLocation(symMember, declaration);
console.log((<any>typeChecker).isArrayType(type));
