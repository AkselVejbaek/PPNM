#pragma once
#include "matrix.hpp"
#include "qr.hpp"
#include <functional>
#include <cmath>
#include <vector>
#include <algorithm>

namespace pp {

using cost_func = std::function<double(const pp::vector&)>;

// ==========================================================
// TASK C: Shared Central Finite Differences (O(eps^(1/3)) spacing)
// ==========================================================
inline pp::vector compute_gradient_central(const cost_func& phi, const pp::vector& x) {
    int n = x.size();
    pp::vector g(n);
    pp::vector x_temp = x;
    double eps_third = std::pow(2.0, -17); // Optimal delta ~ eps^(1/3)

    for (int i = 0; i < n; i++) {
        double dx = std::max(std::abs(x[i]), 1.0) * eps_third;
        
        x_temp[i] = x[i] + dx; double phi_plus = phi(x_temp);
        x_temp[i] = x[i] - dx; double phi_minus = phi(x_temp);
        
        g[i] = (phi_plus - phi_minus) / (2.0 * dx);
        x_temp[i] = x[i]; // Reset
    }
    return g;
}

inline pp::matrix compute_hessian_central(const cost_func& phi, const pp::vector& x) {
    int n = x.size();
    pp::matrix H(n, n);
    pp::vector x_temp = x;
    double eps_third = std::pow(2.0, -17); 

    for (int j = 0; j < n; j++) {
        double dxj = std::max(std::abs(x[j]), 1.0) * eps_third;
        
        x_temp[j] = x[j] + dxj; pp::vector g_plus = compute_gradient_central(phi, x_temp);
        x_temp[j] = x[j] - dxj; pp::vector g_minus = compute_gradient_central(phi, x_temp);
        
        for (int i = 0; i < n; i++) {
            H(i, j) = (g_plus[i] - g_minus[i]) / (2.0 * dxj);
        }
        x_temp[j] = x[j]; // Reset
    }
    return H;
}

// ==========================================================
// TASK A: Newton Minimization Engine with Backtracking Line-Search
// ==========================================================
inline std::pair<pp::vector, int> minimize(
    const cost_func& phi, 
    const pp::vector& start_x, 
    double acc = 1e-4, 
    int max_iter = 1000
) {
    int n = start_x.size();
    pp::vector x = start_x;
    int steps = 0;

    for (int iter = 0; iter < max_iter; iter++) {
        steps++;
        pp::vector g = compute_gradient_central(phi, x);
        if (g.norm() < acc) break;

        pp::matrix H = compute_hessian_central(phi, x);
        
        // Levenberg Regularization to ensure positive-definiteness
        for (int i = 0; i < n; i++) {
            H(i, i) += 1e-6;
        }

        // Solve H * Dx = -g via our trusty QR solver
        pp::qr qr_solver(H);
        pp::vector minus_g(n);
        for (int i = 0; i < n; i++) minus_g[i] = -g[i];
        pp::vector Dx = qr_solver.solve(minus_g);

        // Backtracking line-search loop
        double lambda = 1.0;
        pp::vector z(n);
        double phi_x = phi(x);

        while (lambda >= 1.0 / 1024.0) {
            for (int i = 0; i < n; i++) {
                z[i] = x[i] + lambda * Dx[i];
            }
            if (phi(z) < phi_x) break; // Terminate early on step improvement
            lambda /= 2.0;
        }

        x = z;
    }
    return {x, steps};
}

} // namespace pp