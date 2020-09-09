interface Bar {
    func : string;
}

interface Foo {
}

interface Test {
    isBar(foo: Foo): foo is Bar;
}