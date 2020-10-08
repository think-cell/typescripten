namespace MyLib {
    export const arr = (() => {
        const x = [];
        for (var i = 0; i < 100000; i++) {
            x.push(Math.random());
        }
        return x;
    })();

    var x = 0;

    export function next() {
        x++;
        return x;
    }

    export function calcArrSum() {
        var sum = 0;
        arr.forEach((val) => sum += val);
        return sum;
    }

    export function NextLoop() {
        var result = 0.0;
        for (var i = 0; i < 1000000; i++) {
            result = next();
        }
        return result;
    }
}

export = MyLib;
