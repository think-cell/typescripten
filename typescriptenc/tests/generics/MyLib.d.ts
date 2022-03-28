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

    export interface Node {}
    export interface DerivedNode extends Node {}

    interface NodeListOf<TNode> {
             forEach(callbackfn: (value: TNode, key: number, parent: NodeListOf<TNode>) => void, thisArg?: any): void;
             [index: number]: TNode;
    }
    
    interface A {
     some_property() : string;
     p: Record<string, number>;
    }

    interface Collection<K> {
        item(index: number) : K;
        item_or_null(index: number) : K | null;

        item_generic<T>(index: T) : K | null;
        // TypeScript permits type parameter shadowing, but C++ will not
        // item_generic_shadowing<K extends A>(index: number) : K;

        item_generic2<T extends keyof Node>(index: T) : Array<[number, string]>;
    }
    
    type CollectionUnion = Collection<number> | Collection<A> | A;

    interface B<K, T> {
        get_collection(k: K) : Collection<T>;
    }

    interface C<K extends A> extends B<K, string> {
    }

    export type TestAlias<K> = K | string | number;

    interface IndexedAccessType {
        "foo": string;
        "bar": number;
    }

    interface Test {
        func<K extends keyof IndexedAccessType>(type: K, ev: IndexedAccessType[K]): void;
    }
}





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
