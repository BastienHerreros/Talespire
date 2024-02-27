#pragma once

#include "libs/core/Asset.hpp"

#include <boost/uuid/uuid.hpp>

#include <cstdint>
#include <string>
#include <vector>

namespace libs::core {

/**
 * @struct Layout
 * @brief Define a layout from Talespire
 * A layout stores all the instances of an asset
 */
struct Layout
{
    /**
     * @brief Construct a new Layout object
     * @param [in] assetKindId The uuid of the asset
     * @param [in] assetsCount The number of instance of this assets
     * @param [in] reserved Other private information
     */
    Layout(const boost::uuids::uuid& assetKindId, uint16_t assetsCount, uint16_t reserved);

    /**
     * @brief Construct a new Layout object
     */
    Layout();

    /// The uuid of the asset
    const boost::uuids::uuid m_assetKindId;

    /// The number of instance of this assets
    const uint16_t m_assetsCount;

    /// Other private information
    const uint16_t m_reserved;

    /// List of the instances
    std::vector<Asset> m_assets;
};

}