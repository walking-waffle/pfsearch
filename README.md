# pfsearch
pfsearch is a lightweight multithreaded file regex search tool written in C++17.
It recursively scans a directory and searches for a target string across multiple files in parallel, improving performance over single-threaded approaches.

The project demonstrates practical usage of:
- C++ multithreading (std::thread)
- std::mutex and std::condition_variable
- filesystem traversal using std::filesystem
- bnary file ignore

The program creates a small thread pool, recursively enumerates files in a directory, and distributes file search tasks among worker threads. Each worker reads files line-by-line and prints matches in a grep-like format.

# Build

### Requires
C++17 or later
g++ mingw

### Linux
make

### Windows (MinGW)
mingw32-make.exe

# Usage
pfsearch <target> <path>


### Example
pfsearch "int" .
pfsearch "std::vector" src
pfsearch "void .*\\(" include

# Structure
pfsearch/
 ├── main.cpp
 ├── search.cpp
 ├── search.h
 ├── thread_pool.h
 ├── thread_pool.cpp
 ├── test // put some test file to search
 └── README.md