interface Foo {
    do_foo() : string;
}

interface Bar {
    do_bar() : string;
}

interface Symbol {
    foo?: Foo;
    bar?: Bar;
    member: string;
}