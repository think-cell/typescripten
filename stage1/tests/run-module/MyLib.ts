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
        str: string = "";
        arr: Array<SomeObject> = [];

        foo(x: number) { return "foo() retval " + (typeof x) + " " + x; }
    }

    export class SomeObjectWithConstructor {
        str: string;
        constructor(a: number, b: number) { this.str = "" + (a + b); }
    }

    export class SomeObjectWithPropertyInConstructor {
        constructor(public num: number) {}
    }

    export enum SomeEnum {
        ENUM10 = 10,
        ENUM20 = 20,
        ENUM21
    }

    export enum SomeHeterogeneousEnum {
        ENUM30 = 30,
        ENUMFoo = "Foo",
        ENUM40 = 40
    }

    export function checkHeterogeneousEnum(a: SomeHeterogeneousEnum, b: SomeHeterogeneousEnum, c: SomeHeterogeneousEnum) {
        if (a != SomeHeterogeneousEnum.ENUM30) throw new Error("Invalid ENUM30");
        if (b != SomeHeterogeneousEnum.ENUMFoo) throw new Error("Invalid ENUMFoo");
        if (c != SomeHeterogeneousEnum.ENUM40) throw new Error("Invalid ENUM40");
    }

    export function getEnum30() { return SomeHeterogeneousEnum.ENUM30; }
    export function getEnumFoo() { return SomeHeterogeneousEnum.ENUMFoo; }
    export function getEnum40() { return SomeHeterogeneousEnum.ENUM40; }

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
        func5(): string { return "func5"; }
        func6(): string { return "func6"; }
    }

    export function literalTypesFunction(a: 10, b: "str"): 20 {
        return 20;
    }

    export function createPromise10(): Promise<number> {
        return new Promise(function(resolve) { resolve(10); });
    }

    export function increasePromiseValue(p: Promise<number>): Promise<number> {
        return p.then((x) => x + 1);
    }

    var promiseCompleted = false;
    export function completePromiseTest() {
        promiseCompleted = true;
    }
    process.on("exit", () => {
        if (!promiseCompleted) throw new Error("Promise test did not complete");
    });

    export interface AmbientTest {
    }
    export namespace AmbientTest {
        export interface AmbientNested {
        }
    }
}

export = MyLib;
