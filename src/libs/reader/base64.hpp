#pragma once

#include <string>

namespace libs::reader {
namespace base64 {

std::string encode(const std::string& strToEncode);

std::string decode(const std::string& strToDecode);

}
}