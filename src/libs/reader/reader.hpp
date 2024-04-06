#pragma once

#include <libs/core/Layout.hpp>

#include <string>

namespace libs::reader {

/**
 * @brief Remove the trailing and leading spaces and the "```"
 * @param [in] slabCode The code to clean
 * @return The code without any leanding/trailing spaces and "```"
 */
std::string cleanSlabCode(const std::string& slabCode);

/**
 * @brief Read the slabcode from the compressed and encoded string
 * @param [in] slabCode The compressed (gzip) and encoded (base64) string
 * @return The readable slab code
 */
std::string readSlabCode(const std::string& slabCode);

/**
 * @brief parse the layouts from the slab code
 * @param [in] slabCode The compressed (gzip) and encoded (base64) string
 * @return List of layouts from the code
 */
std::vector<libs::core::Layout> getLayouts(const std::string& slabCode);

}