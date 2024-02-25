#include "libs/reader/reader.hpp"

#include "libs/reader/base64.hpp"
#include "libs/reader/gzip.hpp"
#include "libs/reader/utils.hpp"

#include <libs/core/log.hpp>

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

    std::vector<libs::core::Layout> layouts;
    layouts.reserve(layoutCount);
    for(auto layout = 0u; layout < layoutCount; ++layout)
    {
        char assetKindId[16];
        u_int16_t assetCount, reserved;
        readBits(decompressedStream, assetKindId);
        readBits(decompressedStream, assetCount);
        readBits(decompressedStream, reserved);

        boost::uuids::uuid uuid;
        memcpy(&uuid, assetKindId, 16);
        layouts.emplace_back(uuid, assetCount, reserved);

        libs::core::print(uuid);
    }

    for(auto& layout : layouts)
    {
        for(auto assetIt = 0u; assetIt < layout.m_assetsCount; ++assetIt)
        {
            char rawAsset[64];
            readBits(decompressedStream, rawAsset);

            std::stringstream assetStream{rawAsset};

            double unused, rot;
            Eigen::Vector3d scale;
            readBits(assetStream, unused, 5u);
            readBits(assetStream, rot, 5u);
            readBits(assetStream, scale.x(), 18u);
            readBits(assetStream, scale.y(), 18u);
            readBits(assetStream, scale.z(), 18u);

            layout.m_assets.emplace_back(rot, scale);
        }
    }

    if(!decompressedStream.eof())
    {
        throw std::runtime_error("End of file not reached");
    }

    return layouts;
}
}