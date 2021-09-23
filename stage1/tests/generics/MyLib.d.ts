interface A {
    some_property() : string;
}

interface Collection<K> {
    item(index: number) : K;
    item_or_null(index: number) : K | null;

    item_generic<T>(index: T) : K | null;
    // TypeScript permits type parameter shadowing, but C++ will not
    // item_generic_shadowing<K extends A>(index: number) : K;

    item_generic<T extends keyof Node>(index: T) : Array<[number, string]>;
}

type CollectionUnion = Collection<number> | Collection<A> | A;

interface B<K, T> {
    get_collection(k: K) : Collection<T>;
}

interface C<K extends A> extends B<K, string> {
}

type Test<K> = K | string | number;

// interface D<K extends typeof v> extends B<K, string> {
// }

// interface D<K extends { firstName: string, lastName: string }> extends B<K, string> {
// }

// Outer type parameters:
// class A<K> {
//     public HTMLCollectionOf = new class {
//         item(index: number): K|null { return null; }
//     };
// }

interface D<K, T extends K> extends B<K, T> {
}

interface E<K, T extends K|A> extends B<K, T> {
}

// T extends keyof A
// Type1 extends Type2 ? Type3 : Type4

