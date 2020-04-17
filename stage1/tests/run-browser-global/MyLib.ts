function sumOfTwo(a: number, b: number): number {
    return a + b;
}

var initially10: number = 10;
var initially10: number;  // Duplicated declarations are allowed.

class SomeBaseClass {
    foo(x: number): string { return "base"; }
}

class SomeObject extends SomeBaseClass {
    constructor() { super(); } // TODO: is it implicitly defined?
    str: string = "";
    arr: Array<SomeObject> = [];

    foo(x: number) { return "foo() retval " + (typeof x) + " " + x; }
}

enum SomeEnum {
    ENUM10 = 10,
    ENUM20 = 20,
    ENUM21
}

namespace SomeModule {
    export function sumOfThree(a: number, b:number, c: number): number {
        return a + b + c;
    }
}

function createObject(a: string): SomeObject {
    const x = new SomeObject();
    x.str = a;
    return x;
}

function appendToObjectStr(a: SomeObject): SomeObject {
	a.str += "x";
	return a;
}