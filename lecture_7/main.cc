#include <iostream>
#include "vec.h"
#include "matrix.h"

int main() {
    // 1. Opret en 3x3 Identitetsmatrix
    matrix<double> A(3, 3);
    for(int i=0; i<3; i++) A(i,i) = 1.0;
    
    // Tilføj lidt andre værdier
    A(0,1) = 2.0;
    A(1,2) = 1.5;

    std::cout << "Matrix A:\n" << A << std::endl;

    // 2. Opret en vektor
    vec<double> v(1, 2, 3);
    std::cout << "Vektor v: " << v << std::endl;

    // 3. Beregn A * v
    vec<double> result = A * v;
    std::cout << "A * v = " << result << std::endl;

    return 0;
}