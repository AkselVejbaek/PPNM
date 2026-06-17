#pragma once
#include <vector>
#include <cassert>
#include <functional>
#include <utility>

namespace pp {

// ==========================================
// BINARY SEARCH (Shared)
// ==========================================
inline int binsearch(const std::vector<double>& x, double z) {
    assert(z >= x.front() && z <= x.back());
    int i = 0, j = x.size() - 1;
    while (j - i > 1) {
        int mid = (i + j) / 2;
        if (z > x[mid]) i = mid;
        else j = mid;
    }
    return i;
}

// ==========================================
// TASK A: LINEAR SPLINE
// ==========================================
inline double linterp(const std::vector<double>& x, const std::vector<double>& y, double z) {
    int i = binsearch(x, z);
    double dx = x[i+1] - x[i];
    assert(dx > 0);
    double dy = y[i+1] - y[i];
    return y[i] + dy / dx * (z - x[i]);
}

inline double linterpInteg(const std::vector<double>& x, const std::vector<double>& y, double z) {
    int idx = binsearch(x, z);
    double sum = 0.0;
    
    // Integrate full intervals up to idx
    for (int i = 0; i < idx; i++) {
        double dx = x[i+1] - x[i];
        sum += dx * (y[i+1] + y[i]) / 2.0; // Area of trapezoid
    }
    
    // Integrate the final partial interval up to z
    double dx_last = z - x[idx];
    double y_z = linterp(x, y, z);
    sum += dx_last * (y[idx] + y_z) / 2.0;
    
    return sum;
}

// ==========================================
// TASK B: QUADRATIC SPLINE (OOP Style)
// ==========================================
struct qspline {
    std::vector<double> x, y, b, c;

    qspline(const std::vector<double>& x_in, const std::vector<double>& y_in)
        : x(x_in), y(y_in), b(x_in.size()-1), c(x_in.size()-1) 
    {
        int n = x.size();
        std::vector<double> p(n-1), dx(n-1);
        
        for (int i = 0; i < n - 1; i++) {
            dx[i] = x[i+1] - x[i];
            p[i] = (y[i+1] - y[i]) / dx[i];
        }
        
        // Forward recursion (arbitrarily setting c0 = 0)
        c[0] = 0.0;
        b[0] = p[0];
        for (int i = 0; i < n - 2; i++) {
            c[i] = (p[i] - b[i]) / dx[i];
            b[i+1] = 2.0 * p[i] - b[i]; // Derivative continuity
        }
        c[n-2] = (p[n-2] - b[n-2]) / dx[n-2];
    }

    double eval(double z) const {
        int i = binsearch(x, z);
        double h = z - x[i];
        return y[i] + h * (b[i] + h * c[i]);
    }

    double deriv(double z) const {
        int i = binsearch(x, z);
        double h = z - x[i];
        return b[i] + 2.0 * c[i] * h;
    }

    double integ(double z) const {
        int idx = binsearch(x, z);
        double sum = 0.0;
        
        // Integrate full intervals
        for (int i = 0; i < idx; i++) {
            double h = x[i+1] - x[i];
            sum += h * (y[i] + h * (b[i] / 2.0 + h * c[i] / 3.0));
        }
        
        // Integrate final partial interval
        double h = z - x[idx];
        sum += h * (y[idx] + h * (b[idx] / 2.0 + h * c[idx] / 3.0));
        return sum;
    }
};

// ==========================================
// TASK C: QUADRATIC SPLINE (Functional Style)
// ==========================================
inline std::function<double(double)> make_qspline(std::vector<double> x, std::vector<double> y) {
    int n = x.size();
    std::vector<double> b(n - 1), c(n - 1), p(n - 1), dx(n - 1);
    
    for (int i = 0; i < n - 1; i++) {
        dx[i] = x[i+1] - x[i];
        p[i] = (y[i+1] - y[i]) / dx[i];
    }
    
    b[0] = p[0];
    c[0] = 0.0;
    for (int i = 0; i < n - 2; i++) {
        b[i+1] = 2.0 * p[i] - b[i];
        c[i] = (p[i] - b[i]) / dx[i];
    }
    c[n-2] = (p[n-2] - b[n-2]) / dx[n-2];

    // Explicitly moving the vectors into the lambda capture environment!
    return [x = std::move(x), y = std::move(y), b = std::move(b), c = std::move(c)](double z) {
        int i = 0, j = x.size() - 1;
        while (j - i > 1) {
            int mid = (i + j) / 2;
            if (z > x[mid]) i = mid;
            else j = mid;
        }
        double h = z - x[i];
        return y[i] + h * (b[i] + h * c[i]);
    };
}

} // namespace pp