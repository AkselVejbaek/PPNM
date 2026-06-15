# Compiler settings
CXX = g++
CXXFLAGS = -O3 -Wall -std=c++11

# The default target runs everything
all: out.times.data plot.png

# Rule to build the C++ program
qr_solver: main.cpp matrix.hpp qr.hpp
	$(CXX) $(CXXFLAGS) -o qr_solver main.cpp

# Rule to run the timing loop (Ultra-safe Mac version)
out.times.data: qr_solver
	> $@
	for N in $$(seq 100 40 400); do \
		/usr/bin/time -p ./qr_solver -size $$N 2> time_tmp.txt ; \
		T=$$(awk '/real/ {print $$2}' time_tmp.txt) ; \
		echo "$$N $$T" >> $@ ; \
	done
	rm -f time_tmp.txt

# Rule to plot the data using gnuplot
plot.png: out.times.data
	echo 'set terminal png; \
	      set output "plot.png"; \
	      set title "QR Decomposition Time vs Matrix Size"; \
	      set xlabel "Matrix Size N"; \
	      set ylabel "Time (seconds)"; \
	      f(x) = A*x**3; \
	      fit f(x) "out.times.data" via A; \
	      plot "out.times.data" with points title "Measured time", \
	           f(x) with lines title "Fit O(N^3)"' | gnuplot

# Clean up generated files
clean:
	rm -f qr_solver out.times.data plot.png fit.log time_tmp.txt