namespace MyLib {
    export const arr = (() => {
        const x = [];
        for (var i = 0; i < 100000; i++) {
            x.push(Math.random());
        }
        return x;
    })();

    export function calcArrSum() {
        var sum = 0;
        arr.forEach((val) => sum += val);
        return sum;
    }
}

export = MyLib;
