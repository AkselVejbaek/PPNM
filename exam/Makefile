CXX = g++
CXXFLAGS = -O3 -Wall -std=c++17

all: clean bbpso_solver plot_convergence.png

bbpso_solver: main.cpp bbpso.hpp matrix.hpp
	$(CXX) $(CXXFLAGS) -o bbpso_solver main.cpp

data: bbpso_solver
	./bbpso_solver

plot_convergence.png: data
	@echo 'set terminal pngcairo size 800,600' > plot.gp
	@echo 'set output "plot_convergence.png"' >> plot.gp
	@echo 'set title "BBPSO Convergence Log Profile on Rosenbrock Landscape"' >> plot.gp
	@echo 'set xlabel "Swarm Iterations (Steps)"' >> plot.gp
	@echo 'set ylabel "Best Objective Metric Value Log"' >> plot.gp
	@echo 'set logscale y' >> plot.gp
	@echo 'set grid' >> plot.gp
	@echo 'plot "swarm_convergence.txt" using 1:2 with linespoints lw 2.5 lc "0x800080" title "Swarm Top Metric Tracker"' >> plot.gp
	gnuplot plot.gp
	rm -f plot.gp

clean:
	rm -f bbpso_solver *.txt *.png plot.gp