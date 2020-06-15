#include "../../precompiled.h"
#include "MyLib.d.h"

const int STEPS = 10;

void timed(const auto &name, const auto &f) {
    std::cout << "[" << name << "] ...\n";
    for (int i = 0; i < STEPS; i++) {
        auto start = std::chrono::steady_clock::now();
        std::forward<decltype(f)>(f)();
        auto duration = std::chrono::steady_clock::now() - start;
        std::cout << "[" << name << "] Timed: " << std::chrono::duration_cast<std::chrono::microseconds>(duration).count() << " microseconds\n";
    }
}

int main() {
    double resultJs = 0, resultCpp = 0;
    std::cout << "n = " << tc::js::MyLib::arr()->length() << "\n";
    timed("JS", [&]() {
        resultJs = tc::js::MyLib::calcArrSum();
    });
    timed("C++", [&]() {
        resultCpp = tc::accumulate(tc::js::MyLib::arr(), 0.0, fn_assign_plus());
    });
    std::cout << "resultJs  = " << resultJs << "\n";
    std::cout << "resultCpp = " << resultCpp << "\n";
    _ASSERT(std::fabs(resultJs - resultCpp) < 1e-4);
}
