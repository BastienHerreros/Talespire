#pragma once

#include <boost/uuid/uuid.hpp>

#include <string>

namespace libs::core {

boost::uuids::uuid convertStringToUuid(const std::string& id);

boost::uuids::uuid convertBinToUuid(const std::string& bin);

}