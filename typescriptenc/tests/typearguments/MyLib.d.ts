
declare class EventEmitter {}

declare namespace Test {
    export interface Domain extends EventEmitter {
    }
}

declare module "domain" {
    class Domain2 extends EventEmitter implements Test.Domain {
    }

    function create(): Domain2;
}

declare class URLSearchParams2 implements Iterable<[string, string]> {
    [Symbol.iterator](): IterableIterator<[string, string]>;
}