CXX = g++
CXXFLAGS = -O3 -Wall -std=c++11

all: evd_solver dr_convergence.txt rmax_convergence.txt

evd_solver: main.cpp matrix.hpp evd.hpp
	$(CXX) $(CXXFLAGS) -o evd_solver main.cpp

# 1. Varying dr (fixed rmax=10) in PARALLEL - Safe Method
DRS = 0.1 0.2 0.3 0.4 0.5 0.6 0.7 0.8
dr_convergence.txt: evd_solver
	for DR in $(DRS); do \
		./evd_solver -rmax 10 -dr $$DR > log_dr_$$DR.txt & \
	done; \
	wait
	> $@
	for DR in $(DRS); do cat log_dr_$$DR.txt >> $@; done
	rm -f log_dr_*.txt

# 2. Varying rmax (fixed dr=0.1) in PARALLEL - Safe Method
RMAXS = 5 8 10 12 15 20
rmax_convergence.txt: evd_solver
	for RMAX in $(RMAXS); do \
		./evd_solver -rmax $$RMAX -dr 0.1 > log_rmax_$$RMAX.txt & \
	done; \
	wait
	> $@
	for RMAX in $(RMAXS); do cat log_rmax_$$RMAX.txt >> $@; done
	rm -f log_rmax_*.txt

clean:
	rm -f evd_solver dr_convergence.txt rmax_convergence.txt log_*.txt