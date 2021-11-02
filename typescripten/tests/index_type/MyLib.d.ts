
interface IndexedAccessType {
    "foo": string;
    "bar": number;
}

interface Test {
    func<K extends keyof IndexedAccessType>(type: K, listener: (this: Test, ev: IndexedAccessType[K]) => any): void;
}