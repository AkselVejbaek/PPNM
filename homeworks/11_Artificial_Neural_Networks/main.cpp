#include "matrix.hpp"
#include "minimize.hpp"
#include "ann.hpp"
#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>

int main() {
    std::cout << "========================================\n";
    std::cout << "TASK A & B: FUNCTION APPROXIMATION & ANALYTIC CALCULUS\n";
    std::cout << "========================================\n";

    // Target Function: g(x) = Cos(5*x - 1) * Exp(-x*x) [cite: 53]
    auto g = [](double x) { return std::cos(5.0 * x - 1.0) * std::exp(-x * x); };

    // Generate training nodes sampled on [-1, 1] [cite: 54]
    std::vector<double> x_train, y_train;
    for (double x = -1.0; x <= 1.0; x += 0.15) {
        x_train.push_back(x);
        y_train.push_back(g(x));
    }

    // Construct network utilizing 5 hidden wavelet units [cite: 6]
    pp::ann net(5);
    net.train(x_train, y_train); // Run network optimization 

    // Export curve parameters for visualization plotting checks
    std::ofstream out_interp("ann_interpolation.txt");
    for (double x = -1.0; x <= 1.0; x += 0.02) {
        out_interp << x << " " << g(x) << " " << net.response(x) << " " 
                   << net.derivative(x) << " " << net.anti_derivative(x) << "\n";
    }
    std::cout << "Network successfully trained on g(x). Predictions saved.\n\n";

    std::cout << "========================================\n";
    std::cout << "TASK C: NEURAL DIFFERENTIAL EQUATION SOLVER\n";
    std::cout << "========================================\n";

    // Solving ODE: y'' + y = 0  -> Analytic ground truth solution: y = sin(x)
    // Boundary conditions at c=0: y(0)=0, y'(0)=1 [cite: 60]
    double c = 0.0;
    double yc = 0.0;
    double dyc = 1.0;
    
    pp::ann ode_net(6); // 4 Wavelet nodes to fit the harmonic wave

    // Define collocation sampling bounds across domain [0, 2]
    std::vector<double> x_colloc;
    for (double x = 0.0; x <= 2.0; x += 0.2) x_colloc.push_back(x);

    // Objective function minimizing physics residual + boundary conditions [cite: 62]
    pp::cost_func pynn_cost = [&](const pp::vector& trial_p) {
        pp::vector original_p = ode_net.p;
        ode_net.p = trial_p;

        double physics_integral = 0.0;
        double dx = 0.1; // Simple numerical quadrature step for the integration penalty [cite: 66]
        for (double x = 0.0; x <= 2.0; x += dx) {
            // For y'' + y = 0, residual error tracking = y'' + y [cite: 58]
            double residual = ode_net.second_derivative(x) + ode_net.response(x);
            physics_integral += residual * residual * dx; // [cite: 66]
        }

        // Boundary penalties: alpha=50.0, beta=50.0 [cite: 66, 67]
        double boundary_y = std::pow(ode_net.response(c) - yc, 2.0); // [cite: 66]
        double boundary_dy = std::pow(ode_net.derivative(c) - dyc, 2.0); // [cite: 66]

        ode_net.p = original_p; // Reset
        return physics_integral + 50.0 * boundary_y + 50.0 * boundary_dy; // [cite: 66]
    };

    // Update max_iter from 800 to 2000 inside main.cpp
    auto [optimized_p, steps] = pp::minimize(pynn_cost, ode_net.p, 1e-5, 2000);
    ode_net.p = optimized_p;

    std::ofstream out_ode("ann_ode_solution.txt");
    for (double x = 0.0; x <= 2.0; x += 0.04) {
        out_ode << x << " " << std::sin(x) << " " << ode_net.response(x) << "\n";
    }
    std::cout << "Physics-Informed training complete. Model traces saved.\n";

    return 0;
}