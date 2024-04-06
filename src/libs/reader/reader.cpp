#include "libs/reader/reader.hpp"

#include "libs/reader/base64.hpp"
#include "libs/reader/gzip.hpp"
#include "libs/reader/utils.hpp"

#include <libs/core/log.hpp>
#include <libs/core/utils.hpp>

namespace libs::reader {

std::string cleanSlabCode(const std::string& slabCode)
{
    const auto firstChar = slabCode.find_first_not_of(" ");
    const auto lastChar = slabCode.find_last_not_of(" ");

    std::string cleanedCode = slabCode.substr(firstChar, lastChar - firstChar + 1);

    if(cleanedCode.find("`") != std::string::npos)
    {
        cleanedCode = cleanedCode.substr(3, cleanedCode.size() - 6);
    }

    return cleanedCode;
}

std::string readSlabCode(const std::string& slabCode)
{
    const auto cleanedCode = cleanSlabCode(slabCode);

    const auto decodedCode = base64::decode(cleanedCode);

    const auto decompressed = gzip::decompress(decodedCode);

    std::string decompressedCode(decompressed.cbegin(), decompressed.cend());

    return decompressedCode;
}

std::vector<libs::core::Layout> getLayouts(const std::string& slabCode)
{
    auto decompressedCode = libs::reader::readSlabCode(slabCode);

    if(decompressedCode.empty())
    {
        throw std::runtime_error("Cannot decompressed code");
    }

    std::stringstream decompressedStream{decompressedCode};

    libs::core::print("Reading header...");

    uint32_t magicNumber;
    readBits(decompressedStream, magicNumber);

    if(magicNumber != 0xD1CEFACE)
    {
        throw std::runtime_error("Magic hex not found");
    }

    uint16_t version;
    readBits(decompressedStream, version);

    if(version != 2)
    {
        throw std::runtime_error("Version not supported");
    }

    uint16_t layoutCount;
    readBits(decompressedStream, layoutCount);

    uint16_t creatureCount;
    readBits(decompressedStream, creatureCount);

    if(creatureCount != 0)
    {
        throw std::runtime_error("No creatures should be in the slab for version 2");
    }

    libs::core::print("Reading layouts...");

    std::vector<libs::core::Layout> layouts;
    layouts.reserve(layoutCount);
    for(auto layoutIt = 0u; layoutIt < layoutCount; ++layoutIt)
    {
        char assetKindId[16];
        uint16_t assetCount, reserved;
        readBits(decompressedStream, assetKindId);
        readBits(decompressedStream, assetCount);
        readBits(decompressedStream, reserved);

        boost::uuids::uuid uuid = libs::core::convertBinToUuid(assetKindId);

        layouts.emplace_back(uuid, assetCount, reserved);
    }

    libs::core::print("Reading assets...");

    // Size in bit
    constexpr int64_t componentSize{18};
    constexpr int64_t scaleXSize{componentSize};
    constexpr int64_t scaleYSize{componentSize};
    constexpr int64_t scaleZSize{componentSize};
    constexpr int64_t rotSize{5};
    constexpr int64_t unusedSize{5};

    constexpr int64_t scaleXOffset{0};
    constexpr int64_t scaleYOffset{scaleXOffset + scaleXSize};
    constexpr int64_t scaleZOffset{scaleYOffset + scaleYSize};
    constexpr int64_t rotOffset{scaleZOffset + scaleZSize};
    constexpr int64_t unusedOffset{rotOffset + rotSize};

    assert(scaleXSize + scaleYSize + scaleZSize + rotSize + unusedSize == 64);

    for(auto& layout : layouts)
    {
        for(auto assetIt = 0u; assetIt < layout.m_assetsCount; ++assetIt)
        {
            int64_t rawAsset;
            readBits(decompressedStream, rawAsset);

            [[maybe_unused]] int64_t unused;
            int64_t rot;
            Eigen::Vector3<int64_t> scale;

            scale.x() = (rawAsset >> scaleXOffset) & getMask(scaleXSize);
            scale.y() = (rawAsset >> scaleYOffset) & getMask(scaleYSize);
            scale.z() = (rawAsset >> scaleZOffset) & getMask(scaleZSize);
            rot = (rawAsset >> rotOffset) & getMask(rotSize);
            unused = (rawAsset >> unusedOffset) & getMask(unusedSize);

            layout.m_assets.emplace_back(rot, scale.cast<double>());
        }
    }

    return layouts;
}
}