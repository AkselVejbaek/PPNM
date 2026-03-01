CXX = g++
CXXFLAGS = -std=c++20 -Wall -isysroot $(shell xcrun --show-sdk-path)

all: main
	./main

main: main.cc vec.h
	$(CXX) $(CXXFLAGS) main.cc -o main

clean:
	rm -f main