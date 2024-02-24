#pragma once

#include "libs/core/Asset.hpp"

#include <cstdint>
#include <string>
#include <vector>

namespace libs::core {

struct Layout
{
    Layout(const std::string& assetKindId, uint16_t assetsCount, uint16_t reserved);

    const std::string m_assetKindId;
    const uint16_t m_assetsCount;
    const uint16_t m_reserved;

    std::vector<Asset> m_assets;
};

}