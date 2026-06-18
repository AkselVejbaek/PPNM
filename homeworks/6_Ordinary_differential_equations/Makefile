CXX = g++
CXXFLAGS = -O3 -Wall -std=c++17

all: ode_solver figure8.png

ode_solver: main.cpp ode.hpp matrix.hpp
	$(CXX) $(CXXFLAGS) -o ode_solver main.cpp

figure8.png: ode_solver
	./ode_solver
	@echo 'set terminal pngcairo size 800,600' > plot.gp
	@echo 'set output "figure8.png"' >> plot.gp
	@echo 'set title "Stable Figure-8 Orbit of Three Equal Masses"' >> plot.gp
	@echo 'set xlabel "x"' >> plot.gp
	@echo 'set ylabel "y"' >> plot.gp
	@echo 'set grid' >> plot.gp
	@echo 'plot "orbit_path.txt" using 2:3 with lines lw 6 lc "0x800080" title "Mass 1 Path", \' >> plot.gp
	@echo '     "orbit_path.txt" using 4:5 with lines lw 4 dt 2 lc "0x008080" title "Mass 2 Path", \' >> plot.gp
	@echo '     "orbit_path.txt" using 6:7 with lines lw 2 dt 3 lc "0xDAA520" title "Mass 3 Path"' >> plot.gp
	gnuplot plot.gp
	rm -f plot.gp

clean:
	rm -f ode_solver orbit_path.txt figure8.png plot.gp