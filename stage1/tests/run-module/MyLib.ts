namespace MyLib {
    export function sumOfTwo(a: number, b: number): number {
        return a + b;
    }

    export var initially10: number = 10;

    export namespace ForwardReferencingNamespace {
        export function createSomeObject(): SomeObject {
            return createObject("foobar");
        }
    }

    export class SomeBaseClass {
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

    export function double__underscore() {
    }

    export function runCallback(func: (a: number, b: number) => number, a: number, b: number): number {
        return func(a, b);
    }

    export interface MyInterface1 {
        func1(): string;
    }
    export interface MyInterface2 {
        func2(): string;
    }
    export interface MyInterface3 extends MyInterface1, MyInterface2 {
        func3(): string;
    }
    export interface MyInterface4 {
        func4(): string;
    }
    export interface MyInterface5 {
        func5(): string;
    }
    export interface MyInterface6 {
        func6(): string;
    }
    export class MyInterfaceImpl1 implements MyInterface3, MyInterface4 {
        func1(): string { return "func1"; }
        func2(): string { return "func2"; }
        func3(): string { return "func3"; }
        func4(): string { return "func4"; }
    }
    export class MyInterfaceImpl2 extends MyInterfaceImpl1 implements MyInterface5, MyInterface6 {
        constructor() { super(); }
        func5(): string { return "func5"; }
        func6(): string { return "func6"; }
    }
}

export = MyLib;
