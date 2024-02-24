#pragma once

#include <iostream>
#include <string>
#include <vector>

namespace libs::core {

/**
 * @enum LogLevel
 * @brief Define the level of the logs
 */
enum class LogLevel
{
    Info,
    Debug,
    Trace,
    Warning,
    Error
};

/**
 * @brief Print a message
 * @param [in] message The message to print
 * @param [in] level The level of le log
 */
template<typename T>
void print(const T& message, LogLevel level = LogLevel::Debug)
{
    const std::vector<std::string> levelStr{"INFO", "DEBUG", "TRACE", "WARNING", "ERROR"};
    std::cout << levelStr[static_cast<size_t>(level)] << ": \"" << message << "\"" << std::endl;
}

} // namespace libs::core