#include "search.h"

std::mutex cout_mu;

// 檢查前面 512 bytes，如果有 NULL char 那就是 binary file
bool is_binary(const std::string &file)
{
    std::ifstream f(file, std::ios::binary);
    if (!f)
        return true;

    char buffer[512];
    f.read(buffer, sizeof(buffer));
    std::streamsize n = f.gcount();

    for (int i = 0; i < n; i++)
    {
        if (buffer[i] == '\0')
            return true;
    } // for

    return false;
}

void search_file(const std::string &file, const std::regex &target)
{
    if (is_binary(file))
        return;

    std::ifstream f_in(file);
    if (!f_in)
        return;

    std::string line;
    int line_num = 0;

    while (std::getline(f_in, line))
    {
        line_num++;

        if (std::regex_search(line, target))
        {
            std::lock_guard<std::mutex> lock(cout_mu);
            std::string colored =
                std::regex_replace(
                    line,
                    target,
                    COLOR_RED "$&" COLOR_RESET);
            std::cout
                << "[" << std::this_thread::get_id() << "] "
                << file << ":"
                << line_num << " | "
                << line << "\n";
        }
    }
}