CXX = g++
CXXFLAGS = -O3 -Wall -std=c++11

all: lsfit_solver plot.png

lsfit_solver: main.cpp matrix.hpp qr.hpp lsfit.hpp
	$(CXX) $(CXXFLAGS) -o lsfit_solver main.cpp

plot.png: lsfit_solver
	./lsfit_solver
	echo 'set terminal pngcairo size 800,600; \
	      set output "plot.png"; \
	      set title "Radium-224 (ThX) Radioactive Decay Fit"; \
	      set xlabel "Time (days)"; \
	      set ylabel "ln(Activity)"; \
	      set grid; \
	      plot "data.txt" using 1:2:3 with yerrorbars pt 7 lc rgb "black" title "Experimental Data", \
	           "fit.txt" using 1:2 with lines lw 2 lc rgb "blue" title "Best Fit", \
	           "fit.txt" using 1:3 with lines dt 2 lc rgb "gray" title "c \pm \delta c limits", \
	           "fit.txt" using 1:4 with lines dt 2 lc rgb "gray" notitle, \
	           "fit.txt" using 1:5 with lines dt 2 lc rgb "gray" notitle, \
	           "fit.txt" using 1:6 with lines dt 2 lc rgb "gray" notitle' | gnuplot

clean:
	rm -f lsfit_solver data.txt fit.txt plot.png