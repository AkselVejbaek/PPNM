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
// 1. Central Finite Differences Gradient (O(eps^(1/3)) spacing)
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

// ==========================================================
// 2. Central Finite Differences Hessian Matrix
// ==========================================================
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
// 3. Newton Minimization Engine with Levenberg-Marquardt Damping
// ==========================================================
inline std::pair<pp::vector, int> minimize(
    const cost_func& phi, 
    const pp::vector& start_x, 
    double acc = 1e-4, 
    int max_iter = 1500
) {
    int n = start_x.size();
    pp::vector x = start_x;
    int steps = 0;
    double lambda_damping = 1e-3; // Dynamic stabilization damping factor

    for (int iter = 0; iter < max_iter; iter++) {
        steps++;
        pp::vector g = compute_gradient_central(phi, x);
        if (g.norm() < acc) break;

        pp::matrix H = compute_hessian_central(phi, x);
        
        // Apply robust dynamic diagonal regularization
        for (int i = 0; i < n; i++) {
            H(i, i) += lambda_damping;
        }

        pp::qr qr_solver(H);
        pp::vector minus_g(n);
        for (int i = 0; i < n; i++) minus_g[i] = -g[i];
        pp::vector Dx = qr_solver.solve(minus_g);

        // Backtracking line-search loop
        double alpha = 1.0;
        pp::vector z(n);
        double phi_x = phi(x);
        bool step_accepted = false;

        while (alpha >= 1.0 / 1024.0) {
            for (int i = 0; i < n; i++) {
                z[i] = x[i] + alpha * Dx[i];
            }
            if (phi(z) < phi_x) {
                step_accepted = true;
                break; 
            }
            alpha /= 2.0;
        }

        if (step_accepted) {
            x = z;
            lambda_damping = std::max(1e-5, lambda_damping * 0.5); // Good step, relax damping
        } else {
            lambda_damping = std::min(1e2, lambda_damping * 5.0);   // Trapped, increase damping stability
        }
        
        if (g.norm() < acc) break;
    }
    return {x, steps};
}

} // namespace pp