========================================================================
EXAMINATION PROJECT: BARE-BONES PARTICLE SWARM OPTIMIZATION (BBPSO)
========================================================================
Author: [Aksel Vejbæk]
Course: Practical Programming
Date: June 29, 2026

------------------------------------------------------------------------
1. PROJECT DESCRIPTION
------------------------------------------------------------------------
This project implements the Bare-bones Particle Swarm Optimization (BBPSO) 
algorithm as a parameter-free, derivative-free global optimizer. 
By eliminating velocity vectors entirely, BBPSO updates particle positions 
by sampling coordinates directly from a Gaussian distribution based on personal 
best (pbest) and global best (gbest) positions:

    Mean (mu)     = 0.5 * (pbest + gbest)
    Width (sigma) = |pbest - gbest|

The algorithm features multi-dimensional boundary enforcement and a numerical 
safeguard against variance collapse. It was successfully benchmarks on two 
complex landscapes:
  - Rosenbrock Valley (testing narrow parabolic trench crawling)
  - Himmelblau Function (testing multi-modal global minima tracking)

------------------------------------------------------------------------
2. DIRECTORY STRUCTURE & COMPILATION
------------------------------------------------------------------------
Files:
  - bbpso.hpp  : Core stochastic swarm algorithm implementation.
  - main.cpp   : Rosenbrock/Himmelblau benchmarks and data exporting suite.
  - matrix.hpp : Custom library vector and matrix classes.
  - qr.hpp     : Householder QR decomposition code.
  - Makefile   : Automation script for building, executing, and plotting.

To clean, build, run the optimization benchmarks, and plot the results, use:
    make

This compiles 'bbpso_solver', runs the tests, and outputs 'plot_convergence.png'.

------------------------------------------------------------------------
3. SELF-EVALUATION
------------------------------------------------------------------------
Score: 9 / 10

Justification:
  - Complete, faithful C++ implementation of the stochastic sampling rules.
  - Achieved exceptional global convergence precision (Rosenbrock error ~1e-22, 
    Himmelblau error ~1e-27) without any gradient data.
  - Fully integrated with our existing custom matrix library and automated via Makefile.

------------------------------------------------------------------------
4. DOCUMENTATION OF AI USE
------------------------------------------------------------------------
In compliance with academic integrity guidelines, the use of Generative AI 
(specifically the Gemini large language model) is documented below:

  - Role: The AI acted as an interactive pair-programming assistant, code 
    reviewer, and debugging companion throughout the repository's 
    development.
  - Core Implementations: Initial foundational scaffolding for the matrix, 
    root-finding, neural network, and swarm optimization templates was 
    co-developed with AI assistance, then manually adapted, customized, 
    and optimized to work with our specific custom vector library mechanics.
  - Debugging & Refinement: AI was used to interpret complex compiler warnings 
    (such as unused variables and missing headers), identify numerical traps 
    like local minima stalling, and suggest mathematical corrections (such 
    as shifting from forward to central finite differences and integrating 
    dynamic Levenberg-Marquardt damping variables into the Newton minimizer).
  - Verification: All final code compilation, validation benchmarks on the 
    Rosenbrock/Himmelblau surfaces, data piping, and Gnuplot visualization 
    pipelines were executed, tested, and verified entirely on the local 
    host machine.
========================================================================