#pragma once

#include "libs/core/Asset.hpp"

#include <boost/uuid/uuid.hpp>

#include <cstdint>
#include <string>
#include <vector>

namespace libs::core {

struct Layout
{
    Layout(const boost::uuids::uuid& assetKindId, uint16_t assetsCount, uint16_t reserved);

    const boost::uuids::uuid m_assetKindId;
    const uint16_t m_assetsCount;
    const uint16_t m_reserved;

    std::vector<Asset> m_assets;
};

}