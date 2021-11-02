interface Bar {
    func : string;
}

interface Foo {
}

type FooBarNumber = Foo|Bar|number;

interface Test {
    isBar(foo: Foo): foo is Bar;
    isFunction(a: any): a is (s: string) => number;
    isUnion(a: any): a is Foo|Bar;
    isTypeAlias(b: number, a: any): a is FooBarNumber;
    isTypeAlias2(): this is FooBarNumber;

    nothing() : void;
}

declare function g(a: any) : a is FooBarNumber;