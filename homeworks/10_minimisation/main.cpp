#include "matrix.hpp"
#include "minimize.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <iomanip>

int main() {
    std::cout << "========================================\n";
    std::cout << "TASK A: LANDSCAPE MINIMIZATION TESTS\n";
    std::cout << "========================================\n";

    // 1. Rosenbrock Valley Function
    pp::cost_func rosenbrock = [](const pp::vector& v) {
        double x = v[0], y = v[1];
        return std::pow(1.0 - x, 2) + 100.0 * std::pow(y - x * x, 2);
    };
    pp::vector start_rosen(2); start_rosen[0] = 0.5; start_rosen[1] = 0.5;
    auto [min_rosen, steps_rosen] = pp::minimize(rosenbrock, start_rosen);
    std::cout << "Rosenbrock Min: (" << min_rosen[0] << ", " << min_rosen[1] << ") in " << steps_rosen << " steps.\n";

    // 2. Himmelblau Function
    pp::cost_func himmelblau = [](const pp::vector& v) {
        double x = v[0], y = v[1];
        return std::pow(x * x + y - 11.0, 2) + std::pow(x + y * y - 7.0, 2);
    };
    pp::vector start_himmel(2); start_himmel[0] = 3.5; start_himmel[1] = 2.5;
    auto [min_himmel, steps_himmel] = pp::minimize(himmelblau, start_himmel);
    std::cout << "Himmelblau Min: (" << min_himmel[0] << ", " << min_himmel[1] << ") in " << steps_himmel << " steps.\n\n";

    std::cout << "========================================\n";
    std::cout << "TASK B: HIGGS BOSON DISCOVERY RESONANCE FIT\n";
    std::cout << "========================================\n";

    // Parse data arrays from standard input (stdin)
    std::vector<double> energy, signal, error;
    double E, sig, err;
    while (std::cin >> E >> sig >> err) {
        energy.push_back(E);
        signal.push_back(sig);
        error.push_back(err);
    }

    // Target Deviation Chi-squared Function D(m, Gamma, A)
    // Parameter vector indices mapping: p[0] = mass (m), p[1] = width (Gamma), p[2] = amplitude (A)
    pp::cost_func higgs_deviation = [&](const pp::vector& p) {
        double m = p[0], gamma = p[1], A = p[2];
        double sum_sq = 0.0;
        for (size_t i = 0; i < energy.size(); i++) {
            double F = A / (std::pow(energy[i] - m, 2) + std::pow(gamma, 2) / 4.0);
            double residual = (F - signal[i]) / error[i];
            sum_sq += residual * residual;
        }
        return sum_sq;
    };

    // Initial Guess Setup: Mass close to peak (~125), broad width (~2), clean scaling scale (~10)
    pp::vector p_guess(3);
    p_guess[0] = 125.0; p_guess[1] = 2.0; p_guess[2] = 10.0;

    auto [p_fit, fit_steps] = pp::minimize(higgs_deviation, p_guess, 1e-4);

    std::cout << std::setprecision(4) << std::fixed;
    std::cout << "Converged Breit-Wigner Parameters:\n";
    std::cout << "   Higgs Boson Mass (m):       " << p_fit[0] << " GeV/c^2\n";
    std::cout << "   Experimental Width (Gamma): " << p_fit[1] << " GeV\n";
    std::cout << "   Scale Amplitude Factor (A): " << p_fit[2] << "\n";
    std::cout << "   Calculated in " << fit_steps << " optimization loops.\n\n";

    // Export curve plotting records
    std::ofstream out_fit("higgs_fit_curve.txt");
    double m = p_fit[0], gamma = p_fit[1], A = p_fit[2];
    for (double e = 100.0; e <= 160.0; e += 0.2) {
        double F = A / (std::pow(e - m, 2) + std::pow(gamma, 2) / 4.0);
        out_fit << e << " " << F << "\n";
    }

    return 0;
}