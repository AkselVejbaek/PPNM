#pragma once
#include <functional>
#include <cmath>
#include <utility>
#include <limits>
#include <cassert>

namespace pp {

// Global counter to track efficiency against python/scipy
inline int ncalls = 0;

// Core Recursive 4-Point Open Quadrature Engine (With Point Reuse)
inline std::pair<double, double> integrate_core(
    const std::function<double(double)>& f,
    double a, double b,
    double acc, double eps,
    double f2, double f3
) {
    double h = b - a;
    
    // Evaluate new points only at the 1st and 4th sub-positions
    double f1 = f(a + h / 6.0);
    double f4 = f(a + 5.0 * h / 6.0);

    // Higher-order 4-point Newton-Cotes rule
    double Q = (2.0 * f1 + f2 + f3 + 2.0 * f4) / 6.0 * h;
    // Lower-order embedded rule
    double q = (f1 + f2 + f3 + f4) / 4.0 * h;

    double err = std::abs(Q - q);
    double tol = acc + eps * std::abs(Q);

    if (err < tol) {
        return std::make_pair(Q, err);
    } else {
        // Divide interval in half. Reused coordinates shift roles perfectly:
        // Left child:  reuses original f1 and f2
        // Right child: reuses original f3 and f4
        auto [Q1, err1] = integrate_core(f, a, (a + b) / 2.0, acc / std::sqrt(2.0), eps, f1, f2);
        auto [Q2, err2] = integrate_core(f, (a + b) / 2.0, b, acc / std::sqrt(2.0), eps, f3, f4);
        
        // Task C: Accumulate errors quadratically
        return std::make_pair(Q1 + Q2, std::sqrt(err1 * err1 + err2 * err2));
    }
}

// Master Adaptive Integrator Entry Point
inline std::pair<double, double> integrate(
    const std::function<double(double)>& f,
    double a, double b,
    double acc = 0.001, double eps = 0.001
) {
    double h = b - a;
    // Initial call: no points to reuse yet, compute initial f2 and f3 nodes
    double f2 = f(a + 2.0 * h / 6.0);
    double f3 = f(a + 4.0 * h / 6.0);
    return integrate_core(f, a, b, acc, eps, f2, f3);
}

// ==========================================
// TASK B: Clenshaw-Curtis Variable Transformation
// ==========================================
inline std::pair<double, double> integrate_cc(
    const std::function<double(double)>& f,
    double a, double b,
    double acc = 0.001, double eps = 0.001
) {
    // Maps singular end-points out of existence using x = cos(theta) mapping
    auto f_cc = [&](double theta) {
        double x = (a + b) / 2.0 + (b - a) / 2.0 * std::cos(theta);
        return f(x) * std::sin(theta) * (b - a) / 2.0;
    };
    // Integrate smoothly across finite angular boundaries [0, pi]
    return integrate(f_cc, 0.0, M_PI, acc, eps);
}

// ==========================================
// Infinite Limit Transformation Extension
// ==========================================
inline std::pair<double, double> integrate_inf(
    const std::function<double(double)>& f,
    double a, // Assumes upper integration limit is positive infinity (+inf)
    double acc = 0.001, double eps = 0.001
) {
    // Maps interval [a, inf) to finite region [0, 1] using x = a + (1-t)/t
    auto f_inf = [&](double t) {
        if (t <= 0.0) return 0.0;
        double x = a + (1.0 - t) / t;
        return f(x) / (t * t);
    };
    return integrate_cc(f_inf, 0.0, 1.0, acc, eps);
}

// ==========================================
// The Mathematical Error Function: erf(z)
// ==========================================
inline double erf(double z, double acc = 1e-7, double eps = 1e-7) {
    if (z < 0.0) return -erf(-z, acc, eps);
    
    auto f_erf = [](double x) { return std::exp(-x * x); };
    
    if (z <= 1.0) {
        auto [val, err] = integrate(f_erf, 0.0, z, acc, eps);
        return (2.0 / std::sqrt(M_PI)) * val;
    } else {
        auto f_tail = [&](double t) {
            if (t <= 0.0) return 0.0;
            return std::exp(-std::pow(z + (1.0 - t) / t, 2.0)) / (t * t);
        };
        auto [val, err] = integrate_cc(f_tail, 0.0, 1.0, acc, eps);
        return 1.0 - (2.0 / std::sqrt(M_PI)) * val;
    }
}

} // namespace pp