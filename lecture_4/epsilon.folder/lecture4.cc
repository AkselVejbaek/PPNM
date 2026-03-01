#include <iostream>
#include <limits>
#include <cmath>
#include <iomanip>
#include <algorithm>

// Opgave: Comparing doubles
bool approx(double a, double b, double acc = 1e-9, double eps = 1e-9) {
    double diff = std::abs(a - b);
    if (diff <= acc) return true;  // absolut tolerance
    double max_ab = std::max(std::abs(a), std::abs(b));
    return diff <= eps * max_ab;   // relativ tolerance
}

int main() {
    std::cout << std::fixed << std::setprecision(17);

    // --- TASK 1: Machine Epsilon ---
    float f = 1.0f; while((float)(1.0f + f) != 1.0f) { f /= 2.0f; } f *= 2.0f;
    double d = 1.0; while((double)(1.0 + d) != 1.0) { d /= 2.0; } d *= 2.0;
    long double l = 1.0L; while((long double)(1.0L + l) != 1.0L) { l /= 2.0L; } l *= 2.0L;

    std::cout << "--- Machine Epsilon ---" << std::endl;
    std::cout << "Calculated float:       " << f << std::endl;
    std::cout << "Limits float:           " << std::numeric_limits<float>::epsilon() << std::endl;
    std::cout << "Calculated double:      " << d << std::endl;
    std::cout << "Limits double:          " << std::numeric_limits<double>::epsilon() << std::endl;
    std::cout << "Calculated long double: " << l << std::endl;
    std::cout << "Limits long double:     " << std::numeric_limits<long double>::epsilon() << "\n\n";

    // --- TASK 2: Non-commutativity ---
    double epsilon = std::pow(2, -52);
    double tiny = epsilon / 2.0;
    double a = 1.0 + tiny + tiny;
    double b = tiny + tiny + 1.0;

    std::cout << "--- Non-commutativity ---" << std::endl;
    std::cout << "tiny = " << tiny << std::endl;
    std::cout << "a = 1 + tiny + tiny = " << a << std::endl;
    std::cout << "b = tiny + tiny + 1 = " << b << std::endl;
    std::cout << "a == b ? " << (a == b ? "true" : "false") << std::endl;
    std::cout << "a > 1  ? " << (a > 1 ? "true" : "false") << std::endl;
    std::cout << "b > 1  ? " << (b > 1 ? "true" : "false") << "\n\n";

    // --- TASK 3: Comparing doubles ---
    double d1 = 0.1+0.1+0.1+0.1+0.1+0.1+0.1+0.1;
    double d2 = 8 * 0.1;

    std::cout << "--- Comparing Doubles ---" << std::endl;
    std::cout << "d1 = " << d1 << std::endl;
    std::cout << "d2 = " << d2 << std::endl;
    std::cout << "d1 == d2 ?      " << (d1 == d2 ? "true" : "false") << std::endl;
    std::cout << "approx(d1, d2)? " << (approx(d1, d2) ? "true" : "false") << std::endl;

    return 0;
}