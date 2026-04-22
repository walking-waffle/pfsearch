#include <iostream>
#include <filesystem>
#include <thread>
#include <regex>

#include "search.h"
#include "thread_pool.h"

namespace fs = std::filesystem;

void search_directory(const std::string &path,
                      const std::regex &target,
                      ThreadPool &pool)
{
    if (!fs::exists(path) || !fs::is_directory(path))
    {
        std::cerr << "Invalid directory: " << path << "\n";
        return;
    } // if

    // 遞迴尋找目錄與檔案
    for (const auto &p : fs::recursive_directory_iterator(path))
    {
        // 只留下檔案
        if (fs::is_regular_file(p))
        {
            std::string file = p.path().string();
            // 把 search_file 作為 task 放入 thread_pool 中
            pool.enqueue([file, target]()
                         { search_file(file, target); });
        } // if
    } // for
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        std::cout << "Usage: pfsearch <target> <path>\n";
        return 1;
    }

    std::regex target(argv[1]);
    std::string path = argv[2];

    // 取得硬體建議 thread 數量，拿不到就用 4 個 threads
    unsigned int num_threads =
        std::thread::hardware_concurrency() ? std::thread::hardware_concurrency() : 4;

    ThreadPool pool(num_threads);
    search_directory(path, target, pool);
} // main