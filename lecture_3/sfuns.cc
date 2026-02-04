#include <cmath>
#include <numbers>
#include <limits>
#include "sfuns.h"

namespace sfuns {

constexpr double PI = std::numbers::pi;

double fgamma(double x) {
    if (x < 0) return PI / std::sin(PI * x) / fgamma(1 - x);
    if (x < 9) return fgamma(x + 1) / x;

    double lnfgamma =
        x * std::log(x + 1 / (12 * x - 1 / x / 10))
        - x
        + std::log(2 * PI / x) / 2;

    return std::exp(lnfgamma);
}

double lngamma(double x) {
    constexpr double NaN = std::numeric_limits<double>::quiet_NaN();
    if (x <= 0) return NaN;

    if (x < 9) return lngamma(x + 1) - std::log(x);

    double lnfgamma =
        x * std::log(x + 1 / (12 * x - 1 / x / 10))
        - x
        + std::log(2 * PI / x) / 2;

    return lnfgamma;
}

} // namespace sfuns
