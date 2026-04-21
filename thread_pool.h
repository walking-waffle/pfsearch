#pragma once
#include <vector>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <atomic>

class ThreadPool
{
public:
    ThreadPool(size_t num_threads);
    ~ThreadPool();

    void enqueue(std::function<void()> task);
    void stop();

private:
    void worker();

    std::vector<std::thread> threads;
    std::queue<std::function<void()>> tasks;

    std::mutex mu;
    // blocking until condition is true
    std::condition_variable cv;
    std::atomic<bool> done;
};