#include "mc.hpp"
#include <iostream>
#include <fstream>
#include <cmath>
#include <iomanip>

int main() {
    std::cout << "========================================\n";
    std::cout << "TASK A: PLAIN MC & ERROR SCALING CHECKS\n";
    std::cout << "========================================\n";

    // 1. Calculate Area of Unit Circle: x^2 + y^2 <= 1
    auto circle_f = [](const pp::vector& x) {
        return (x[0]*x[0] + x[1]*x[1] <= 1.0) ? 1.0 : 0.0;
    };
    pp::vector a_circle(2); a_circle[0] = -1.0; a_circle[1] = -1.0;
    pp::vector b_circle(2); b_circle[0] =  1.0; b_circle[1] =  1.0;
    
    double true_circle_area = M_PI;

    std::ofstream out_err("mc_error_scaling.txt");
    for (int N = 500; N <= 50000; N += 500) {
        auto [calc_area, est_err] = pp::plain_mc(circle_f, a_circle, b_circle, N);
        auto [q_area, q_err] = pp::quasi_mc(circle_f, a_circle, b_circle, N);
        
        double actual_plain_err = std::abs(calc_area - true_circle_area);
        double actual_quasi_err = std::abs(q_area - true_circle_area);
        
        out_err << N << " " << est_err << " " << actual_plain_err << " " << actual_quasi_err << "\n";
    }
    std::cout << "Circle tracking complete. Data saved for error scaling graphs.\n";

    // 2. Calculate Volume of 3D Ellipsoid: x^2/1^2 + y^2/2^2 + z^2/3^2 <= 1
    auto ellipsoid_f = [](const pp::vector& p) {
        return (p[0]*p[0]/1.0 + p[1]*p[1]/4.0 + p[2]*p[2]/9.0 <= 1.0) ? 1.0 : 0.0;
    };
    pp::vector a_ell(3); a_ell[0] = -1.0; a_ell[1] = -2.0; a_ell[2] = -3.0;
    pp::vector b_ell(3); b_ell[0] =  1.0; b_ell[1] =  2.0; b_ell[2] =  3.0;
    
    auto [ell_vol, ell_err] = pp::plain_mc(ellipsoid_f, a_ell, b_ell, 100000);
    double true_ell_vol = (4.0 / 3.0) * M_PI * 1.0 * 2.0 * 3.0;
    std::cout << "Ellipsoid Volume Calc:  " << ell_vol << " +/- " << ell_err << "\n";
    std::cout << "True Ellipsoid Volume:  " << true_ell_vol << "\n\n";

    std::cout << "========================================\n";
    std::cout << "TASK B: DIFFICULT SINGULAR INTEGRAL EVAL\n";
    std::cout << "========================================\n";

    auto singular_f = [](const pp::vector& p) {
        double denom = 1.0 - std::cos(p[0]) * std::cos(p[1]) * std::cos(p[2]);
        if (std::abs(denom) < 1e-9) return 0.0;
        return 1.0 / (denom * std::pow(M_PI, 3));
    };
    pp::vector a_sing(3); // initialized to 0,0,0
    pp::vector b_sing(3); b_sing[0] = M_PI; b_sing[1] = M_PI; b_sing[2] = M_PI;

    int N_samples = 200000;
    double true_sing_val = 1.3932039296856768;

    // i) Using standard custom LCG
    auto [val_lcg, err_lcg] = pp::plain_mc(singular_f, a_sing, b_sing, N_samples);

    // ii) Using Standard Library Generator
    std::mt19937 std_rand(42);
    std::uniform_real_distribution<double> dist(0.0, 1.0);
    auto plain_mc_std = [&](int N) {
        double sum = 0.0; pp::vector v(3);
        for(int i=0; i<N; i++){
            for(int d=0; d<3; d++) v[d] = a_sing[d] + (b_sing[d]-a_sing[d])*dist(std_rand);
            sum += singular_f(v);
        }
        return (sum/N) * std::pow(M_PI, 3);
    };
    double val_std = plain_mc_std(N_samples);

    // iii) Using Quasi-Monte Carlo Low-Discrepancy Sequence
    auto [val_qmc, err_qmc] = pp::quasi_mc(singular_f, a_sing, b_sing, N_samples);

    // iv) Task C: Stratified Recursive Sampling
    auto [val_strat, err_strat] = pp::stratified_mc(singular_f, a_sing, b_sing, N_samples, 1000);

    std::cout << std::setprecision(10);
    std::cout << "True Integral Value:   " << true_sing_val << "\n";
    std::cout << "i) Custom LCG Plain:   " << val_lcg << " (Err: " << std::abs(val_lcg - true_sing_val) << ")\n";
    std::cout << "ii) std::mt19937 Plain: " << val_std   << " (Err: " << std::abs(val_std - true_sing_val) << ")\n";
    std::cout << "iii) Quasi-MC Halton:  " << val_qmc   << " (Err: " << std::abs(val_qmc - true_sing_val) << ")\n";
    std::cout << "iv) Stratified MC:     " << val_strat << " (Err: " << std::abs(val_strat - true_sing_val) << ")\n\n";

    return 0;
}