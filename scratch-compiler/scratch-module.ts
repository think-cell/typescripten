namespace MyModule {
    export namespace Ns {
        export var a: (number, b: number) => number;
        export function foo(func: (a: number, b: number) => number) { }
    }
}

export = MyModule;
