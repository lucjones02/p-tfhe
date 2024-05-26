#ifndef UTILS_HPP
#define UTILS_HPP

#include <iostream>

constexpr int TESTS = 10'000;

void erase_terminal();
void update_progress(const std::string& func_name, int i);

#endif // !UTILS_HPP
