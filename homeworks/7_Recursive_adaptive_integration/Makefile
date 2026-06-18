CXX = g++
CXXFLAGS = -O3 -Wall -std=c++17

all: run_tests plot_erf.png plot_accuracy.png

run_tests: main.cpp adaptive_int.hpp matrix.hpp
	$(CXX) $(CXXFLAGS) -o run_tests main.cpp

data: run_tests
	./run_tests

plot_erf.png: data
	@echo 'set terminal pngcairo size 800,600' > plot.gp
	@echo 'set output "plot_erf.png"' >> plot.gp
	@echo 'set title "Numerical Evaluation of the Error Function erf(z)"' >> plot.gp
	@echo 'set xlabel "z"' >> plot.gp
	@echo 'set ylabel "erf(z)"' >> plot.gp
	@echo 'set grid' >> plot.gp
	@echo 'plot "erf_curve.txt" with lines lw 2.5 lc "0x800080" title "Our erf(z) implementation"' >> plot.gp
	gnuplot plot.gp
	rm -f plot.gp

plot_accuracy.png: data
	@echo 'set terminal pngcairo size 800,600' > plot.gp
	@echo 'set output "plot_accuracy.png"' >> plot.gp
	@echo 'set title "Actual Absolute Error vs. Requested Accuracy Goal (acc)"' >> plot.gp
	@echo 'set xlabel "Requested Accuracy (acc)"' >> plot.gp
	@echo 'set ylabel "Absolute Deviation |erf(1)_{calc} - erf(1)_{true}|"' >> plot.gp
	@echo 'set logscale xy' >> plot.gp
	@echo 'set grid' >> plot.gp
	@echo 'plot "accuracy_decay.txt" using 1:2 with linespoints pt 7 ps 1.5 lw 2 lc "0x008080" title "Error Decay Curve", \' >> plot.gp
	@echo '     x with lines dt 2 lc "0x000000" title "Ideal linear target line (y=x)"' >> plot.gp
	gnuplot plot.gp
	rm -f plot.gp

clean:
	rm -f run_tests *.txt *.png plot.gp