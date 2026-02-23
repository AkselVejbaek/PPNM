CXX = c++
CXXFLAGS = -std=c++20

Out.txt: main
	./main > Out.txt

sfuns.o: sfuns.h sfuns.cc
	$(CXX) $(CXXFLAGS) -c sfuns.cc -o sfuns.o

main.o: sfuns.h main_gamma.cc
	$(CXX) $(CXXFLAGS) -c main_gamma.cc -o main.o

main: main.o sfuns.o
	$(CXX) main.o sfuns.o -o main

clean:
	rm -f Out.txt main main.o sfuns.o
