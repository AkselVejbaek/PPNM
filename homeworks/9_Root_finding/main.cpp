#include "matrix.hpp"
#include "ode.hpp"
#include "roots.hpp"
#include <iostream>
#include <fstream>
#include <cmath>
#include <iomanip>

int main() {
    std::cout << "========================================\n";
    std::cout << "TASK A: GRADIENT ROOT FINDING OPTIMIZATION\n";
    std::cout << "========================================\n";

    // 1. Rosenbrock Valley Minimum Search
    // f(x,y) = (1-x)^2 + 100*(y-x^2)^2
    pp::vector_func rosenbrock_grad = [](const pp::vector& v) {
        double x = v[0], y = v[1];
        pp::vector grad(2);
        grad[0] = -2.0 * (1.0 - x) - 400.0 * x * (y - x * x);
        grad[1] = 200.0 * (y - x * x);
        return grad;
    };
    // Change this line:
    pp::vector rosen_start(2); rosen_start[0] = 0.5; rosen_start[1] = 0.5;
    pp::vector rosen_min = pp::newton(rosenbrock_grad, rosen_start);
    std::cout << "Rosenbrock Min found at: (" << rosen_min[0] << ", " << rosen_min[1] << ")\n";
    std::cout << "   (Expected Global Minimum at (1, 1))\n\n";

    // 2. Himmelblau's Function Minimum Search
    // f(x,y) = (x^2 + y - 11)^2 + (x + y^2 - 7)^2
    pp::vector_func himmelblau_grad = [](const pp::vector& v) {
        double x = v[0], y = v[1];
        pp::vector grad(2);
        grad[0] = 4.0 * x * (x * x + y - 11.0) + 2.0 * (x + y * y - 7.0);
        grad[1] = 2.0 * (x * x + y - 11.0) + 4.0 * y * (x + y * y - 7.0);
        return grad;
    };
    pp::vector himmel_start(2); himmel_start[0] = 3.5; himmel_start[1] = 2.5;
    pp::vector himmel_min = pp::newton(himmelblau_grad, himmel_start);
    std::cout << "Himmelblau Min found at: (" << himmel_min[0] << ", " << himmel_min[1] << ")\n";
    std::cout << "   (Expected one of the roots at (3, 2))\n\n";

    std::cout << "========================================\n";
    std::cout << "TASK B: SCHRÖDINGER HYDROGEN SHOOTING METHOD\n";
    std::cout << "========================================\n";

    double rmin = 1e-4;
    double rmax = 8.0;

    // Shooting objective auxiliary wrapper matching M(E) = FE(rmax) = 0
    pp::vector_func shooting_objective = [rmin, rmax](const pp::vector& E_vec) {
        double E = E_vec[0];
        
        // Define Radial system: f'' = 2 * (-1/r - E) * f
        // State variables: y[0] = f(r), y[1] = f'(r)
        pp::ode_func schrodinger_ode = [E](double r, const pp::vector& y) {
            pp::vector dydr(2);
            dydr[0] = y[1];
            dydr[1] = 2.0 * (-1.0 / r - E) * y[0];
            return dydr;
        };

        // Initial Boundary conditions at rmin: f(rmin) = rmin - rmin^2
        pp::vector y_init(2);
        y_init[0] = rmin - rmin * rmin;
        y_init[1] = 1.0 - 2.0 * rmin; // Derivative matching

        // Integrate from rmin across to rmax
        pp::ode_path trajectory = pp::driver(schrodinger_ode, rmin, y_init, rmax, 0.01, 1e-7, 1e-7);
        
        pp::vector residual(1);
        residual[0] = trajectory.y.back()[0]; // Capture FE(rmax)
        return residual;
    };

    // Guess an initial energy state of -0.2 Hartree units
    // Change this line:
    pp::vector energy_guess(1); energy_guess[0] = -0.6; // Start closer to -0.5!
    pp::vector converged_energy = pp::newton(shooting_objective, energy_guess, 1e-5);
    
    std::cout << std::setprecision(6);
    std::cout << "Converged Hydrogen Ground State Energy E0: " << converged_energy[0] << " Hartree\n";
    std::cout << "   (Analytical exact physical target value: -0.5 Hartree)\n\n";

    // Re-run the final physical wave function with our converged energy to output values
    double E_final = converged_energy[0];
    pp::ode_func final_ode = [E_final](double r, const pp::vector& y) {
        pp::vector dydr(2);
        dydr[0] = y[1];
        dydr[1] = 2.0 * (-1.0 / r - E_final) * y[0];
        return dydr;
    };
    pp::vector y_init(2);
    y_init[0] = rmin - rmin * rmin; y_init[1] = 1.0 - 2.0 * rmin;
    pp::ode_path wavefunction_data = pp::driver(final_ode, rmin, y_init, rmax, 0.01, 1e-7, 1e-7);

    // Save outputs for visual validation comparisons
    std::ofstream out_wave("wavefunction.txt");
    for (size_t i = 0; i < wavefunction_data.t.size(); i++) {
        double r = wavefunction_data.t[i];
        double numerical_f = wavefunction_data.y[i][0];
        double exact_f = r * std::exp(-r); // Exact state check formula f0(r) = r*e^-r
        out_wave << r << " " << numerical_f << " " << exact_f << "\n";
    }
    std::cout << "Wavefunction profile exported successfully to wavefunction.txt\n";

    return 0;
}