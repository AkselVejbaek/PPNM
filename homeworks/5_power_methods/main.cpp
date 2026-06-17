#include "matrix.hpp"
#include "qr.hpp"
#include "krylov.hpp"
#include <iostream>
#include <cstdlib>
#include <cmath>

int main() {
    std::srand(42);

    std::cout << "========================================\n";
    std::cout << "TASK: POWER METHODS & KRYLOV SUBSPACES\n";
    std::cout << "========================================\n";

    // Create a generic random square matrix
    int n = 5;
    pp::matrix A(n, n);
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            A(i, j) = std::rand() % 10;
        }
        A(i, i) += 10; // Bump diagonal to ensure a strong dominant eigenvalue
    }

    A.print("Original Matrix A:");

    // 1. Power Iteration Test
    auto [lambda_max, v_max] = pp::power_iteration(A);
    std::cout << "\n[1] Power Iteration (Dominant Eigenvalue): " << lambda_max << "\n";

    // 2. Shifted Inverse Iteration Test
    double shift = 12.0;
    auto [lambda_closest, v_closest] = pp::inverse_iteration(A, shift);
    std::cout << "[2] Shifted Inverse Iteration (Closest to " << shift << "): " << lambda_closest << "\n";

    // 3. Arnoldi Iteration Test
    int m = n; // Full reduction for validation purposes
    auto [Q, H] = pp::arnoldi(A, m);
    
    H.print("\n[3] Upper Hessenberg Matrix H (from Arnoldi):");
    
    // Validation: Mathematically verify that AQ = QH
    double diff = 0.0;
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < m; j++) {
            double AQ_ij = 0, QH_ij = 0;
            // Dot product for AQ
            for(int k = 0; k < n; k++) AQ_ij += A(i, k) * Q[j][k];
            // Dot product for QH
            for(int k = 0; k < m; k++) QH_ij += Q[k][i] * H(k, j);
            
            diff += std::abs(AQ_ij - QH_ij);
        }
    }
    std::cout << "\nArnoldi Factorization Check (Sum of |AQ - QH| elements): " << diff << "\n";
    std::cout << "(If this number is close to 0, the decomposition is flawless!)\n\n";

    return 0;
}