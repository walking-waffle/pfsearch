#include "search.h"
#include <iostream>
#include <fstream>
#include <filesystem>

namespace fs = std::filesystem;

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

void search_directory(const std::string &path)
{
    if (!fs::exists(path))
    {
        std::cerr << "Path does not exist: "
                  << path << "\n";
        return;
    }

    if (!fs::is_directory(path))
    {
        std::cerr << "Not a directory: "
                  << path << "\n";
        return;
    }

    for (auto &p :
         fs::recursive_directory_iterator(
             path,
             fs::directory_options::skip_permission_denied))
    {
        if (fs::is_regular_file(p))
        {
            {
                std::lock_guard<std::mutex> lock(mu);
                task_queue.push(p.path().string());
            }

            cv.notify_one();
        }
    }
}