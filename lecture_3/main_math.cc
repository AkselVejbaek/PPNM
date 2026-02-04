#include <iostream>
#include <cmath>
#include <numbers> //C++20

int main() {
    double sqrt2 = std::sqrt(2.0);
    double div = 21.0 / 5.0;
    double e_pow_pi = std::exp(std::numbers::pi);
    double pi_pow_e = std::pow(std::numbers::pi, std::exp(1.0));

    std::cout << "sqrt(2) = " << sqrt2 << std::endl;
    std::cout << "21/5    = " << div << std::endl;
    std::cout << "e^pi    = " << e_pow_pi << std::endl;
    std::cout << "pi^e    = " << pi_pow_e << std::endl;

    return 0;
}
