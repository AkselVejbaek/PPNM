CXX = g++
CXXFLAGS = -O3 -Wall -std=c++17

all: krylov_solver
	./krylov_solver

krylov_solver: main.cpp matrix.hpp qr.hpp krylov.hpp
	$(CXX) $(CXXFLAGS) -o krylov_solver main.cpp

clean:
	rm -f krylov_solver