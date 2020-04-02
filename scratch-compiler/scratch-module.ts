namespace MyModule {
    export class MyClassWithMethods {
        foo(a: number) {
        }

        bar(a: number); // MethodDeclaration
        bar(a: string); // MethodDeclaration
        bar(a) {
        }

        baz: (number) => string;

        templ<A>(a: A): [A, A] {
            return [a, a];
        }
    }
    export class MyClassWithConstructor {
        constructor(a: number) {
        }
    }
    export interface MyInterface {
        ifoo(a: number);  // MethodSignature
        ifoo(a: string);
    }
}

export = MyModule;
