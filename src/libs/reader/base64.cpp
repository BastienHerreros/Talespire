#include "libs/reader/base64.hpp"

#include <boost/algorithm/string.hpp>
#include <boost/archive/iterators/base64_from_binary.hpp>
#include <boost/archive/iterators/binary_from_base64.hpp>
#include <boost/archive/iterators/transform_width.hpp>

namespace libs::reader {
namespace base64 {

std::string decode(const std::string& strToDecode)
{
    return {
      boost::archive::iterators::
        transform_width<boost::archive::iterators::binary_from_base64<std::string::const_iterator>, 8, 6>{
          std::begin(strToDecode)},
      {std::end(strToDecode)},
    };
}

std::string encode(const std::string& strToEncode)
{
    std::string r{
      boost::archive::iterators::base64_from_binary<
        boost::archive::iterators::transform_width<std::string::const_iterator, 6, 8>>{std::begin(strToEncode)},
      {std::end(strToEncode)},
    };
    return r.append((3 - strToEncode.size() % 3) % 3, '=');
}

}
}