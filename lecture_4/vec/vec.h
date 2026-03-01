#ifndef VEC_H
#define VEC_H

#include <iostream>
#include <string>

class vec {
public:
    double x, y, z;

    // Konstruktører (Rule of Five)
    vec() : x(0), y(0), z(0) {}                         // Default
    vec(double x, double y, double z) : x(x), y(y), z(z) {} // Parametriseret
    vec(const vec& other) = default;                    // Copy constructor
    vec(vec&& other) = default;                         // Move constructor
    ~vec() = default;                                   // Destructor

    // Assignments
    vec& operator=(const vec& other) = default;         // Copy assignment
    vec& operator=(vec&& other) = default;              // Move assignment

    // Operator overloading (medlemsfunktioner)
    vec& operator+=(const vec& other);
    vec& operator-=(const vec& other);
    vec& operator*=(double scalar);

    // Metoder
    void print(std::string s = "") const;
    double norm() const;
    double dot(const vec& other) const;
    vec cross(const vec& other) const;
};

// Ikke-medlems operatorer (for den "moderne" form)
vec operator+(vec a, const vec& b);
vec operator-(vec a, const vec& b);
vec operator*(vec a, double s);
vec operator*(double s, vec a);
std::ostream& operator<<(std::ostream& os, const vec& v);

bool approx(const vec& a, const vec& b, double acc = 1e-9, double eps = 1e-9);

#endif