#include "libs/reader/reader.hpp"

#include "libs/reader/base64.hpp"

#include <libs/core/log.hpp>

#include <boost/iostreams/filter/gzip.hpp>
#include <boost/iostreams/filtering_stream.hpp>

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
    typedef std::vector<char> buffer_t;

    const auto cleanedCode = cleanSlabCode(slabCode);

    libs::core::print("Cleaned code is " + cleanedCode);

    const auto decodedCode = base64::decode(cleanedCode);

    libs::core::print("Decoded code is " + decodedCode);

    buffer_t compressed;
    std::copy(decodedCode.begin(), decodedCode.end(), std::back_inserter(compressed));

    buffer_t decompressed;
    boost::iostreams::filtering_ostream os;
    os.push(boost::iostreams::gzip_decompressor());
    os.push(boost::iostreams::back_inserter(decompressed));

    boost::iostreams::write(os, &compressed[0], static_cast<std::streamsize>(compressed.size()));

    std::string decompressedCode(decompressed.begin(), decompressed.end());

    libs::core::print("Decompressed code is " + decompressedCode);

    return decompressedCode;
}

}