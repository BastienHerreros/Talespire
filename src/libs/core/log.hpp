#pragma once

#include <string>

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
 * @brief Print a string
 * @param [in] message The message to print
 * @param [in] level The level of le log
 */
void print(const std::string& message, LogLevel level = LogLevel::Debug);
void print(size_t message, LogLevel level = LogLevel::Debug);

} // namespace libs::core