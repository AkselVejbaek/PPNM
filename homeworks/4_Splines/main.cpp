#include "spline.hpp"
#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>

int main() {
    std::cout << "========================================\n";
    std::cout << "TASK B: QUADRATIC SPLINE DEBUGGING\n";
    std::cout << "========================================\n";
    
    std::vector<double> x_dbg = {1, 2, 3, 4, 5};
    std::vector<double> y1 = {1, 1, 1, 1, 1};
    std::vector<double> y2 = {1, 2, 3, 4, 5};
    std::vector<double> y3 = {1, 4, 9, 16, 25};

    pp::qspline qs1(x_dbg, y1);
    std::cout << "y = 1   -> b[0]=" << qs1.b[0] << ", c[0]=" << qs1.c[0] << " (Expect 0, 0)\n";
    
    pp::qspline qs2(x_dbg, y2);
    std::cout << "y = x   -> b[0]=" << qs2.b[0] << ", c[0]=" << qs2.c[0] << " (Expect 1, 0)\n";
    
    pp::qspline qs3(x_dbg, y3);
    std::cout << "y = x^2 -> b[0]=" << qs3.b[0] << ", c[0]=" << qs3.c[0] << " (Expect 3, 0 due to c[0]=0 assumption)\n\n";

    // ==========================================
    // DATA GENERATION FOR PLOTTING
    // ==========================================
    
    // 1. Data points: cos(x) for Linear Spline
    std::vector<double> x_lin, y_lin;
    std::ofstream out_lin_data("lin_data.txt");
    for (int i = 0; i <= 9; i++) {
        x_lin.push_back(i);
        y_lin.push_back(std::cos(i));
        out_lin_data << i << " " << std::cos(i) << "\n";
    }
    
    std::ofstream out_lin_fit("lin_fit.txt");
    for (double z = 0.0; z <= 9.0; z += 0.1) {
        double interp = pp::linterp(x_lin, y_lin, z);
        double integ  = pp::linterpInteg(x_lin, y_lin, z);
        out_lin_fit << z << " " << interp << " " << integ << "\n";
    }

    // 2. Data points: sin(x) for Quadratic Spline
    std::vector<double> x_quad, y_quad;
    std::ofstream out_quad_data("quad_data.txt");
    for (int i = 0; i <= 9; i++) {
        x_quad.push_back(i);
        y_quad.push_back(std::sin(i));
        out_quad_data << i << " " << std::sin(i) << "\n";
    }

    pp::qspline qs(x_quad, y_quad);
    
    // Also test Task C Functional Spline!
    auto qs_func = pp::make_qspline(x_quad, y_quad);

    std::ofstream out_quad_fit("quad_fit.txt");
    for (double z = 0.0; z <= 9.0; z += 0.1) {
        double interp = qs.eval(z);
        double deriv  = qs.deriv(z);
        double integ  = qs.integ(z);
        
        // Assert functional spline matches OOP exactly
        assert(std::abs(interp - qs_func(z)) < 1e-9);

        out_quad_fit << z << " " << interp << " " << deriv << " " << integ << "\n";
    }

    std::cout << "Data files generated successfully. Ready to plot!\n";
    return 0;
}