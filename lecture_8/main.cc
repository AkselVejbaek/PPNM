#include <iostream>
#include "vec.h"
#include "matrix.h"

int main() {
    matrix<double> A(3, 3);
    // Fyld A med nogle lineært uafhængige tal
    A(0,0)=12; A(0,1)=-51; A(0,2)=4;
    A(1,0)=6;  A(1,1)=167; A(1,2)=-68;
    A(2,0)=-4; A(2,1)=24;  A(2,2)=-41;

    matrix<double> Q(3, 3), R(3, 3);
    qr_decomp(A, Q, R);

    std::cout << "Matrix A:\n" << A << std::endl;
    std::cout << "Matrix Q (Orthogonal):\n" << Q << std::endl;
    std::cout << "Matrix R (Upper Triangular):\n" << R << std::endl;

    // Check: Q^T * Q skal være Identitet
    return 0;
}