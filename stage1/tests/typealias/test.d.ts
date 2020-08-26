declare function TestFunction() : test2.FooBar;

type GlobalFooBar = test2.A | test.B;
type GlobalFn = (a: test2.FooBar3) => test.FooBar;
type GenericValueType<T> = T;

declare namespace test2 {
    export function TestFunction() : test.FooBar;
    export type FooBar2 = test.FooBar;
    export type FooBar3 = A | number;
    export type FooBar = test.SFoo | test.SBar;

    export interface A {
        a: test.FooBar;
    }

    export function TestFunction2() : test2.A;
}

declare namespace test {
    export function TestFunction() : test2.FooBar;

    export interface B {
        b: test2.FooBar;
    }

    export interface SFoo {
        a:string;
        b: number;
    }
    
    export type FooBar = SFoo | SBar;

    export interface SBar {
        c:string;
        d: number;
    }
}
