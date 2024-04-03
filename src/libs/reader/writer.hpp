#pragma once

#include <libs/core/Layout.hpp>

#include <string>

namespace libs::reader {

/**
 * @brief Get the uncompressed slab code
 * @param [in] layouts The list of layouts to save
 * @return The uncompressed slab code
 */
std::string getUncompressedCode(const std::vector<libs::core::Layout>& layouts);

/**
 * @brief Save a list of layouts to a compressed slab code
 * @param [in] layouts The list of layouts to save
 * @return The compressed slab code
 */
std::string saveLayouts(const std::vector<libs::core::Layout>& layouts);

}