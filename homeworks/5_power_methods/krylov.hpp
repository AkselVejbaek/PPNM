#pragma once
#include "matrix.hpp"
#include "qr.hpp"
#include <vector>
#include <cmath>
#include <tuple>
#include <cstdlib>
#include <cassert>

namespace pp {

// ==========================================
// 1. Power Iteration
// Finds the largest dominant eigenvalue
// ==========================================
inline std::tuple<double, pp::vector> power_iteration(const pp::matrix& A, int max_iter = 1000, double tol = 1e-9) {
    int n = A.size1();
    pp::vector x(n);
    
    // Start with a random normalized vector
    for (int i = 0; i < n; i++) x[i] = ((double)std::rand() / RAND_MAX);
    x /= x.norm();

    double lambda = 0.0;
    for (int iter = 0; iter < max_iter; iter++) {
        // Calculate Ax = A * x
        pp::vector Ax(n);
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) Ax[i] += A(i, j) * x[j];
        }
        
        // Rayleigh quotient
        double lambda_new = x.dot(Ax);
        x = Ax;
        x /= x.norm();

        // Check for convergence
        if (std::abs(lambda_new - lambda) < tol) {
            lambda = lambda_new;
            break;
        }
        lambda = lambda_new;
    }
    return std::make_tuple(lambda, x);
}

// ==========================================
// 2. Shifted Inverse Iteration
// Finds the eigenvalue closest to shift 's'
// ==========================================
inline std::tuple<double, pp::vector> inverse_iteration(const pp::matrix& A, double s, int max_iter = 1000, double tol = 1e-9) {
    int n = A.size1();
    pp::matrix As = A;
    
    // Apply shift: As = A - sI
    for (int i = 0; i < n; i++) As(i, i) -= s;
    
    // Decompose once using our trusty QR solver!
    pp::qr solver(As); 

    pp::vector x(n);
    for (int i = 0; i < n; i++) x[i] = ((double)std::rand() / RAND_MAX);
    x /= x.norm();

    double lambda = 0.0;
    for (int iter = 0; iter < max_iter; iter++) {
        // Solve (A - sI)x_new = x
        pp::vector x_new = solver.solve(x); 
        x_new /= x_new.norm();
        
        // Calculate Rayleigh quotient using the ORIGINAL matrix A
        pp::vector Ax(n);
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) Ax[i] += A(i, j) * x_new[j];
        }
        double lambda_new = x_new.dot(Ax);
        
        if (std::abs(lambda_new - lambda) < tol) {
            x = x_new;
            lambda = lambda_new;
            break;
        }
        x = x_new;
        lambda = lambda_new;
    }
    return std::make_tuple(lambda, x);
}

// ==========================================
// 3. Arnoldi Iteration
// Partial orthogonal reduction into Hessenberg form
// ==========================================
inline std::tuple<pp::matrix, pp::matrix> arnoldi(const pp::matrix& A, int m) {
    int n = A.size1();
    assert(m <= n);
    
    pp::matrix Q(n, m); 
    pp::matrix H(m, m); 

    pp::vector q(n);
    for (int i = 0; i < n; i++) q[i] = ((double)std::rand() / RAND_MAX);
    q /= q.norm();
    Q[0] = q; 

    // Stabilized Gram-Schmidt process
    for (int k = 0; k < m; k++) {
        // v = A * q_k
        pp::vector v(n);
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) v[i] += A(i, j) * Q[k][j];
        }
        
        // Orthogonalize against all previous q vectors
        for (int j = 0; j <= k; j++) {
            H(j, k) = Q[j].dot(v);
            v -= Q[j] * H(j, k);
        }
        
        // Normalization step
        if (k + 1 < m) {
            H(k + 1, k) = v.norm();
            if (H(k + 1, k) > 1e-12) {
                Q[k + 1] = v;
                Q[k + 1] /= H(k + 1, k);
            } else {
                break; // Exact invariant subspace found!
            }
        }
    }
    return std::make_tuple(Q, H);
}

} // namespace pp