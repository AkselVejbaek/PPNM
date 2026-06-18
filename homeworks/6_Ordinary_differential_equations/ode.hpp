#pragma once
#include "matrix.hpp"
#include <vector>
#include <functional>
#include <cmath>
#include <cassert>

namespace pp {

using ode_func = std::function<pp::vector(double, const pp::vector&)>;

struct ode_path {
    std::vector<double> t;
    std::vector<pp::vector> y;
};

// ==========================================
// TASK A: Embedded RK12 Stepper (Loop-Based)
// ==========================================
inline void rkstep12(
    const ode_func& f,
    double t,
    const pp::vector& yt,
    double h,
    pp::vector& yh,
    pp::vector& err
) {
    int n = yt.size();
    
    // Stage 1: Standard Euler step components
    pp::vector k0 = f(t, yt);
    
    // Manually calculate y_mid = yt + k0 * (h / 2.0)
    pp::vector y_mid(n);
    for (int i = 0; i < n; i++) {
        y_mid[i] = yt[i] + k0[i] * (h / 2.0);
    }
    
    // Stage 2: Midpoint step evaluation
    pp::vector k1 = f(t + h / 2.0, y_mid);
    
    // Higher-order prediction: yh = yt + k1 * h
    // Lower-order prediction: y_euler = yt + k0 * h
    // Error estimate: err = yh - y_euler = (k1 - k0) * h
    for (int i = 0; i < n; i++) {
        yh[i] = yt[i] + k1[i] * h;
        err[i] = (k1[i] - k0[i]) * h;
    }
}

// ==========================================
// TASK B: Adaptive Driver (Loop-Based)
// ==========================================
inline ode_path driver(
    const ode_func& f,
    double a,
    const pp::vector& ya,
    double b,
    double h_init,
    double acc,
    double eps
) {
    ode_path path;
    double t = a;
    pp::vector y = ya;
    double h = h_init;
    
    path.t.push_back(t);
    path.y.push_back(y);
    
    int n = y.size();
    
    while (t < b) {
        if (t + h > b) h = b - t;
        
        pp::vector yh(n);
        pp::vector err(n);
        
        rkstep12(f, t, y, h, yh, err);
        
        // Manually calculate norms since .norm() or operators might be missing/unreliable
        double err_sum_sq = 0.0;
        double yh_sum_sq = 0.0;
        for (int i = 0; i < n; i++) {
            err_sum_sq += err[i] * err[i];
            yh_sum_sq += yh[i] * yh[i];
        }
        double error_norm = std::sqrt(err_sum_sq);
        double yh_norm = std::sqrt(yh_sum_sq);
        
        double tolerance = acc + eps * yh_norm;
        
        if (error_norm <= tolerance) {
            t += h;
            y = yh;
            
            path.t.push_back(t);
            path.y.push_back(y);
            
            if (error_norm > 1e-12) {
                h *= std::pow(tolerance / error_norm, 0.25) * 0.95;
            } else {
                h *= 2.0; 
            }
        } else {
            h *= std::pow(tolerance / error_norm, 0.25) * 0.95;
        }
        
        if (std::abs(h) < 1e-15) {
            assert(false && "Step size shrunk below arithmetic machine limit!");
        }
    }
    
    return path;
}

} // namespace pp