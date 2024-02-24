#include "libs/reader/base64.hpp"

#include <boost/algorithm/string.hpp>
#include <boost/archive/iterators/base64_from_binary.hpp>
#include <boost/archive/iterators/binary_from_base64.hpp>
#include <boost/archive/iterators/transform_width.hpp>

namespace libs::reader {
namespace base64 {

std::string decode(const std::string& strToDecode)
{
    using namespace boost::archive::iterators;
    using It = transform_width<binary_from_base64<std::string::const_iterator>, 8, 6>;
    return boost::algorithm::trim_right_copy_if(std::string(It(std::begin(strToDecode)), It(std::end(strToDecode))),
                                                [](char c) { return c == '\0'; });
}

std::string encode(const std::string& strToEncode)
{
    using namespace boost::archive::iterators;
    using It = base64_from_binary<transform_width<std::string::const_iterator, 6, 8>>;
    auto tmp = std::string(It(std::begin(strToEncode)), It(std::end(strToEncode)));
    return tmp.append((3 - strToEncode.size() % 3) % 3, '=');
}

}
}