#include <iostream>
#include <vector>
#include <mutex>
#include <queue>
#include <thread>
#include <condition_variable>
#include "search.h"

std::queue<std::string> task_queue;
std::mutex mu;
std::condition_variable cv;
bool done = false;

// 處理一個檔案中的目標
void worker(std::string target)
{
    while (true)
    {
        std::string file;

        {
            std::unique_lock<std::mutex> lock(mu);

            cv.wait(lock, []
                    { return !task_queue.empty() || done; });

            if (task_queue.empty())
                return;

            file = task_queue.front();
            task_queue.pop();
        }

        search_file(file, target);
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

    // 掃描目錄 (producer)
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