#pragma once

#include <string>

namespace libs::reader {
namespace gzip {

/**
 * @brief Compress a string using the gzip format
 * @param [in] data The string to compress
 * @return The string in the gzip format
 */
std::string compress(const std::string& data);

/**
 * @brief Decompress a string from the gzip format
 * @param [in] data The string to decompress
 * @return The decompressed string
 */
std::string decompress(const std::string& data);

}
}
