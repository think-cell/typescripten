declare namespace Foo {
    function func1(x: number, y?: number): string;
    function func1(x: string): number;

    enum MyEnum { A = 10, B = 15, C = 20 }
    const enum NonAmbientEnum { A = 10, B, C }
    enum MyEnumMissingValues { A, B, C }
    enum MyDoubleEnum { A = 20.5, B, C }
    enum MyStringEnum { A = "Foo", B = "Bar" }
    enum MyStringEnumMissingValues { A = "Foo", B, C, D }
    enum MyHeterogeneousEnum { A = "Foo", B = 20.0, C = "Bar" }
    enum MyHeterogeneousEnumMissingValues { A = "Foo", B = 20, C, D }

    namespace NsInterface1 {
        let foo: number;
        interface Interface1 {
            do1(): void;
            someFlag() : boolean;
            trueFlag() : true;
            falseFlag() : false;
            anyValue(): any;
        }
    }
    namespace NsInterface2 {
        interface Interface2 {
            do2(): void;
            enumFoo(a: MyEnum, b: MyEnum.A | MyEnum.B, c: MyEnum.A | string): MyEnum;
        }
    }
    namespace NsInterface1 {
        interface Interface1 {
            getInstance2(a: NsInterface2.Interface2): NsInterface2.Interface2;
            (a: string): undefined | string;
            prop: number;
            readonly readonlyProp: number;
        }
        namespace Interface1 {
            var staticInterface1: string;
            function doSomethingStatic(): void;
		    var staticInterface1: string; // Duplicated declarations are sometimes allowed.
        }
    }
    namespace NsInterface2 {
        interface Interface2 {
            getInstance1(a: NsInterface1.Interface1): NsInterface1.Interface1;
        }
    }

    interface SuperBaseInterface {
        foo(): void;
    }
    interface BaseInterface extends SuperBaseInterface {
        foo(): string | number; // More specific return type than in base
    }
    interface Interface1 extends BaseInterface {
        bar(): string;
    }
    interface Interface2 extends BaseInterface {
        baz(): string;
        overloadedFunc(a: number | string | undefined): number | null | undefined;
    }
    type Interface1Or2 = Interface1 | Interface2;

    class ConstructibleClass implements Interface2 {
        constructor(a: number);
        foo(): number; // More specific return type than in base
        baz(): string;

        overloadedFunc(param1: number): number;
        overloadedFunc(param1: string): null;

        complexFunc(a: (Interface1Or2 | number)[]) : ReadonlyArray<(Interface1Or2 | number)>[];
    }

    var someProperty: number;

    interface InterfaceWithThisType {
        foo(): this is InterfaceWithThisType; // Trigger 'thisType != undefined' inside TypeScript compiler.
        bar(): InterfaceWithThisType; // Ensure that type is mangled correctly.
    }

    class ClassWithDefaultConstructor {
        a: ConstructibleClass;
    }

    class WeirdNames {
        default: number;
        "$foo": number;
    }
}

export = Foo;
