CXX = g++
CXXFLAGS = -std=c++17 -pthread -Wall

ifeq ($(OS),Windows_NT)
    EXE = .exe
    RM = del /Q
else
    EXE =
    RM = rm -f
endif

TARGET = pfsearch$(EXE)

OBJS = main.o search.o thread_pool.o

$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $(TARGET) $(CXXFLAGS)

main.o: main.cpp search.h thread_pool.h
	$(CXX) $(CXXFLAGS) -c main.cpp

search.o: search.cpp search.h
	$(CXX) $(CXXFLAGS) -c search.cpp

thread_pool.o: thread_pool.cpp thread_pool.h
	$(CXX) $(CXXFLAGS) -c thread_pool.cpp

clean:
	$(RM) $(OBJS) $(TARGET)

.PHONY: clean