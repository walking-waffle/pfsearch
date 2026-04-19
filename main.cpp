#include <iostream>
#include <vector>
#include <mutex>
#include <queue>
#include <thread>
#include <filesystem>
#include <atomic>
#include <condition_variable>

#include "search.h"

namespace fs = std::filesystem;

std::queue<std::string> task_queue;
std::mutex mu;
std::condition_variable cv;
std::atomic<bool> done(false);

// 處理一個檔案中的目標
void worker(const std::string &target)
{
    while (true)
    {
        std::unique_lock<std::mutex> lock(mu);

        cv.wait(lock, []
                { return !task_queue.empty() || done; });

        if (done && task_queue.empty())
            return;

        std::string file = task_queue.front();
        task_queue.pop();
        lock.unlock();
        search_file(file, target);
    }
}

void search_directory(const std::string &path)
{
    if (!fs::exists(path))
    {
        std::cerr << "Path does not exist: " << path << "\n";
        return;
    } // if

    if (!fs::is_directory(path))
    {
        std::cerr << "Not a directory: " << path << "\n";
        return;
    } // if

    for (const auto &p : fs::recursive_directory_iterator(path))
    {
        if (fs::is_regular_file(p.path()))
        {
            {
                std::lock_guard<std::mutex> lock(mu);
                task_queue.push(p.path().string());
            }

            cv.notify_one();
        } // if
    }
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        std::cout << "Usage: pfsearch <target> <path>\n";
        return 1;
    }

    std::string target = argv[1];
    std::string path = argv[2];

    // 建立 thread pool
    std::vector<std::thread> threads;
    for (int i = 0; i < 4; i++)
        threads.emplace_back(worker, target);

    // 掃描目錄
    search_directory(path);

    // 通知 worker 結束
    {
        std::lock_guard<std::mutex> lock(mu);
        done = true;
    }
    cv.notify_all();

    // 等待 threads
    for (auto &t : threads)
        t.join();
    return 0;
}