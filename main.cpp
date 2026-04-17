#include <iostream>
#include "search.h"

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        std::cout << "Usage: psearch <pattern> <path>\n";
        return 1;
    } // if

    // std::string pattern = argv[1];
    // std::string path = argv[2];

    search_directory(argv[1], argv[2]);

    return 0;
} // main