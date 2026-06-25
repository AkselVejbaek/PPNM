#pragma once
#include "matrix.hpp"
#include "qr.hpp"
#include <functional>
#include <cmath>
#include <vector>
#include <cassert>

namespace pp {

using vector_func = std::function<pp::vector(const pp::vector&)>;

inline pp::matrix compute_jacobian(const vector_func& f, const pp::vector& x, const pp::vector& fx) {
    int n = x.size();
    pp::matrix J(n, n);
    pp::vector x_temp = x;
    double eps_mach_sqrt = std::pow(2.0, -26); // Approx square root of machine epsilon

    for (int j = 0; j < n; j++) {
        double dx = std::max(std::abs(x[j]), 1.0) * eps_mach_sqrt;
        x_temp[j] += dx;
        pp::vector fx_dx = f(x_temp);
        
        for (int i = 0; i < n; i++) {
            J(i, j) = (fx_dx[i] - fx[i]) / dx;
        }
        x_temp[j] = x[j]; // Reset
    }
    return J;
}

// =========================================================================
// TASK A: Newton's Method with Backtracking Line-Search
// =========================================================================
inline pp::vector newton(
    const vector_func& f, 
    const pp::vector& start_x, 
    double acc = 1e-6, 
    double alpha_min = 1e-4, 
    int max_iter = 100
) {
    int n = start_x.size();
    pp::vector x = start_x;
    pp::vector fx = f(x);

    for (int iter = 0; iter < max_iter; iter++) {
        if (fx.norm() < acc) break;

        // Compute Numerical Jacobian & solve for Newton Step using our QR Solver
        pp::matrix J = compute_jacobian(f, x, fx);
        pp::qr qr_solver(J);
        
        pp::vector minus_fx(n);
        for (int i = 0; i < n; i++) minus_fx[i] = -fx[i];
        
        pp::vector Dx = qr_solver.solve(minus_fx);

        // Backtracking line-search loop
        double alpha = 1.0;
        pp::vector z(n);
        pp::vector fz(n);

        while (true) {
            for (int i = 0; i < n; i++) {
                z[i] = x[i] + alpha * Dx[i];
            }
            fz = f(z);

            if (fz.norm() < fx.norm()) break; // Step improves the residual
            if (alpha < alpha_min) break;     // Accept bad step at limit
            alpha /= 2.0;
        }

        x = z;
        fx = fz;

        // Check if the update step has shrunk past meaningful evaluation sizing
        double dx_norm = 0.0;
        for (int i = 0; i < n; i++) dx_norm += (alpha * Dx[i]) * (alpha * Dx[i]);
        if (std::sqrt(dx_norm) < 1e-15) break;
    }
    return x;
}

} // namespace pp