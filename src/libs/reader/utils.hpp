#pragma once

#include <sstream>
#include <string>

namespace libs::reader {

template<typename T>
void readBits(std::stringstream& data, T& val)
{
    data.read(reinterpret_cast<char*>(&val), sizeof(T));

    if(data.fail())
    {
        throw std::runtime_error("Cannot read");
    }
}

template<typename T>
void readBits(std::stringstream& data, T& val, std::streamsize size)
{
    data.read(reinterpret_cast<char*>(&val), size);
}

int getMask(size_t size) { return (1 << size) - 1; }

}