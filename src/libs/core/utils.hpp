#pragma once

#include <boost/uuid/uuid.hpp>

#include <string>
#include <vector>

namespace libs::core {

/**
 * @brief Convert a string into an uuid
 * @param [in] id The string that contains the uuid (ex: 857fea21-3c24-47e1-8901-5df910301dac)
 * @return The converted uuid
 */
boost::uuids::uuid convertStringToUuid(const std::string& id);

/**
 * @brief Convert a byte array into an uuid
 * @param [in] bin The byte array
 * @return The converted uuid
 */
boost::uuids::uuid convertBinToUuid(const char bin[16]);

/**
 * @brief Convert an uuid to a byte array
 * @param [in] uuid The uuid
 * @return The byte array
 */
std::vector<char> convertUuidToBin(const boost::uuids::uuid& uuid);

}