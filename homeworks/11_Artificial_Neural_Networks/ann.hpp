#pragma once
#include "matrix.hpp"
#include "minimize.hpp"
#include <functional>
#include <cmath>
#include <vector>

namespace pp {

struct ann {
    int n;          // Number of hidden neurons [cite: 43]
    pp::vector p;   // Flat parameter vector: size 3*n. [cite: 45]
                    // Layout: p[3*i] = a_i, p[3*i+1] = b_i, p[3*i+2] = w_i 

    ann(int num_neurons) : n(num_neurons), p(3 * num_neurons) {
        // Initialize weights and positions using randomized spreads to prevent stalling
        for (int i = 0; i < n; i++) {
            p[3 * i]     = -0.8 + 1.6 * ((double)i / (n > 1 ? n - 1 : 1)); // Distribute centers (a_i)
            p[3 * i + 1] = 0.2 + 0.3 * (i % 2);                          // Alternating scale widths (b_i)
            p[3 * i + 2] = (i % 2 == 0 ? 0.5 : -0.5);                    // Alternating sign weights (w_i)
        }
    }

    // ==========================================================
    // TASK A: Network Response Function Evaluation [cite: 47]
    // ==========================================================
    double response(double x) const {
        double sum = 0.0;
        for (int i = 0; i < n; i++) {
            double a = p[3 * i];
            double b = p[3 * i + 1];
            double w = p[3 * i + 2];
            double t = (x - a) / b;
            sum += (t * std::exp(-t * t)) * w; // Gaussian Wavelet: f(t) = t*e^(-t^2) [cite: 22, 27]
        }
        return sum; // [cite: 25]
    }

    // ==========================================================
    // TASK B: Exact Analytical Derivatives & Anti-Derivative 
    // ==========================================================
    double derivative(double x) const {
        double sum = 0.0;
        for (int i = 0; i < n; i++) {
            double a = p[3 * i];
            double b = p[3 * i + 1];
            double w = p[3 * i + 2];
            double t = (x - a) / b;
            // d/dx [ f((x-a)/b) ] = (1/b) * (1 - 2*t^2) * e^(-t^2)
            sum += (1.0 / b) * (1.0 - 2.0 * t * t) * std::exp(-t * t) * w;
        }
        return sum;
    }

    double second_derivative(double x) const {
        double sum = 0.0;
        for (int i = 0; i < n; i++) {
            double a = p[3 * i];
            double b = p[3 * i + 1];
            double w = p[3 * i + 2];
            double t = (x - a) / b;
            // d^2/dx^2 [ f((x-a)/b) ] = (1/b^2) * (4*t^3 - 6*t) * e^(-t^2)
            sum += (1.0 / (b * b)) * (4.0 * std::pow(t, 3.0) - 6.0 * t) * std::exp(-t * t) * w;
        }
        return sum;
    }

    double anti_derivative(double x) const {
        double sum = 0.0;
        for (int i = 0; i < n; i++) {
            double a = p[3 * i];
            double b = p[3 * i + 1];
            double w = p[3 * i + 2];
            double t = (x - a) / b;
            // Integral [ t * e^(-t^2) dt ] = -0.5 * e^(-t^2)
            // Multiplying by dx scale step factor 'b':
            sum += (-0.5 * b) * std::exp(-t * t) * w;
        }
        return sum;
    }

    // ==========================================================
    // TASK A Training Loop via Least-Squares Minimization [cite: 49, 51]
    // ==========================================================
    void train(const std::vector<double>& x_data, const std::vector<double>& y_data) {
        pp::cost_func network_cost = [&](const pp::vector& trial_p) {
            // Temporarily load minimization trial parameters into our network architecture
            pp::vector saved_p = this->p;
            this->p = trial_p;

            double sum_sq_residual = 0.0;
            for (size_t k = 0; k < x_data.size(); k++) {
                double diff = this->response(x_data[k]) - y_data[k];
                sum_sq_residual += diff * diff; // [cite: 36]
            }

            this->p = saved_p; // Revert
            return sum_sq_residual; // [cite: 37]
        };

        // Update max_iter from 500 to 1500 inside ann.hpp
        auto [optimized_p, steps] = pp::minimize(network_cost, this->p, 1e-5, 1500);
        this->p = optimized_p;
    }
};

} // namespace pp