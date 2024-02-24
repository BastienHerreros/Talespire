#include "libs/core/log.hpp"

#include <iostream>
#include <vector>

namespace libs::core {

const std::vector<std::string> levelStr{"INFO", "DEBUG", "TRACE", "WARNING", "ERROR"};

void print(LogLevel level, const std::string& message)
{
    std::cout << levelStr[static_cast<size_t>(level)] << ": " << message << std::endl;
}

}
