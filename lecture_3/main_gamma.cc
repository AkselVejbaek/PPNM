#include <iostream>
#include <cmath>
#include "sfuns.h"

int main() {

    for (int i = 1; i <= 10; i++) {
        double g = sfuns::fgamma(i);
        std::cout << "Gamma(" << i << ") = " << g << std::endl;
    }

    return 0;
}