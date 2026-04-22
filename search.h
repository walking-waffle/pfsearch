#pragma once
#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <mutex>
#include <thread>
#include <regex>

#define COLOR_RED "\033[31m"
#define COLOR_RESET "\033[0m"

void search_file(const std::string &path, const std::regex &target);