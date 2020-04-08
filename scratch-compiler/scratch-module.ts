namespace MyModule {
    export class Foo {  // Class | ValueModule
        someMethod() { }  // Method
        a: number;  // Property
    }
    export namespace Foo {
        export function someFunction() { }  // Function
        export var a: number;  // FunctionScopedVariable
    }
}

export = MyModule;
