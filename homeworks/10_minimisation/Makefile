CXX = g++
CXXFLAGS = -O3 -Wall -std=c++17

all: clean plot_higgs.png

plot_higgs.png: mc_solver
	./mc_solver < higgs.data.txt
	@echo 'set terminal pngcairo size 800,600' > plot.gp
	@echo 'set output "plot_higgs.png"' >> plot.gp
	@echo 'set title "Higgs Boson Discovery: Breit-Wigner Fit over Background-Subtracted Signal"' >> plot.gp
	@echo 'set xlabel "Energy E (GeV)"' >> plot.gp
	@echo 'set ylabel "Cross Section Signal \\sigma(E)"' >> plot.gp
	@echo 'set grid' >> plot.gp
	@echo 'plot "higgs.data.txt" using 1:2:3 with yerrorbars pt 7 ps 1.0 lc "0x008080" title "CERN Experimental Data", \' >> plot.gp
	@echo '     "higgs_fit_curve.txt" using 1:2 with lines lw 2.5 lc "0x800080" title "Breit-Wigner Fit Resonance Profile"' >> plot.gp
	gnuplot plot.gp
	rm -f plot.gp

mc_solver: main.cpp minimize.hpp matrix.hpp qr.hpp
	$(CXX) $(CXXFLAGS) -o mc_solver main.cpp

clean:
	rm -f mc_solver higgs_fit_curve.txt plot.gp