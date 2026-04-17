#include "search.h"
#include <iostream>
#include <fstream>
#include <filesystem>

namespace fs = std::filesystem;

// file:檔案路徑   pattern:要找的字串
void search_file(const std::string &file, const std::string &pattern)
{
    std::ifstream f_in(file);
    if (!f_in)
        return;

    std::string line;
    int line_num = 0;

    while (std::getline(f_in, line))
    {
        line_num++;

        // grep style
        if (line.find(pattern) != std::string::npos)
            std::cout << file << ":" << line_num << ": " << line << "\n";
    } // while
}

void search_directory(const std::string &path, const std::string &pattern)
{
    if (!fs::exists(path))
    {
        std::cerr << "Path does not exist: " << path << "\n";
        return;
    }

    if (!fs::is_directory(path))
    {
        std::cerr << "Not a directory: " << path << "\n";
        return;
    }

    for (auto &p : fs::recursive_directory_iterator(path))
    {
        if (fs::is_regular_file(p))
            search_file(p.path().string(), pattern);
    }
}