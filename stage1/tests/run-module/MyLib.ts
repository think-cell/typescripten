namespace MyLib {
    export function sumOfTwo(a: number, b: number): number {
        return a + b;
    }

    export var initially10: number = 10;

    export class SomeObject {
        constructor() {} // TODO: is it implicitly defined?
        str: string = "";
        arr: Array<SomeObject> = [];
    }

    export function createObject(a: string): SomeObject {
        const x = new SomeObject();
        x.str = a;
        return x;
    }
}

export = MyLib;
