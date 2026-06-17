CXX = g++
CXXFLAGS = -O3 -Wall -std=c++14

all: spline_solver plot_lin.png plot_quad.png

spline_solver: main.cpp spline.hpp
	$(CXX) $(CXXFLAGS) -o spline_solver main.cpp

# Run program to generate data files
data: spline_solver
	./spline_solver

plot_lin.png: data
	echo 'set terminal pngcairo size 800,600; \
	      set output "plot_lin.png"; \
	      set title "Linear Spline: Interpolation & Integration of cos(x)"; \
	      set xlabel "x"; \
	      set ylabel "y"; \
	      set grid; \
	      plot "lin_data.txt" using 1:2 with points pt 7 lc rgb "black" title "Data points {xi, cos(xi)}", \
	           "lin_fit.txt" using 1:2 with lines lw 2 lc rgb "blue" title "Linear Interpolant S(x)", \
	           "lin_fit.txt" using 1:3 with lines lw 2 lc rgb "red" title "Integral of S(x)"' | gnuplot

plot_quad.png: data
	echo 'set terminal pngcairo size 800,600; \
	      set output "plot_quad.png"; \
	      set title "Quadratic Spline: Interpolation, Derivative & Integral of sin(x)"; \
	      set xlabel "x"; \
	      set ylabel "y"; \
	      set grid; \
	      plot "quad_data.txt" using 1:2 with points pt 7 lc rgb "black" title "Data points {xi, sin(xi)}", \
	           "quad_fit.txt" using 1:2 with lines lw 2 lc rgb "blue" title "Quadratic Interpolant S(x)", \
	           "quad_fit.txt" using 1:3 with lines lw 2 lc rgb "green" title "Derivative dS/dx", \
	           "quad_fit.txt" using 1:4 with lines lw 2 lc rgb "red" title "Integral of S(x)"' | gnuplot

clean:
	rm -f spline_solver *.txt *.png