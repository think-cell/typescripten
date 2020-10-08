#include "../../precompiled.h"
#include "MyLib.d.h"
#include <random>
#include <emscripten/em_asm.h>

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

    double timed2(auto const& strName, auto fn) noexcept {
        std::cout << "[" << strName << "] ...\n";
        double fResult;
        auto const tpStart = std::chrono::steady_clock::now();
        for (int i = 0; i < 1000000; ++i) {
            fResult = fn();
        }
        auto const dur = std::chrono::steady_clock::now() - tpStart;
        std::cout << "[" << strName << "]\n\tTimed: " << std::chrono::duration_cast<std::chrono::microseconds>(dur).count() << " microseconds\n\tResult: " << fResult << "\n";
        return fResult;
    }
}

extern "C" {
  double raw_js_call();
}

int main() {
    std::cout << "===== Make 1.000.000 function calls\n";
    // Must call em++ with --js-library main-lib.js to link raw_js_call
    // timed2("Extern C function call", []() noexcept {
    //     return raw_js_call();
    // });
    
    timed2("tc::js method call", []() noexcept {
        return tc::js::MyLib::next();
    });

    timed2("EM_ASM method call", []() noexcept {
        return 
            EM_ASM_DOUBLE({
                return MyLib.next();
            });
    });

    timed2("Create emscripten::val", []() noexcept {
        emscripten::val::object();
        return 0.0;
    });

    timed("JS", []() {
        return tc::js::MyLib::NextLoop();
    });

    std::cout << "===== Add up " << tc::js::MyLib::arr()->length() << " double values\n";
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
