#include "matrix.hpp"
#include "ode.hpp"
#include <iostream>
#include <fstream>
#include <cmath>

int main() {
    std::cout << "========================================\n";
    std::cout << "TASK C: THE GRAVITATIONAL THREE-BODY PROBLEM\n";
    std::cout << "========================================\n";

    // 12 State Variables mapped linearly into a single vector:
    // y[0,1]   = Body 1 Position (x,y)
    // y[2,3]   = Body 2 Position (x,y)
    // y[4,5]   = Body 3 Position (x,y)
    // y[6,7]   = Body 1 Velocity (vx,vy)
    // y[8,9]   = Body 2 Velocity (vx,vy)
    // y[10,11] = Body 3 Velocity (vx,vy)
    pp::vector y0(12);

    // Initial stable positions (Figure-8 layout configuration)
    double x_init = 0.97000436;
    double y_init = -0.24308753;
    
    y0[0] = -x_init; y0[1] = -y_init; // Body 1
    y0[2] =  x_init; y0[3] =  y_init; // Body 2
    y0[4] = 0.0;     y0[5] = 0.0;     // Body 3

    // Initial conditions for stable momentum (Chenciner/Montgomery velocities)
    double vx = 0.46620531;
    double vy = 0.43236573;
    
    y0[6] = vx;  y0[7] = vy;                    // Body 1
    y0[8] = vx;  y0[9] = vy;                    // Body 2
    y0[10] = -2.0 * vx; y0[11] = -2.0 * vy;     // Body 3 (Total system momentum must equal 0)

    // Newtonian Physics Core: dy/dt execution callback
    pp::ode_func gravity_sys = [](double t, const pp::vector& y) {
        pp::vector dydt(12);
        
        // Map current positions
        double r1[2] = { y[0], y[1] };
        double r2[2] = { y[2], y[3] };
        double r3[2] = { y[4], y[5] };

        // Derivatives of positions are simply velocities
        for(int i = 0; i < 6; i++) dydt[i] = y[i + 6];

        // Gravitational Constants (Assumes uniform body mass m = 1, G = 1)
        double d12_3 = std::pow(std::pow(r2[0]-r1[0],2) + std::pow(r2[1]-r1[1],2), 1.5);
        double d13_3 = std::pow(std::pow(r3[0]-r1[0],2) + std::pow(r3[1]-r1[1],2), 1.5);
        double d23_3 = std::pow(std::pow(r3[0]-r2[0],2) + std::pow(r3[1]-r2[1],2), 1.5);

        // Body 1 Accelerations (dv1/dt)
        dydt[6] = (r2[0] - r1[0])/d12_3 + (r3[0] - r1[0])/d13_3;
        dydt[7] = (r2[1] - r1[1])/d12_3 + (r3[1] - r1[1])/d13_3;

        // Body 2 Accelerations (dv2/dt)
        dydt[8] = (r1[0] - r2[0])/d12_3 + (r3[0] - r2[0])/d23_3;
        dydt[9] = (r1[1] - r2[1])/d12_3 + (r3[1] - r2[1])/d23_3;

        // Body 3 Accelerations (dv3/dt)
        dydt[10] = (r1[0] - r3[0])/d13_3 + (r2[0] - r3[0])/d23_3;
        dydt[11] = (r1[1] - r3[1])/d13_3 + (r2[1] - r3[1])/d23_3;

        return dydt;
    };

    // Integrate exactly 1 full orbit time period (T ≈ 6.3259)
    double T_period = 6.3259;
    std::cout << "Integrating stable gravitational orbits using adaptive step sizing...\n";
    pp::ode_path orbit_data = pp::driver(gravity_sys, 0.0, y0, T_period, 0.01, 1e-5, 1e-5);
    
    // Dump spatial geometry trace tracks for Gnuplot visualization
    std::ofstream out("orbit_path.txt");
    for (size_t i = 0; i < orbit_data.t.size(); i++) {
        const auto& y = orbit_data.y[i];
        // Output format: t | x1,y1 | x2,y2 | x3,y3
        out << orbit_data.t[i] << " " 
            << y[0] << " " << y[1] << " "
            << y[2] << " " << y[3] << " "
            << y[4] << " " << y[5] << "\n";
    }
    
    std::cout << "Process finished. Path points saved to orbit_path.txt\n";
    return 0;
}