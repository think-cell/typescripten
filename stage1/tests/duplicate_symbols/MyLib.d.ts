
// Declare namespace X twice and define it as a type alias too
declare namespace X {
    // Declare X.Y as an interface and a variable 
    export interface Y {
        a: number;
    }

    // Declare X.Z three times as class, namespace and interface
    export class Z {
        b: number;
    }
}

declare namespace X { 
    export var Y: {
        prototype: Y;
        new(): Y;
    };

    export namespace Z {
        export class C {
            c: number;
        }
    }

    export interface Z {
        d: number;
    }
}
declare type X = string;

// Declare XYZ as namespace and enum
declare namespace XYZ {
    export var x : number;
}

declare enum XYZ {
    a,
    b,
    c
}

declare enum XYZ {
    d = 4
}

// Declare interface twice
interface Foo {
    x: number;
}
interface Foo {
    y: number;
}

// Declare object test as a namespace with member and a function
declare namespace test {
    export var T: number;
}

declare function test(a: number) : void;
declare function test(a: string) : void;
  