#pragma once
#include "matrix.hpp"
#include <vector>
#include <cmath>
#include <functional>
#include <algorithm>
#include <numeric>
#include <random>

namespace pp {

// ==========================================================
// TASK A: Plain Monte Carlo Integration with Linear Congruential Generator
// ==========================================================
struct lcg_rand {
    unsigned long long state;
    lcg_rand(unsigned long long seed = 42) : state(seed) {}
    
    // Quick-and-dirty LCG parameters
    double next() {
        state = state * 6364136223846793005ULL + 1442695040888963407ULL;
        return (double)state / 18446744073709551615.0;
    }
};

inline std::pair<double, double> plain_mc(
    const std::function<double(const pp::vector&)>& f,
    const pp::vector& a, const pp::vector& b, int N
) {
    int dim = a.size();
    double volume = 1.0;
    for (int i = 0; i < dim; i++) volume *= (b[i] - a[i]);

    lcg_rand rand_gen(1337);
    double sum = 0.0;
    double sum_sq = 0.0;
    pp::vector x(dim);

    for (int i = 0; i < N; i++) {
        for (int d = 0; d < dim; d++) {
            x[d] = a[d] + (b[d] - a[d]) * rand_gen.next();
        }
        double fx = f(x);
        sum += fx;
        sum_sq += fx * fx;
    }

    double mean = sum / N;
    double sigma_sq = (sum_sq / N) - (mean * mean);
    if (sigma_sq < 0.0) sigma_sq = 0.0; // Float precision safety guard

    double integral = mean * volume;
    double error = volume * std::sqrt(sigma_sq / N);

    return {integral, error};
}

// ==========================================================
// TASK B: Quasi-Monte Carlo Integration using Halton Sequences
// ==========================================================
inline double van_der_corput(int n, int base) {
    double q = 0.0;
    double bk = 1.0 / base;
    while (n > 0) {
        q += (n % base) * bk;
        n /= base;
        bk /= base;
    }
    return q;
}

inline void halton_sample(int index, int dim, pp::vector& x) {
    // Array of prime numbers for prime bases
    static const int primes[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37};
    for (int d = 0; d < dim; d++) {
        x[d] = van_der_corput(index, primes[d]);
    }
}

inline std::pair<double, double> quasi_mc(
    const std::function<double(const pp::vector&)>& f,
    const pp::vector& a, const pp::vector& b, int N
) {
    int dim = a.size();
    double volume = 1.0;
    for (int i = 0; i < dim; i++) volume *= (b[i] - a[i]);

    double sum1 = 0.0, sum2 = 0.0;
    pp::vector x(dim);

    // Estimate error using two disjoint subsequences (odd vs even indices)
    for (int i = 1; i <= N; i++) {
        halton_sample(i, dim, x);
        for (int d = 0; d < dim; d++) x[d] = a[d] + (b[d] - a[d]) * x[d];
        sum1 += f(x);

        halton_sample(i + N, dim, x);
        for (int d = 0; d < dim; d++) x[d] = a[d] + (b[d] - a[d]) * x[d];
        sum2 += f(x);
    }

    double int1 = (sum1 / N) * volume;
    double int2 = (sum2 / N) * volume;

    return {int1, std::abs(int1 - int2)};
}

// ==========================================================
// TASK C: Recursive Stratified Sampling
// ==========================================================
inline std::pair<double, double> stratified_mc(
    const std::function<double(const pp::vector&)>& f,
    pp::vector a, pp::vector b, int N, int nmin = 500
) {
    int dim = a.size();
    
    // Base Case: If we don't have enough points left, perform plain MC on this subvolume
    if (N < nmin) {
        return plain_mc(f, a, b, N);
    }

    // Phase 1: Sample baseline to compute localized subvariances per dimension
    double volume = 1.0;
    for (int i = 0; i < dim; i++) volume *= (b[i] - a[i]);

    std::vector<double> sum_left(dim, 0.0), sum_sq_left(dim, 0.0);
    std::vector<double> sum_right(dim, 0.0), sum_sq_right(dim, 0.0);
    std::vector<int> count_left(dim, 0), count_right(dim, 0);

    lcg_rand rand_gen(N); // Vary seed by depth N
    pp::vector x(dim);

    for (int i = 0; i < nmin; i++) {
        for (int d = 0; d < dim; d++) x[d] = a[d] + (b[d] - a[d]) * rand_gen.next();
        double fx = f(x);

        for (int d = 0; d < dim; d++) {
            double mid = (a[d] + b[d]) / 2.0;
            if (x[d] < mid) {
                sum_left[d] += fx; sum_sq_left[d] += fx * fx; count_left[d]++;
            } else {
                sum_right[d] += fx; sum_sq_right[d] += fx * fx; count_right[d]++;
            }
        }
    }

    // Phase 2: Find the dimension with the largest variance split discrepancy
    int max_dim = 0;
    double max_var_diff = -1.0;

    for (int d = 0; d < dim; d++) {
        double var_l = (count_left[d] > 1) ? (sum_sq_left[d]/count_left[d] - std::pow(sum_left[d]/count_left[d], 2)) : 0.0;
        double var_r = (count_right[d] > 1) ? (sum_sq_right[d]/count_right[d] - std::pow(sum_right[d]/count_right[d], 2)) : 0.0;
        double total_var_est = std::abs(var_l - var_r);
        if (total_var_est > max_var_diff) {
            max_var_diff = total_var_est;
            max_dim = d;
        }
    }

    // Phase 3: Bifurcate volume along max_dim and apportion remaining sample points proportionally
    double mid_split = (a[max_dim] + b[max_dim]) / 2.0;
    
    double var_left_split = (count_left[max_dim] > 1) ? (sum_sq_left[max_dim]/count_left[max_dim] - std::pow(sum_left[max_dim]/count_left[max_dim],2)) : 0.0;
    double var_right_split = (count_right[max_dim] > 1) ? (sum_sq_right[max_dim]/count_right[max_dim] - std::pow(sum_right[max_dim]/count_right[max_dim],2)) : 0.0;
    
    double weight_l = std::sqrt(std::max(var_left_split, 1e-6));
    double weight_r = std::sqrt(std::max(var_right_split, 1e-6));
    double fraction_l = weight_l / (weight_l + weight_r);

    int points_left = N - nmin;
    int N_left = static_cast<int>(points_left * fraction_l);
    int N_right = points_left - N_left;

    // Recurse left half
    pp::vector b_left = b; b_left[max_dim] = mid_split;
    auto [I_left, E_left] = stratified_mc(f, a, b_left, N_left, nmin);

    // Recurse right half
    pp::vector a_right = a; a_right[max_dim] = mid_split;
    auto [I_right, E_right] = stratified_mc(f, a_right, b, N_right, nmin);

    // Combined grand answers
    return {I_left + I_right, std::sqrt(E_left * E_left + E_right * E_right)};
}

} // namespace pp