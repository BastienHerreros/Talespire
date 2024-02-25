#pragma once

#include <libs/core/Layout.hpp>

#include <string>

namespace libs::reader {

std::string cleanSlabCode(const std::string& slabCode);

std::string readSlabCode(const std::string& slabCode);

std::vector<libs::core::Layout> getLayouts(const std::string& slabCode);

}