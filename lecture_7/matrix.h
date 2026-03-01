#ifndef MATRIX_H
#define MATRIX_H

#include <vector>
#include <iostream>
#include <cassert>
#include "vec.h" // Vi genbruger din vec klasse til matrix-vektor multiplikation

template <typename T>
class matrix {
private:
    int rows, cols;
    std::vector<T> data;

public:
    matrix(int r, int c) : rows(r), cols(c), data(r * c, 0) {}

    // Tilgå elementer (row-major order)
    T& operator()(int r, int c) {
        assert(r < rows && c < cols);
        return data[r * cols + c];
    }

    const T& operator()(int r, int c) const {
        assert(r < rows && c < cols);
        return data[r * cols + c];
    }

    int size1() const { return rows; }
    int size2() const { return cols; }

    // Matrix-Vektor multiplikation (A * v = b)
    // Antager v er en 3D vec fra tidligere lektion
    vec<T> operator*(const vec<T>& v) const {
        assert(cols == 3); 
        vec<T> result;
        result.x = (*this)(0,0)*v.x + (*this)(0,1)*v.y + (*this)(0,2)*v.z;
        result.y = (*this)(1,0)*v.x + (*this)(1,1)*v.y + (*this)(1,2)*v.z;
        result.z = (*this)(2,0)*v.x + (*this)(2,1)*v.y + (*this)(2,2)*v.z;
        return result;
    }
};

template <typename T>
std::ostream& operator<<(std::ostream& os, const matrix<T>& A) {
    for (int i = 0; i < A.size1(); ++i) {
        os << "[ ";
        for (int j = 0; j < A.size2(); ++j) {
            os << A(i, j) << " ";
        }
        os << "]\n";
    }
    return os;
}

#endif