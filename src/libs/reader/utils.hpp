#pragma once

#include "libs/core/log.hpp"

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

    const auto numByteRead = data.gcount();
    libs::core::print("readBits => " + std::to_string(numByteRead));

    if(numByteRead != sizeof(T))
    {
        throw std::runtime_error("Stream only read " + std::to_string(numByteRead) + " bytes");
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
    if(sizeof(T) < static_cast<size_t>(size))
    {
        throw std::runtime_error("Retrun type is too small");
    }

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