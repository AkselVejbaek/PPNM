CXX = g++
CXXFLAGS = -O3 -Wall -std=c++17

all: clean ann_solver plot_interp.png plot_ode.png

ann_solver: main.cpp ann.hpp minimize.hpp matrix.hpp qr.hpp
	$(CXX) $(CXXFLAGS) -o ann_solver main.cpp

data: ann_solver
	./ann_solver

plot_interp.png: data
	@echo 'set terminal pngcairo size 800,600' > plot.gp
	@echo 'set output "plot_interp.png"' >> plot.gp
	@echo 'set title "Neural Network Function Approximation and Analytical Calculus"' >> plot.gp
	@echo 'set xlabel "x"' >> plot.gp
	@echo 'set grid' >> plot.gp
	@echo 'plot "ann_interpolation.txt" using 1:2 with lines lw 3 lc "0x000000" title "Target g(x)", \' >> plot.gp
	@echo '     "ann_interpolation.txt" using 1:3 with lines dt 2 lw 2 lc "0x008080" title "ANN Approximation F_p(x)", \' >> plot.gp
	@echo '     "ann_interpolation.txt" using 1:4 with lines lw 1.5 lc "0x800080" title "Exact Analytical ANN Derivative F_{p}\x27(x)", \' >> plot.gp
	@echo '     "ann_interpolation.txt" using 1:5 with lines lw 1.5 lc "0xDAA520" title "Exact Analytical ANN Integral \\int F_p(x)dx"' >> plot.gp
	gnuplot plot.gp
	rm -f plot.gp

plot_ode.png: data
	@echo 'set terminal pngcairo size 800,600' > plot.gp
	@echo 'set output "plot_ode.png"' >> plot.gp
	@echo 'set title "Physics-Informed Neural Network ODE Solver (y\x27\x27 + y = 0)"' >> plot.gp
	@echo 'set xlabel "x"' >> plot.gp
	@echo 'set ylabel "y(x)"' >> plot.gp
	@echo 'set grid' >> plot.gp
	@echo 'plot "ann_ode_solution.txt" using 1:2 with lines lw 3 lc "0x000000" title "Exact Solution: sin(x)", \' >> plot.gp
	@echo '     "ann_ode_solution.txt" using 1:3 with lines dt 2 lw 2 lc "0xCD5C5C" title "Neural Network Solved Profile"' >> plot.gp
	gnuplot plot.gp
	rm -f plot.gp

clean:
	rm -f ann_solver *.txt *.png plot.gp