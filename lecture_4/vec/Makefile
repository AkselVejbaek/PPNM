# Compiler og flag
CXX = g++
CXXFLAGS = -std=c++20 -Wall -isysroot $(shell xcrun --show-sdk-path)

# Mål (den færdige fil)
TARGET = main

# Objektfiler (ting der skal bygges før main kan samles)
OBJS = main.o vec.o

# Standard regel
all: $(TARGET)
	./$(TARGET)

# Linker objektfilerne sammen til det færdige program
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(TARGET)

# Kompilerer main.cc til main.o
main.o: main.cc vec.h
	$(CXX) $(CXXFLAGS) -c main.cc

# Kompilerer vec.cc til vec.o
vec.o: vec.cc vec.h
	$(CXX) $(CXXFLAGS) -c vec.cc

# Rengøring
clean:
	rm -f $(TARGET) $(OBJS) Out.txt