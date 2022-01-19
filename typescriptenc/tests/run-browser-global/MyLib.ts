namespace MyLib {
    export function sumOfTwo(a: number, b: number): number {
        return a + b;
    }

    export var initially10: number = 10;
    // export var initially10: number;  // Duplicated declarations are allowed.

    export interface MyInterface {
    }

    export class SomeBaseClass implements MyInterface {
        foo(x: number): string { return "base"; }
    }

    export class SomeObject extends SomeBaseClass {
        constructor() { super(); } // TODO: is it implicitly defined?
        str: string = "";
        arr: Array<SomeObject> = [];

        foo(x: number) { return "foo() retval " + (typeof x) + " " + x; }
    }

    export enum SomeEnum {
        ENUM10 = 10,
        ENUM20 = 20,
        ENUM21
    }

    export namespace SomeModule {
        export function sumOfThree(a: number, b:number, c: number): number {
            return a + b + c;
        }
    }

    export function createObject(a: string): SomeObject {
        const x = new SomeObject();
        x.str = a;
        return x;
    }

    export function appendToObjectStr(a: SomeObject): SomeObject {
        a.str += "x";
        return a;
    }
}

export = MyLib;