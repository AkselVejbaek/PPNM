#include "vec.h"
#include <cmath>
#include <algorithm>

// Hjælpefunktion til approx (som vi lavede sidst)
static bool approx_double(double a, double b, double acc, double eps) {
    double diff = std::abs(a - b);
    if (diff <= acc) return true;
    return diff <= eps * std::max(std::abs(a), std::abs(b));
}

vec& vec::operator+=(const vec& other) {
    x += other.x; y += other.y; z += other.z;
    return *this;
}

// Den moderne operator+
// Her tager vi 'a' som "copy by value". Det gør, at compileren kan 
// optimere 'move' operationer automatisk.
vec operator+(vec a, const vec& b) {
    a += b;
    return a;
}

double vec::norm() const { return std::sqrt(x*x + y*y + z*z); }

double vec::dot(const vec& other) const { return x*other.x + y*other.y + z*other.z; }

vec vec::cross(const vec& other) const {
    return vec(y*other.z - z*other.y, z*other.x - x*other.z, x*other.y - y*other.x);
}

void vec::print(std::string s) const {
    std::cout << s << "{" << x << ", " << y << ", " << z << "}" << std::endl;
}

std::ostream& operator<<(std::ostream& os, const vec& v) {
    return os << "{" << v.x << ", " << v.y << ", " << v.z << "}";
}

bool approx(const vec& a, const vec& b, double acc, double eps) {
    return approx_double(a.x, b.x, acc, eps) && 
           approx_double(a.y, b.y, acc, eps) && 
           approx_double(a.z, b.z, acc, eps);
}