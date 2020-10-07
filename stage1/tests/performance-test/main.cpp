#include "../../precompiled.h"
#include "MyLib.d.h"
#include <random>

namespace {
    constexpr int c_STEPS = 10;

    double timed(auto const& strName, auto fn) noexcept {
        std::cout << "[" << strName << "] ...\n";
        double fResult;
        for (int i = 0; i < c_STEPS; ++i) {
            auto const tpStart = std::chrono::steady_clock::now();
            fResult = fn();
            auto const dur = std::chrono::steady_clock::now() - tpStart;
            std::cout << "[" << strName << "]\n\tTimed: " << std::chrono::duration_cast<std::chrono::microseconds>(dur).count() << " microseconds\n\tResult: " << fResult << "\n";
        }
        return fResult;
    }
}

int main() {
    std::cout << "n = " << tc::js::MyLib::arr()->length() << "\n";
    {
        std::vector<double> vecf;
        timed("Wasm", [&]() {
            if(tc::empty(vecf)) {
                vecf = tc::make_vector(
                    tc::transform(tc::iota(0, 100000), [&](int) noexcept { return static_cast<double>(std::rand())/RAND_MAX; })
                );
            }
            return tc::accumulate(vecf, 0.0, fn_assign_plus());
        });
    }

    double const fResultJs = timed("JS", []() {
        return tc::js::MyLib::calcArrSum();
    });
    double const fResultCpp = timed("C++", []() {
        return tc::accumulate(tc::js::MyLib::arr(), 0.0, fn_assign_plus());
    });
    _ASSERT(std::fabs(fResultJs - fResultCpp) < 1e-4);
}
