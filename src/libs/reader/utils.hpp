#pragma once

#include <sstream>
#include <string>

namespace libs::reader {

/**
 * @brief Read bytes from the stream
 * @tparam T The type of the returned value
 * @param [in] data The stream to read from
 * @param [in] val The returned value
 * @note The function will read sizeof(T) bytes
 */
template<typename T>
void readBits(std::stringstream& data, T& val)
{
    data.read(reinterpret_cast<char*>(&val), sizeof(T));

    if(data.fail())
    {
        throw std::runtime_error("Cannot read bytes");
    }
}

/**
 * @brief Read bytes from the stream
 * @tparam T The type of the returned value
 * @param [in] data The stream to read from
 * @param [in] val The returned value
 * @param [in] size The number of bytes to read
 */
template<typename T>
void readBits(std::stringstream& data, T& val, std::streamsize size)
{
    data.read(reinterpret_cast<char*>(&val), size);

    if(data.fail())
    {
        throw std::runtime_error("Cannot read bytes");
    }
}

/**
 * @brief Get a mask for the given number of bits
 * @param [in] size The size of the mask
 * @return The mask
 * @note ex size 2 => 0b0011
 */
int getMask(size_t size) { return (1 << size) - 1; }

}