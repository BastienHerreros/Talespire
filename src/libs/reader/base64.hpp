#pragma once

#include <string>

namespace libs::reader {
namespace base64 {

/**
 * @brief Encode a string in base 64
 * @param [in] strToEncode The string to encode
 * @return The base64 string
 */
std::string encode(const std::string& strToEncode);

/**
 * @brief Decode a string from base 64
 * @param [in] strToDecode The string to decode
 * @return The decoded string
 */
std::string decode(const std::string& strToDecode);

}
}