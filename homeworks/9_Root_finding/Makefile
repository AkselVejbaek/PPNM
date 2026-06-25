CXX = g++
CXXFLAGS = -O3 -Wall -std=c++17

all: root_solver plot_hydrogen.png

root_solver: main.cpp roots.hpp matrix.hpp ode.hpp qr.hpp
	$(CXX) $(CXXFLAGS) -o root_solver main.cpp

data: root_solver
	./root_solver

plot_hydrogen.png: data
	@echo 'set terminal pngcairo size 800,600' > plot.gp
	@echo 'set output "plot_hydrogen.png"' >> plot.gp
	@echo 'set title "Hydrogen S-Wave Radial Wavefunction via Shooting Method"' >> plot.gp
	@echo 'set xlabel "Radial Distance r (Bohr radii)"' >> plot.gp
	@echo 'set ylabel "Reduced Wavefunction f(r)"' >> plot.gp
	@echo 'set grid' >> plot.gp
	@echo 'plot "wavefunction.txt" using 1:2 with lines lw 3 lc "0x008080" title "Numerical Shooting Method (E0 ≈ -0.50)", \' >> plot.gp
	@echo '     "wavefunction.txt" using 1:3 with lines dt 2 lw 2 lc "0x000000" title "Exact Analytical f_0(r) = r*e^{-r}"' >> plot.gp
	gnuplot plot.gp
	rm -f plot.gp

clean:
	rm -f root_solver *.txt *.png plot.gp