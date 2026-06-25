#include "matrix.hpp"
#include "bbpso.hpp"
#include <iostream>
#include <fstream>
#include <cmath>
#include <iomanip>

int main() {
    std::cout << "========================================\n";
    std::cout << "EXAM PROJECT: BARE-BONES PARTICLE SWARM\n";
    std::cout << "========================================\n";

    // 1. Test on Rosenbrock Valley Function
    pp::objective_func rosenbrock = [](const pp::vector& v) {
        double x = v[0], y = v[1];
        return std::pow(1.0 - x, 2) + 100.0 * std::pow(y - x * x, 2);
    };

    pp::vector min_bounds(2), max_bounds(2);
    min_bounds[0] = -5.0; min_bounds[1] = -5.0;
    max_bounds[0] =  5.0; max_bounds[1] =  5.0;

    pp::vector global_min_rosen = pp::bbpso(rosenbrock, min_bounds, max_bounds, 40, 500);
    std::cout << "Rosenbrock Swarm Global Minimum found at: (" 
              << global_min_rosen[0] << ", " << global_min_rosen[1] << ")\n";
    std::cout << "   Value: " << rosenbrock(global_min_rosen) << " (Expected: 0.0 at (1,1))\n\n";

    // 2. Test on Himmelblau Function
    pp::objective_func himmelblau = [](const pp::vector& v) {
        double x = v[0], y = v[1];
        return std::pow(x * x + y - 11.0, 2) + std::pow(x + y * y - 7.0, 2);
    };

    pp::vector global_min_himmel = pp::bbpso(himmelblau, min_bounds, max_bounds, 40, 500);
    std::cout << "Himmelblau Swarm Global Minimum found at: (" 
              << global_min_himmel[0] << ", " << global_min_himmel[1] << ")\n";
    std::cout << "   Value: " << himmelblau(global_min_himmel) << " (Expected: 0.0)\n\n";

    // 3. Export convergence tracking profiles for reporting
    std::ofstream out_conv("swarm_convergence.txt");
    for (int steps = 10; steps <= 300; steps += 10) {
        pp::vector res = pp::bbpso(rosenbrock, min_bounds, max_bounds, 40, steps);
        out_conv << steps << " " << rosenbrock(res) << "\n";
    }
    std::cout << "Convergence verification profile saved to swarm_convergence.txt\n";

    return 0;
}