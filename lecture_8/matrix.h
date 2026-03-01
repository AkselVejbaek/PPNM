#ifndef MATRIX_H
#define MATRIX_H

#include <vector>
#include <iostream>
#include <cassert>
#include <cmath>
#include "vec.h"

template <typename T>
class matrix {
public: // Gjort public for nem adgang i øvelsen
    int rows, cols;
    std::vector<T> data;

    matrix(int r, int c) : rows(r), cols(c), data(r * c, 0) {}

    T& operator()(int r, int c) { return data[r * cols + c]; }
    const T& operator()(int r, int c) const { return data[r * cols + c]; }

    // Hent en kolonne som en vec (kun for 3x3 i denne øvelse)
    vec<T> get_col(int c) const {
        return vec<T>((*this)(0,c), (*this)(1,c), (*this)(2,c));
    }

    // Sæt en kolonne fra en vec
    void set_col(int c, const vec<T>& v) {
        (*this)(0,c) = v.x; (*this)(1,c) = v.y; (*this)(2,c) = v.z;
    }
};

// QR-dekomponering (Gram-Schmidt) for 3x3
template <typename T>
void qr_decomp(const matrix<T>& A, matrix<T>& Q, matrix<T>& R) {
    // Vi implementerer den klassiske Gram-Schmidt
    for (int i = 0; i < 3; i++) {
        vec<T> ai = A.get_col(i);
        for (int j = 0; j < i; j++) {
            vec<T> qj = Q.get_col(j);
            R(j, i) = ai.dot(qj);
            ai = ai + (-R(j, i)) * qj;
        }
        R(i, i) = ai.norm();
        Q.set_col(i, (1.0 / R(i, i)) * ai);
    }
}

template <typename T>
std::ostream& operator<<(std::ostream& os, const matrix<T>& A) {
    for (int i = 0; i < A.rows; ++i) {
        os << "[ ";
        for (int j = 0; j < A.cols; ++j) os << A(i, j) << " ";
        os << "]\n";
    }
    return os;
}
#endif