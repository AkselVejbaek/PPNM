CXX = g++
CXXFLAGS = -std=c++20 -isysroot $(shell xcrun --show-sdk-path)

all: lecture4
	./lecture4

lecture4: lecture4.cc
	$(CXX) $(CXXFLAGS) lecture4.cc -o lecture4

clean:
	rm -f lecture4