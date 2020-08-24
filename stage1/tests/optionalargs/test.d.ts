interface Foo {
    do_foo(a: string, b: number | undefined) : string;
    do_foo2(a: string, b: number | undefined, c: string) : string;
    do_foo3(a: string, b: number | undefined, c: string | undefined) : string;
    do_foo4(a: string | undefined, b: number, c: string | undefined) : string;
}
