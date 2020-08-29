#pragma once

#include <iostream>

// Logger macros
#define LOG_INFO(msg) (std::cout << msg << '\n')
#define LOG_ERROR(msg) (std::cerr << msg << '\n')
