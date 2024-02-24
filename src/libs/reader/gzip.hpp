#pragma once

#include <string>

namespace libs::reader {
namespace gzip {

std::string compress(const std::string& data);

std::string decompress(const std::string& data);

}
}
