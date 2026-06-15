#include "matrix.hpp"
#include "qr.hpp"
#include <cstdlib>
#include <iostream>
#include <string>

// Helper: Compute the transpose of a matrix
pp::matrix transpose(const pp::matrix& A) {
    pp::matrix At(A.size2(), A.size1());
    for (int i = 0; i < A.size1(); i++) {
        for (int j = 0; j < A.size2(); j++) {
            At(j, i) = A(i, j);
        }
    }
    return At;
}

// Helper: Multiply a matrix by a vector (A * x)
pp::vector mat_vec_mul(const pp::matrix& A, const pp::vector& x) {
    pp::vector res(A.size1());
    for (int i = 0; i < A.size1(); i++) {
        for (int j = 0; j < A.size2(); j++) {
            res[i] += A(i, j) * x[j];
        }
    }
    return res;
}

// Notice we added argc and argv to read terminal commands
int main(int argc, char* argv[]) {
    std::srand(42); 

    int N = 0;

    // 1. Command Line Parser: Check if "-size N" was passed
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if (arg == "-size" && i + 1 < argc) {
            N = std::atoi(argv[i + 1]); // Convert string to integer
        }
    }

    // 2. Timing Mode: If N was provided, ONLY do the math and exit quietly
    if (N > 0) {
        pp::matrix A_time(N, N);
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                A_time(i, j) = std::rand() % 10;
            }
        }
        pp::qr qr_time(A_time); // Factorize to measure the time
        return 0; // Exit successfully
    }

    // ==========================================
    // Original Test Code (Runs if no -size given)
    // ==========================================
    std::cout << "========================================\n";
    std::cout << "TASK 1: TALL MATRIX DECOMPOSITION CHECKS\n";
    std::cout << "========================================\n";
    
    pp::matrix A_tall(5, 3);
    for (int j = 0; j < 3; j++) {
        for (int i = 0; i < 5; i++) {
            A_tall(i, j) = std::rand() % 10;
        }
    }
    
    pp::qr qr_tall(A_tall);
    pp::matrix QtQ = transpose(qr_tall.Q) * qr_tall.Q;
    pp::matrix QR_tall = qr_tall.Q * qr_tall.R;
    
    A_tall.print("Original Tall Matrix A:");
    qr_tall.Q.print("Matrix Q:");
    qr_tall.R.print("Matrix R (Should be Upper Triangular):");
    QtQ.print("Check Q^T * Q (Should be Identity Matrix):");
    QR_tall.print("Check Q * R (Should match Matrix A):");

    std::cout << "\n========================================\n";
    std::cout << "TASK 2 & 3: SOLVING, DET, AND INVERSE\n";
    std::cout << "========================================\n";

    pp::matrix A_sq(3, 3);
    pp::vector b(3);
    for (int i = 0; i < 3; i++) {
        b[i] = std::rand() % 10;
        for (int j = 0; j < 3; j++) {
            A_sq(i, j) = std::rand() % 10;
        }
    }
    
    pp::qr qr_sq(A_sq);
    pp::vector x = qr_sq.solve(b);
    pp::vector Ax = mat_vec_mul(A_sq, x);
    pp::matrix B = qr_sq.inverse();
    pp::matrix AB = A_sq * B;

    A_sq.print("Square Matrix A:");
    b.print("Vector b:");
    x.print("Solved Solution Vector x:");
    Ax.print("Check A * x (Should match Vector b):");
    std::cout << "Determinant of A: " << qr_sq.det() << "\n\n";
    B.print("Inverse Matrix B (A^-1):");
    AB.print("Check A * B (Should be Identity Matrix):");

    return 0;
}