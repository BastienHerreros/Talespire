#include "libs/reader/writer.hpp"

#include "libs/reader/base64.hpp"
#include "libs/reader/gzip.hpp"
#include "libs/reader/utils.hpp"

#include <libs/core/log.hpp>
#include <libs/core/utils.hpp>

namespace libs::reader {

std::string getUncompressedCode(const std::vector<libs::core::Layout>& layouts)
{
    std::stringstream finalCode;

    libs::core::print("Writing header...");

    // Add magic number
    writeBits(finalCode, uint32_t{0xD1CEFACE}, sizeof(uint32_t));

    // Add version
    writeBits(finalCode, uint16_t{2}, sizeof(uint16_t));

    // Add layout count
    writeBits(finalCode, static_cast<uint16_t>(layouts.size()), sizeof(uint16_t));

    // Add creature count
    writeBits(finalCode, uint16_t{0}, sizeof(uint16_t));

    libs::core::print("Writing layouts...");

    for(const auto& layout : layouts)
    {
        char uuid[16];
        memcpy(uuid, libs::core::convertUuidToBin(layout.m_assetKindId).data(), 16);

        writeBits(finalCode, uuid);
        writeBits(finalCode, layout.m_assetsCount, sizeof(uint16_t));
        writeBits(finalCode, layout.m_reserved, sizeof(uint16_t));
    }

    libs::core::print("Writing assets...");

    // Size in bit
    constexpr int64_t componentSize{18};
    constexpr int64_t scaleXSize{componentSize};
    constexpr int64_t scaleYSize{componentSize};
    constexpr int64_t scaleZSize{componentSize};
    [[maybe_unused]] constexpr int64_t rotSize{5};
    [[maybe_unused]] constexpr int64_t unusedSize{5};

    assert(scaleXSize + scaleYSize + scaleZSize + rotSize + unusedSize == 64);

    for(const auto& layout : layouts)
    {
        for(const auto& asset : layout.m_assets)
        {
            const auto scale = asset.getRawScale();
            const auto rot = static_cast<int64_t>(asset.getRawRot());

            int64_t rawAsset = (rot << scaleZSize);
            rawAsset = (rawAsset | static_cast<int64_t>(scale.z())) << scaleYSize;
            rawAsset = (rawAsset | static_cast<int64_t>(scale.y())) << scaleXSize;
            rawAsset = (rawAsset | static_cast<int64_t>(scale.x()));

            writeBits(finalCode, rawAsset);
        }
    }

    return std::string(finalCode.str());
}

std::string saveLayouts(const std::vector<libs::core::Layout>& layouts)
{
    const auto uncompressedCode = getUncompressedCode(layouts);

    return "```" + base64::encode(gzip::compress(uncompressedCode)) + "```";
}

}