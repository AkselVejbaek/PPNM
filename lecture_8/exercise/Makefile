CXX = g++
CXXFLAGS = -std=c++20 -Wall -pthread -isysroot $(shell xcrun --show-sdk-path)
N = 500000000

all: out.times

main: main.cc
	$(CXX) $(CXXFLAGS) main.cc -o main

out.times: main
	@echo "Måler performance..."
	@rm -f out.times
	@for n in 1 2 3 4 5 6 7 8; do \
		printf "$$n " >> out.times; \
		/usr/bin/time -l ./main -threads $$n -terms $(N) 2>&1 | grep real | awk '{print $$1}' >> out.times; \
	done
	@cat out.times

clean:
	rm -f main out.times