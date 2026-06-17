#include "matrix.hpp"
#include "qr.hpp"
#include "lsfit.hpp"
#include <iostream>
#include <vector>
#include <functional>
#include <cmath>
#include <fstream>
#include <iomanip>

int main() {
    // 1. Historical Data (Rutherford and Soddy - ThX)
    std::vector<double> t_raw  = {1, 2, 3, 4, 6, 9, 10, 13, 15};
    std::vector<double> y_raw  = {117, 100, 88, 72, 53, 29.5, 25.2, 15.2, 11.1};
    std::vector<double> dy_raw = {6, 5, 4, 4, 4, 3, 3, 2, 2};

    int n = t_raw.size();
    pp::vector t(n), y(n), dy(n);
    
    // Transform exponential data to linear log space
    for(int i = 0; i < n; i++) {
        t[i]  = t_raw[i];
        y[i]  = std::log(y_raw[i]);
        dy[i] = dy_raw[i] / y_raw[i]; // Propagation of uncertainty
    }

    // 2. Define fitting functions: F(t) = c0*1 + c1*t
    std::vector<std::function<double(double)>> fs = {
        [](double x) { return 1.0; },
        [](double x) { return x; }
    };

    // 3. Perform the Least-Squares Fit
    auto result = pp::lsfit(fs, t, y, dy);
    pp::vector c = std::get<0>(result);
    pp::matrix Sigma = std::get<1>(result);

    double c0  = c[0];
    double c1  = c[1];
    double dc0 = std::sqrt(Sigma(0,0)); // Uncertainty is sqrt of diagonal
    double dc1 = std::sqrt(Sigma(1,1));

    // 4. Calculate Physics Properties
    double lambda   = -c1;
    double dlambda  = dc1;
    double T_half   = std::log(2.0) / lambda;
    double dT_half  = std::log(2.0) / (lambda * lambda) * dlambda;

    std::cout << "========================================\n";
    std::cout << "      Radium-224 (ThX) Fit Results      \n";
    std::cout << "========================================\n";
    std::cout << "c0 (ln a)      = " << c0 << " +/- " << dc0 << "\n";
    std::cout << "c1 (-lambda)   = " << c1 << " +/- " << dc1 << "\n";
    std::cout << "Half-life      = " << T_half << " days +/- " << dT_half << " days\n";
    std::cout << "Modern 224Ra   = 3.63 days\n";
    std::cout << "Matches within error? " 
              << (std::abs(T_half - 3.63) <= dT_half ? "YES" : "NO") << "\n";
    std::cout << "========================================\n";

    // 5. Output Data for Gnuplot
    std::ofstream out_data("data.txt");
    for(int i = 0; i < n; i++) {
        out_data << t[i] << " " << y[i] << " " << dy[i] << "\n";
    }
    
    std::ofstream out_fit("fit.txt");
    for(double x = 0; x <= 16; x += 0.5) {
        double fit = c0 + c1*x;
        // The 4 combinations of uncertainties
        double f1 = (c0 + dc0) + (c1 + dc1)*x;
        double f2 = (c0 + dc0) + (c1 - dc1)*x;
        double f3 = (c0 - dc0) + (c1 + dc1)*x;
        double f4 = (c0 - dc0) + (c1 - dc1)*x;
        out_fit << x << " " << fit << " " << f1 << " " << f2 << " " << f3 << " " << f4 << "\n";
    }

    return 0;
}