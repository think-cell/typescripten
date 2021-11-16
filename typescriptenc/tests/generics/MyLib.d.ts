declare module "MyLib" {
    export interface Token<TKind extends SyntaxKind> {
        readonly kind: TKind;
    }

    export enum SyntaxKind {
        Unknown = 0,
        EndOfFileToken = 1,
        SingleLineCommentTrivia = 2,
        MultiLineCommentTrivia = 3   
    }

    export type UnknownToken = Token<SyntaxKind.Unknown|SyntaxKind.SingleLineCommentTrivia>;
    export type CommentKind = SyntaxKind.SingleLineCommentTrivia | SyntaxKind.MultiLineCommentTrivia;

    export interface TestEnum {
        prop?: SyntaxKind;
        func(a: SyntaxKind.Unknown|SyntaxKind.EndOfFileToken) : void;
    }

    export interface TestEnum2<TKind extends CommentKind> extends Token<TKind> {}
    export type CommentToken = Token<CommentKind>;
}
// interface NodeListOf<TNode> {
// //     length: number;
// //     item(index: number): TNode;
// //     /**
// //      * Performs the specified action for each node in an list.
// //      * @param callbackfn  A function that accepts up to three arguments. forEach calls the callbackfn function one time for each element in the list.
// //      * @param thisArg  An object to which the this keyword can refer in the callbackfn function. If thisArg is omitted, undefined is used as the this value.
// //      */
//      forEach(callbackfn: (value: TNode, key: number, parent: NodeListOf<TNode>) => void, thisArg?: any): void;
// //     // [index: number]: TNode;
// }

// interface A {
//     // some_property() : string;
//     p: Record<string, number>;
// }

// interface Collection<K> {
//     item(index: number) : K;
//     item_or_null(index: number) : K | null;

//     // item_generic<T>(index: T) : K | null;
//     // TypeScript permits type parameter shadowing, but C++ will not
//     // item_generic_shadowing<K extends A>(index: number) : K;

//     item_generic<T extends keyof Node>(index: T) : Array<[number, string]>;
// }

// type CollectionUnion = Collection<number> | Collection<A> | A;

// interface B<K, T> {
//     get_collection(k: K) : Collection<T>;
// }

// interface C<K extends A> extends B<K, string> {
// }

// type Test<K> = K | string | number;

// // interface D<K extends typeof v> extends B<K, string> {
// // }

// // interface D<K extends { firstName: string, lastName: string }> extends B<K, string> {
// // }

// // Outer type parameters:
// // class A<K> {
// //     public HTMLCollectionOf = new class {
// //         item(index: number): K|null { return null; }
// //     };
// // }

// interface D<K, T extends K> extends B<K, T> {
// }

// interface E<K, T extends K|A> extends B<K, T> {
// }

// // T extends keyof A
// // Type1 extends Type2 ? Type3 : Type4


// interface IndexedAccessType {
//     "foo": string;
//     "bar": number;
// }

// interface Test {
//     func<K extends keyof IndexedAccessType>(type: K, listener: (this: Test, ev: IndexedAccessType[K]) => any): void;
// }