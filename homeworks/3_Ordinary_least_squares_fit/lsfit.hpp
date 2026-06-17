#pragma once
#include "matrix.hpp"
#include "qr.hpp"
#include <vector>
#include <functional>
#include <tuple>
#include <cmath>

namespace pp {

inline std::tuple<pp::vector, pp::matrix> lsfit(
    const std::vector<std::function<double(double)>>& fs, 
    const pp::vector& x, 
    const pp::vector& y, 
    const pp::vector& dy) 
{
    int n = x.size();
    int m = fs.size();
    pp::matrix A(n, m);
    pp::vector b(n);

    // 1. Build the weighted design matrix A and target vector b
    for(int i = 0; i < n; i++) {
        b[i] = y[i] / dy[i];
        for(int k = 0; k < m; k++) {
            A(i, k) = fs[k](x[i]) / dy[i];
        }
    }

    // 2. Solve Ac = b using our existing QR decomposition
    pp::qr solver(A);
    pp::vector c = solver.solve(b);

    // 3. Calculate Covariance Matrix Sigma = (A^T A)^-1 = R^-1 * (R^-1)^T
    pp::matrix Rinv(m, m);
    
    // Invert the upper triangular matrix R
    for(int j = 0; j < m; j++) {
        Rinv(j, j) = 1.0 / solver.R(j, j);
        for(int i = j - 1; i >= 0; i--) {
            double sum = 0;
            for(int k = i + 1; k <= j; k++) {
                sum += solver.R(i, k) * Rinv(k, j);
            }
            Rinv(i, j) = -sum / solver.R(i, i);
        }
    }

    // Multiply Rinv * Rinv^T to get Sigma
    pp::matrix Sigma(m, m);
    for(int i = 0; i < m; i++) {
        for(int j = 0; j < m; j++) {
            double sum = 0;
            for(int k = 0; k < m; k++) {
                sum += Rinv(i, k) * Rinv(j, k);
            }
            Sigma(i, j) = sum;
        }
    }

    return std::make_tuple(c, Sigma);
}

} // namespace pp