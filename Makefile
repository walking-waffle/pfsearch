CXX = g++
CXXFLAGS = -std=c++17 -pthread

all:
	$(CXX) main.cpp search.cpp thread_pool.cpp -o pfsearch $(CXXFLAGS)