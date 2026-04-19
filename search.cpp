#include "search.h"
#include <iostream>
#include <fstream>
#include <filesystem>

void search_file(const std::string &file,
                 const std::string &target)
{
    std::ifstream f_in(file);
    if (!f_in)
        return;

    std::string line;
    int line_num = 0;

    while (std::getline(f_in, line))
    {
        line_num++;

        if (line.find(target) != std::string::npos)
            std::cout << file << ":" << line_num
                      << ": " << line << "\n";
    }
}