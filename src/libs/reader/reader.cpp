#include "libs/reader/reader.hpp"

#include "libs/reader/base64.hpp"
#include "libs/reader/gzip.hpp"

#include <libs/core/log.hpp>

namespace libs::reader {

std::string cleanSlabCode(const std::string& slabCode)
{
    const auto firstChar = slabCode.find_first_not_of(" ");
    const auto lastChar = slabCode.find_last_not_of(" ");

    std::string cleanedCode = slabCode.substr(firstChar, lastChar - firstChar + 1);

    if(cleanedCode.find("`") != std::string::npos)
    {
        cleanedCode = cleanedCode.substr(3, cleanedCode.size() - 6);
    }

    return cleanedCode;
}

std::string readSlabCode(const std::string& slabCode)
{
    const auto cleanedCode = cleanSlabCode(slabCode);

    libs::core::print("Cleaned code is " + cleanedCode);

    const auto decodedCode = base64::decode(cleanedCode);

    libs::core::print("Decoded code is " + decodedCode);

    const auto decompressed = gzip::decompress(decodedCode);

    std::string decompressedCode(decompressed.cbegin(), decompressed.cend());

    libs::core::print("Decompressed code is " + decompressedCode);

    return decompressedCode;
}

}