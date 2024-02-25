#include "libs/reader/reader.hpp"

#include "libs/reader/base64.hpp"
#include "libs/reader/gzip.hpp"
#include "libs/reader/utils.hpp"

#include <libs/core/log.hpp>
#include <libs/core/utils.hpp>

#include <bitset>

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

    libs::core::print("Cleaned code is " + cleanedCode);

    const auto decodedCode = base64::decode(cleanedCode);

    libs::core::print("Decoded code is " + decodedCode);

    const auto decompressed = gzip::decompress(decodedCode);

    std::string decompressedCode(decompressed.cbegin(), decompressed.cend());

    libs::core::print("Decompressed code is " + decompressedCode);

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
    libs::core::print("Bytes read:" + std::to_string(decompressedStream.tellg()), libs::core::LogLevel::Trace);

    uint16_t version;
    readBits(decompressedStream, version);

    if(version != 2)
    {
        throw std::runtime_error("Version not supported");
    }

    libs::core::print("Bytes read:" + std::to_string(decompressedStream.tellg()), libs::core::LogLevel::Trace);

    uint16_t layoutCount;
    readBits(decompressedStream, layoutCount);

    libs::core::print("Bytes read:" + std::to_string(decompressedStream.tellg()), libs::core::LogLevel::Trace);

    uint16_t creatureCount;
    readBits(decompressedStream, creatureCount);

    libs::core::print("Bytes read:" + std::to_string(decompressedStream.tellg()), libs::core::LogLevel::Trace);

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
        u_int16_t assetCount, reserved;
        readBits(decompressedStream, assetKindId);
        readBits(decompressedStream, assetCount);
        readBits(decompressedStream, reserved);

        boost::uuids::uuid uuid = libs::core::convertBinToUuid(assetKindId);

        layouts.emplace_back(uuid, assetCount, reserved);
    }

    libs::core::print("Bytes read:" + std::to_string(decompressedStream.tellg()), libs::core::LogLevel::Trace);

    libs::core::print("Reading assets...");

    // Size in bit
    constexpr int componentSize{18};
    constexpr int scaleXSize{componentSize};
    constexpr int scaleYSize{componentSize};
    constexpr int scaleZSize{componentSize};
    constexpr int rotSize{5};
    constexpr int unusedSize{5};

    constexpr int scaleXOffset{0};
    constexpr int scaleYOffset{scaleXOffset + scaleXSize};
    constexpr int scaleZOffset{scaleYOffset + scaleYSize};
    constexpr int rotOffset{scaleZOffset + scaleZSize};
    constexpr int unusedOffset{rotOffset + rotSize};

    assert(scaleXSize + scaleYSize + scaleZSize + rotSize + unusedSize == 64);

    for(auto& layout : layouts)
    {
        for(auto assetIt = 0u; assetIt < layout.m_assetsCount; ++assetIt)
        {
            int64_t rawAsset;
            readBits(decompressedStream, rawAsset);

            [[maybe_unused]] long int unused;
            long int rot;
            Eigen::Vector3<long int> scale;

            scale.x() = (rawAsset >> scaleXOffset) & getMask(scaleXSize);
            scale.y() = (rawAsset >> scaleYOffset) & getMask(scaleYSize);
            scale.z() = (rawAsset >> scaleZOffset) & getMask(scaleZSize);
            rot = (rawAsset >> rotOffset) & getMask(rotSize);
            unused = (rawAsset >> unusedOffset) & getMask(unusedSize);

            layout.m_assets.emplace_back(rot, scale.cast<double>());
        }
    }

    libs::core::print("Bytes read:" + std::to_string(decompressedStream.tellg()), libs::core::LogLevel::Trace);

    return layouts;
}
}