#include "libs/reader/gzip.hpp"

#include <libs/core/log.hpp>

#include <boost/iostreams/copy.hpp>
#include <boost/iostreams/filter/gzip.hpp>
#include <boost/iostreams/filtering_streambuf.hpp>

#include <iostream>
#include <sstream>

namespace libs::reader {
namespace gzip {

std::string compress(const std::string& data)
{
    std::istringstream origin(data);

    boost::iostreams::filtering_istreambuf in;
    in.push(boost::iostreams::gzip_compressor(boost::iostreams::gzip_params(boost::iostreams::gzip::best_compression)));
    in.push(origin);

    std::ostringstream compressed;
    boost::iostreams::copy(in, compressed);
    return compressed.str();
}

std::string decompress(const std::string& data)
{
    std::istringstream compressed(data);

    boost::iostreams::filtering_istreambuf in;
    in.push(boost::iostreams::gzip_decompressor());
    in.push(compressed);

    std::ostringstream origin;

    try
    {
        boost::iostreams::copy(in, origin);
    }
    catch(const std::exception& e)
    {
        libs::core::print(e.what(), libs::core::LogLevel::Error);
    }

    return origin.str();
}

}
}
