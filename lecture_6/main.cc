#include "vec.h"
#include <complex>
#include <iostream>

int main() {
    // Test med double
    vec<double> v_double(1.1, 2.2, 3.3);
    std::cout << "Double vec: " << v_double << " | Norm: " << v_double.norm() << std::endl;

    // Test med float
    vec<float> v_float(1.0f, 2.0f, 3.0f);
    std::cout << "Float vec:  " << v_float << std::endl;

    // Test med komplekse tal (Extra opgave)
    using namespace std::complex_literals;
    vec<std::complex<double>> v_complex(1.0 + 1i, 2.0 - 1i, 3.0);
    std::cout << "Complex vec: " << v_complex << " | Norm: " << v_complex.norm() << std::endl;

    return 0;
}