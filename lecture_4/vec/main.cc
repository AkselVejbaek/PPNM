#include "vec.h"
#include <iostream>

int main() {
    vec v1(1, 2, 3);
    vec v2(4, 5, 6);

    std::cout << "v1: " << v1 << "\n";
    std::cout << "v2: " << v2 << "\n";
    std::cout << "v1 + v2: " << (v1 + v2) << "\n";
    std::cout << "v1 dot v2: " << v1.dot(v2) << "\n";
    std::cout << "Norm of v1: " << v1.norm() << "\n";

    vec v3 = v1; // Test copy constructor
    std::cout << "v1 approx v3? " << (approx(v1, v3) ? "Yes" : "No") << "\n";

    return 0;
}