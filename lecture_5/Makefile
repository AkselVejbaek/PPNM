CXX = g++
CXXFLAGS = -std=c++20 -Wall -isysroot $(shell xcrun --show-sdk-path)

all: main
	@echo "--- Kører programmet med kommandolinje-argumenter (1.0, 2.0) ---"
	./main 1.0 2.0
	@echo "\n--- Kører programmet med pipes (stdin) ---"
	echo "0.5 1.5" | ./main

main: main.o
	$(CXX) $(CXXFLAGS) main.o -o main

main.o: main.cc
	$(CXX) $(CXXFLAGS) -c main.cc

clean:
	rm -f main main.o output.txt