#include <iostream>
#include <limits>

int main() {
    // Float epsilon
    float f_eps = 1.0f;
    while (1.0f + f_eps != 1.0f) {
        f_eps /= 2.0f;
    }
    f_eps *= 2.0f; // Vi skal bruge den sidste værdi, der virkede

    // Double epsilon
    double d_eps = 1.0;
    while (1.0 + d_eps != 1.0) {
        d_eps /= 2.0;
    }
    d_eps *= 2.0;

    // Long double epsilon
    long double ld_eps = 1.0L;
    while (1.0L + ld_eps != 1.0L) {
        ld_eps /= 2.0L;
    }
    ld_eps *= 2.0L;

    std::cout << "Machine Epsilon Results:" << std::endl;
    std::cout << "Float:       " << f_eps << " (Expected ~1.19e-07)" << std::endl;
    std::cout << "Double:      " << d_eps << " (Expected ~2.22e-16)" << std::endl;
    std::cout << "Long Double: " << ld_eps << " (Expected ~1.08e-19 on Mac ARM)" << std::endl;

    return 0;
}