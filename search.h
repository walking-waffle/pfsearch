#pragma once
#include <string>
#include <queue>
#include <mutex>
#include <condition_variable>

extern std::queue<std::string> task_queue;
extern std::mutex mu;
extern std::condition_variable cv;
extern bool done;

void search_file(const std::string &path, const std::string &target);
void search_directory(const std::string &path);