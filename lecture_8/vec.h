#ifndef VEC_H
#define VEC_H

#include <iostream>
#include <cmath>
#include <string>
#include <algorithm>
#include <complex>

template <typename T>
class vec {
public:
    T x, y, z;

    // Konstruktører
    vec() : x(0), y(0), z(0) {}
    vec(T x, T y, T z) : x(x), y(y), z(z) {}

    // Operatorer (+=, -= osv. skal defineres inde i klassen eller som templates)
    vec& operator+=(const vec& other) {
        x += other.x; y += other.y; z += other.z;
        return *this;
    }

    vec& operator*=(T scalar) {
        x *= scalar; y *= scalar; z *= scalar;
        return *this;
    }

    // Metoder
    // Ved std::complex returnerer norm() typisk en double/float, ikke complex.
    auto norm() const {
        return std::sqrt(std::norm(x) + std::norm(y) + std::norm(z));
    }

    T dot(const vec& other) const {
        return x * other.x + y * other.y + z * other.z;
    }

    void print(std::string s = "") const {
        std::cout << s << "{" << x << ", " << y << ", " << z << "}" << std::endl;
    }
};

// --- Templated Eksterne Operatorer ---

template <typename T>
vec<T> operator+(vec<T> a, const vec<T>& b) {
    a += b;
    return a;
}

template <typename T>
vec<T> operator*(vec<T> a, T s) {
    a *= s;
    return a;
}

template <typename T>
vec<T> operator*(T s, vec<T> a) {
    a *= s;
    return a;
}

template <typename T>
std::ostream& operator<<(std::ostream& os, const vec<T>& v) {
    return os << "{" << v.x << ", " << v.y << ", " << v.z << "}";
}

// Robust sammenligning (approx) til templates
template <typename T>
bool approx(T a, T b, double acc = 1e-9, double eps = 1e-9) {
    double diff = std::abs(a - b);
    if (diff <= acc) return true;
    return diff <= eps * std::max(std::abs(a), std::abs(b));
}

template <typename T>
bool approx(const vec<T>& v, const vec<T>& u, double acc = 1e-9, double eps = 1e-9) {
    return approx(v.x, u.x, acc, eps) && approx(v.y, u.y, acc, eps) && approx(v.z, u.z, acc, eps);
}

#endif