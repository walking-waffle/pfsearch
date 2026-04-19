CXX = g++
CXXFLAGS = -std=c++17 -pthread

all:
	$(CXX) main.cpp search.cpp -o pfsearch $(CXXFLAGS)