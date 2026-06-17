#pragma once
#include "matrix.hpp"

namespace pp {
    
struct qr {
    matrix Q;
    matrix R;

    // 1. Constructor performs MGS QR decomposition
    qr(const matrix& A) {
        int n = A.size1();
        int m = A.size2();
        Q = A;
        R = matrix(m, m);
        
        for (int i = 0; i < m; i++) {
            double len = Q[i].norm();
            R(i, i) = len;
            Q[i] /= len;
            
            for (int j = i + 1; j < m; j++) {
                R(i, j) = Q[i].dot(Q[j]);
                Q[j] -= Q[i] * R(i, j);
            }
        }
    }

    // 2. Linear solver using back-substitution (QRx = b -> Rx = Q^T*b)
    vector solve(const vector& b) const {
        int m = R.size2();
        vector x(m);
        vector y(m);

        // Step 1: Calculate y = Q^T * b
        for (int i = 0; i < m; i++) {
            y[i] = Q[i].dot(b);
        }

        // Step 2: Back-substitution to solve Rx = y
        for (int i = m - 1; i >= 0; i--) {
            double sum = 0;
            for (int j = i + 1; j < m; j++) {
                sum += R(i, j) * x[j];
            }
            x[i] = (y[i] - sum) / R(i, i);
        }

        return x;
    }

    // 3. Calculate the determinant of R (which equals det(A))
    double det() const {
        int m = R.size2();
        double d = 1.0;
        for (int i = 0; i < m; i++) {
            d *= R(i, i);
        }
        return d;
    }

    // 4. Calculate the matrix inverse
    matrix inverse() const {
        int n = Q.size1();
        int m = R.size2();
        assert(n == m); // Inverse only exists for square matrices!
        
        matrix Inv(n, n);
        
        for (int j = 0; j < n; j++) {
            vector e(n);
            e[j] = 1.0; 
            
            vector x = solve(e); 
            Inv[j] = x;          
        }
        
        return Inv;
    }
};

} // namespace pp