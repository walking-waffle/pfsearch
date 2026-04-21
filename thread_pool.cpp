#include "thread_pool.h"

ThreadPool::ThreadPool(size_t num_threads) : done(false)
{
    for (size_t i = 0; i < num_threads; i++)
        threads.emplace_back(&ThreadPool::worker, this);
}

ThreadPool::~ThreadPool()
{
    stop();
}

void ThreadPool::enqueue(std::function<void()> task)
{
    {
        std::lock_guard<std::mutex> lock(mu);
        tasks.push(task);
    }
    cv.notify_one();
}

void ThreadPool::stop()
{
    {
        std::lock_guard<std::mutex> lock(mu);
        done = true;
    }

    cv.notify_all();

    for (auto &t : threads)
        t.join();
}

// 處理一個檔案中的目標，輸入我要找的 target
void ThreadPool::worker()
{
    while (true)
    {
        std::unique_lock<std::mutex> lock(mu);

        // thread pool 有東西 or 全部結束才會醒來，否則 wait
        // 單純的 cv.wait(lock)，如果 thread 醒來，執行到task_queue.pop();就會crash
        cv.wait(lock, [this]
                { return !tasks.empty() || done; });

        if (done && tasks.empty())
            return;

        auto task = tasks.front();
        tasks.pop();

        lock.unlock();
        task();
    }
}