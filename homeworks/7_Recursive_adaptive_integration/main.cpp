#include "adaptive_int.hpp"
#include <iostream>
#include <fstream>
#include <cmath>
#include <iomanip>

int main() {
    std::cout << "========================================\n";
    std::cout << "TASK A & B: VALIDATION OF INTEGRATION RULES\n";
    std::cout << "========================================\n";

    // 1. Core integration testing (Singular functions)
    // We use 1/sqrt(x) to test the endpoint singularity safely
    auto f1 = [](double x) { pp::ncalls++; return 1.0 / std::sqrt(x); };
    
    pp::ncalls = 0;
    // Ordinary integrator starts slightly off 0 to avoid dividing by zero
    auto [val1, err1] = pp::integrate(f1, 1e-4, 1.0, 1e-4, 1e-4); 
    std::cout << "Ordinary Int:  Int(1/sqrt(x)) from 1e-4 = " << val1 << " (Calls: " << pp::ncalls << ")\n";

    pp::ncalls = 0;
    // Clenshaw-Curtis safely maps the exact 0.0 out of existence!
    auto [val2, err2] = pp::integrate_cc(f1, 0.0, 1.0, 1e-4, 1e-4); 
    std::cout << "Clenshaw-Curt: Int(1/sqrt(x)) from 0.0  = " << val2 << " (Calls: " << pp::ncalls << ")\n";
    std::cout << "*(Notice how Clenshaw-Curtis completely optimized away the endpoint singularity!)*\n\n";
    
    // 2. Infinite boundary testing
    auto f_gauss = [](double x) { return std::exp(-x * x); };
    auto [val_inf, err_inf] = pp::integrate_inf(f_gauss, 0.0, 1e-6, 1e-6);
    std::cout << "Infinite Int:  Int_0^inf(exp(-x^2)) = " << val_inf << " (True val: " << std::sqrt(M_PI)/2.0 << ")\n\n";

    // ==========================================
    // SAMPLING DATA GENERATION FOR PLOTTING
    // ==========================================
    
    // Sample 1: The standard curve mapping of erf(z)
    std::ofstream out_erf("erf_curve.txt");
    for (double z = -3.0; z <= 3.0; z += 0.1) {
        out_erf << z << " " << pp::erf(z) << "\n";
    }

    // Sample 2: Log-Log accuracy conversion monitoring at erf(1)
    double true_erf1 = 0.84270079294971486934;
    std::ofstream out_acc("accuracy_decay.txt");
    
    double test_acc = 0.1;
    for (int i = 0; i < 8; i++) {
        double calc_erf1 = pp::erf(1.0, test_acc, 0.0); // eps = 0 as requested
        double diff = std::abs(calc_erf1 - true_erf1);
        out_acc << test_acc << " " << (diff < 1e-16 ? 1e-16 : diff) << "\n";
        test_acc /= 10.0;
    }

    std::cout << "Calculations complete. Script results exported to data paths.\n";
    return 0;
}