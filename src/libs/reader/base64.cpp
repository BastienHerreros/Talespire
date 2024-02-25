#include "libs/reader/base64.hpp"

#include <boost/algorithm/string.hpp>
#include <boost/beast/core/detail/base64.hpp>

namespace libs::reader {
namespace base64 {

std::string decode(const std::string& strToDecode)
{
    std::string res;
    res.resize(boost::beast::detail::base64::decoded_size(strToDecode.size()));
    const auto [outSize, inReadSize] =
      boost::beast::detail::base64::decode(res.data(), strToDecode.data(), strToDecode.size());
    res.resize(outSize);
    return res;
}

std::string encode(const std::string& strToEncode)
{
    std::string res;
    res.resize(boost::beast::detail::base64::encoded_size(strToEncode.size()));
    boost::beast::detail::base64::encode(res.data(), strToEncode.data(), strToEncode.size());

    return res;
}

}
}