#pragma once
#include <iostream>
#include <vector>
#include <cmath>
#include <cassert>
#include <iomanip>

namespace pp {

struct vector {
    std::vector<double> data;
    vector() = default;
    vector(int n) : data(n, 0.0) {}
    
    inline int size() const { return data.size(); }
    inline double& operator[](int i) { return data[i]; }
    inline const double& operator[](int i) const { return data[i]; }

    vector& operator/=(double c) {
        for(int i = 0; i < size(); i++) data[i] /= c;
        return *this;
    }
    vector& operator-=(const vector& other) {
        for(int i = 0; i < size(); i++) data[i] -= other.data[i];
        return *this;
    }
    double norm() const {
        double sum = 0;
        for(int i = 0; i < size(); i++) sum += data[i] * data[i];
        return std::sqrt(sum);
    }
    double dot(const vector& other) const {
        double res = 0;
        for(int i = 0; i < size(); i++) res += data[i] * other.data[i];
        return res;
    }
    void print(std::string s = "") const {
        std::cout << s;
        for(double x : data) std::cout << std::setw(8) << std::setprecision(4) << x << " ";
        std::cout << "\n";
    }
};

inline vector operator*(vector a, const double c) {
    for(int i = 0; i < a.size(); i++) a[i] *= c;
    return a;
}

struct matrix {
    std::vector<pp::vector> cols;
    matrix() = default;
    matrix(int n, int m) : cols(m, pp::vector(n)) {}
    
    inline int size1() const { return cols.empty() ? 0 : cols[0].size(); }
    inline int size2() const { return cols.size(); }
    inline double& operator()(int i, int j) { return cols[j][i]; }
    inline const double& operator()(int i, int j) const { return cols[j][i]; }
    inline vector& operator[](int i) { return cols[i]; }
    inline const vector& operator[](int i) const { return cols[i]; }

    void print(std::string s = "") const {
        std::cout << s << "\n";
        for(int i = 0; i < size1(); i++) {
            for(int j = 0; j < size2(); j++) {
                std::cout << std::setw(8) << std::setprecision(4) << cols[j][i] << " ";
            }
            std::cout << "\n";
        }
    }
};

inline matrix operator*(const matrix& A, const matrix& B) {
    assert(A.size2() == B.size1());
    matrix R(A.size1(), B.size2());
    for(int j = 0; j < B.size2(); j++) {
        for(int k = 0; k < A.size2(); k++) {
            for(int i = 0; i < A.size1(); i++) {
                R(i, j) += A(i, k) * B(k, j);
            }
        }
    }
    return R;
}

} // namespace pp