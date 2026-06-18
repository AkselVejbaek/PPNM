CXX = g++
CXXFLAGS = -O3 -Wall -std=c++17

all: mc_solver plot_errors.png

mc_solver: main.cpp mc.hpp matrix.hpp
	$(CXX) $(CXXFLAGS) -o mc_solver main.cpp

data: mc_solver
	./mc_solver

plot_errors.png: data
	@echo 'set terminal pngcairo size 800,600' > plot.gp
	@echo 'set output "plot_errors.png"' >> plot.gp
	@echo 'set title "Monte Carlo Error Scaling Behavior"' >> plot.gp
	@echo 'set xlabel "Number of Samples (N)"' >> plot.gp
	@echo 'set ylabel "Absolute Integration Error"' >> plot.gp
	@echo 'set logscale xy' >> plot.gp
	@echo 'set grid' >> plot.gp
	@echo 'plot "mc_error_scaling.txt" using 1:2 with lines lw 2 lc "0x800080" title "Plain MC Predicted Error", \' >> plot.gp
	@echo '     "mc_error_scaling.txt" using 1:3 with points pt 7 ps 0.8 lc "0x008080" title "Plain MC Actual Error", \' >> plot.gp
	@echo '     "mc_error_scaling.txt" using 1:4 with linespoints pt 5 ps 0.8 lw 1.5 lc "0xDAA520" title "Quasi-MC Actual Error (Halton)"' >> plot.gp
	gnuplot plot.gp
	rm -f plot.gp

clean:
	rm -f mc_solver *.txt *.png plot.gp