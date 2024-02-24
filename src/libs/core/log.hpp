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
 * @param [in] level The level of le log
 * @param [in] message The message to print
 */
void print(LogLevel level, const std::string& message);

} // namespace libs::core