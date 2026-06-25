#pragma once
#include "matrix.hpp"
#include <vector>
#include <cmath>
#include <functional>
#include <random>
#include <algorithm>

namespace pp {

using objective_func = std::function<double(const pp::vector&)>;

struct particle {
    pp::vector x;     // Current position
    pp::vector pbest; // Personal best position
    double fitness;   // Current fitness value
    double best_fit;  // Best fitness value achieved by this particle
};

inline pp::vector bbpso(
    const objective_func& f,
    const pp::vector& bounds_min,
    const pp::vector& bounds_max,
    int swarm_size = 40,
    int max_iter = 500
) {
    int dim = bounds_min.size();
    
    // Setup high-quality Mersenne Twister random engine
    std::mt19937 gen(1337);
    std::uniform_real_distribution<double> uniform_dist(0.0, 1.0);
    std::normal_distribution<double> normal_dist(0.0, 1.0);

    // 1. Initialize the Swarm
    std::vector<particle> swarm(swarm_size);
    pp::vector gbest(dim);
    double gbest_fit = std::numeric_limits<double>::infinity();

    for (int i = 0; i < swarm_size; i++) {
        swarm[i].x = pp::vector(dim);
        swarm[i].pbest = pp::vector(dim);
        
        for (int d = 0; d < dim; d++) {
            swarm[i].x[d] = bounds_min[d] + (bounds_max[d] - bounds_min[d]) * uniform_dist(gen);
            swarm[i].pbest[d] = swarm[i].x[d];
        }
        
        swarm[i].fitness = f(swarm[i].x);
        swarm[i].best_fit = swarm[i].fitness;

        if (swarm[i].best_fit < gbest_fit) {
            gbest_fit = swarm[i].best_fit;
            gbest = swarm[i].pbest;
        }
    }

    // 2. Core Optimization Swarm Loop
    for (int iter = 0; iter < max_iter; iter++) {
        for (int i = 0; i < swarm_size; i++) {
            for (int d = 0; d < dim; d++) {
                // BBPSO core rule: Sample from a Gaussian distribution based on pbest and gbest
                double mean = 0.5 * (swarm[i].pbest[d] + gbest[d]);
                double std_dev = std::abs(swarm[i].pbest[d] - gbest[d]);

                // Handle boundary collapse guard
                if (std_dev < 1e-12) std_dev = 1e-12;

                // Position update step
                swarm[i].x[d] = mean + std_dev * normal_dist(gen);

                // Enforce strict boundary confinement rules
                if (swarm[i].x[d] < bounds_min[d]) swarm[i].x[d] = bounds_min[d];
                if (swarm[i].x[d] > bounds_max[d]) swarm[i].x[d] = bounds_max[d];
            }

            // Evaluate fitness of the new position
            swarm[i].fitness = f(swarm[i].x);

            // Update personal best state
            if (swarm[i].fitness < swarm[i].best_fit) {
                swarm[i].best_fit = swarm[i].fitness;
                swarm[i].pbest = swarm[i].x;

                // Update global swarm best state
                if (swarm[i].best_fit < gbest_fit) {
                    gbest_fit = swarm[i].best_fit;
                    gbest = swarm[i].pbest;
                }
            }
        }
    }

    return gbest;
}

} // namespace pp