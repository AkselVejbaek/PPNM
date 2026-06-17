#include "matrix.hpp"
#include "evd.hpp"
#include <iostream>
#include <string>
#include <cstdlib>
#include <vector>
#include <algorithm>

// Helper to calculate transpose
pp::matrix transpose(const pp::matrix& A) {
    pp::matrix At(A.size2(), A.size1());
    for (int i = 0; i < A.size1(); i++) {
        for (int j = 0; j < A.size2(); j++) {
            At(j, i) = A(i, j);
        }
    }
    return At;
}

int main(int argc, char* argv[]) {
    double rmax = 10.0;
    double dr = 0.3;
    bool hydrogen_mode = false;
    bool plot_wave = false;

    // Parse command line arguments
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if (arg == "-rmax" && i + 1 < argc) {
            rmax = std::atof(argv[i + 1]);
            hydrogen_mode = true;
        }
        if (arg == "-dr" && i + 1 < argc) {
            dr = std::atof(argv[i + 1]);
            hydrogen_mode = true;
        }
        if (arg == "-plotwave") {
            plot_wave = true;
            hydrogen_mode = true;
        }
    }

    if (hydrogen_mode) {
        int npoints = (int)(rmax / dr) - 1;
        pp::vector r(npoints);
        for (int i = 0; i < npoints; i++) {
            r[i] = dr * (i + 1);
        }

        pp::matrix H(npoints, npoints);
        double kin_factor = -0.5 / (dr * dr);

        // Build Kinetic Matrix K
        for (int i = 0; i < npoints - 1; i++) {
            H(i, i) = -2 * kin_factor;
            H(i, i + 1) = 1 * kin_factor;
            H(i + 1, i) = 1 * kin_factor;
        }
        H(npoints - 1, npoints - 1) = -2 * kin_factor;

        // Add Potential Matrix W (-1/r)
        for (int i = 0; i < npoints; i++) {
            H(i, i) += -1.0 / r[i];
        }

        // Solve EVD
        pp::EVD solver(H);

        if (plot_wave) {
            // Sort eigenvalues to find the lowest states (ground state, 1st excited, etc.)
            std::vector<int> idx(npoints);
            for (int i = 0; i < npoints; i++) idx[i] = i;
            std::sort(idx.begin(), idx.end(), [&solver](int i1, int i2) {
                return solver.w[i1] < solver.w[i2];
            });

            // Normalization constant: Const = 1 / sqrt(dr)
            double norm = 1.0 / std::sqrt(dr);

            // Print table: r | f0(r) | f1(r) | f2(r)
            for (int i = 0; i < npoints; i++) {
                std::cout << r[i] << " " 
                          << solver.V(i, idx[0]) * norm << " "
                          << solver.V(i, idx[1]) * norm << " "
                          << solver.V(i, idx[2]) * norm << "\n";
            }
            return 0;
        }

        // Standard convergence output (if -plotwave is NOT passed)
        double E0 = solver.w[0]; 
        for(int i = 1; i < npoints; i++){
            if(solver.w[i] < E0) E0 = solver.w[i];
        }
        std::cout << rmax << " " << dr << " " << E0 << "\n";
        return 0;
    }

    // ==========================================
    // TASK 1: RANDOM SYMMETRIC MATRIX CHECKS
    // ==========================================
    std::cout << "Testing EVD on Random Symmetric Matrix...\n";
    int n = 4;
    pp::matrix A(n, n);
    for (int i = 0; i < n; i++) {
        for (int j = i; j < n; j++) {
            double val = std::rand() % 10;
            A(i, j) = val;
            A(j, i) = val; 
        }
    }
    
    A.print("Original Symmetric Matrix A:");
    pp::EVD solver(A);
    
    pp::matrix D(n, n);
    for(int i = 0; i < n; i++) D(i,i) = solver.w[i];
    
    pp::matrix Vt = transpose(solver.V);
    pp::matrix VD = solver.V * D;
    pp::matrix VDVt = VD * Vt;
    
    VDVt.print("Check V * D * V^T (Should match A):");
    
    pp::matrix VtV = Vt * solver.V;
    VtV.print("Check V^T * V (Should be Identity):");

    return 0;
}