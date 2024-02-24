#define BOOST_TEST_MODULE testReader

#include <libs/core/Layout.hpp>
#include <libs/core/log.hpp>
#include <libs/reader/base64.hpp>
#include <libs/reader/gzip.hpp>
#include <libs/reader/reader.hpp>

#include <boost/test/unit_test.hpp>

#include <sstream>

BOOST_AUTO_TEST_SUITE(test_reader)

BOOST_AUTO_TEST_CASE(test_reader_clean_code_same)
{
    const std::string refCode = "test1 test2";

    {
        const std::string slabCode = "```test1 test2```";
        BOOST_CHECK_EQUAL(libs::reader::cleanSlabCode(slabCode), refCode);
    }

    {
        const std::string slabCode = "test1 test2";
        BOOST_CHECK_EQUAL(libs::reader::cleanSlabCode(slabCode), refCode);
    }

    {
        const std::string slabCode = "  test1 test2     ";
        BOOST_CHECK_EQUAL(libs::reader::cleanSlabCode(slabCode), refCode);
    }

    {
        const std::string slabCode = "  ```test1 test2```";
        BOOST_CHECK_EQUAL(libs::reader::cleanSlabCode(slabCode), refCode);
    }

    {
        const std::string slabCode = "```test1 test2```    ";
        BOOST_CHECK_EQUAL(libs::reader::cleanSlabCode(slabCode), refCode);
    }

    {
        const std::string slabCode = "```test1 test2```    ";
        BOOST_CHECK_EQUAL(libs::reader::cleanSlabCode(slabCode), refCode);
    }

    {
        const std::string slabCode = "   ```test1 test2```    ";
        BOOST_CHECK_EQUAL(libs::reader::cleanSlabCode(slabCode), refCode);
    }
}

BOOST_AUTO_TEST_CASE(test_reader_compress_decompress)
{
    const std::string original{"test"};
    const auto encoded = libs::reader::base64::encode(libs::reader::gzip::compress(original));
    const auto decompressed = libs::reader::gzip::decompress(libs::reader::base64::decode(encoded));
    BOOST_CHECK_EQUAL(decompressed, original);
}

std::string stringToHex(const std::string& input)
{
    static const char hex_digits[] = "0123456789ABCDEF";

    std::string output;
    output.reserve(input.length() * 2);
    for(auto c : input)
    {
        output.push_back(hex_digits[c >> 4]);
        output.push_back(hex_digits[c & 15]);
    }
    return output;
}

// template<typename T>
// void deserializeUnmanagedAndAdvance(char* data, T& val)
// {
//     std::memcpy(&val, data, sizeof(T));
//     data += sizeof(T);
// }

template<typename T>
void readBits(std::stringstream& data, T& val)
{
    data.read(reinterpret_cast<char*>(&val), sizeof(T));
}

template<typename T>
void readBits(std::stringstream& data, T& val, std::streamsize size)
{
    data.read(reinterpret_cast<char*>(&val), size);
}

BOOST_AUTO_TEST_CASE(test_reader_slab)
{
    const std::string slabCode = "```H4sIAAAAAAAACzv369xFJgZGBgYGjkvnjZIPm7jsEO9eOn/dW3GQGAIAAO5TP34oAAAA```";

    auto res = libs::reader::readSlabCode(slabCode);

    std::stringstream ss{res};

    uint32_t magicNumber;
    readBits(ss, magicNumber);

    BOOST_CHECK_EQUAL(magicNumber, 0xD1CEFACE);

    uint16_t version;
    readBits(ss, version);

    BOOST_CHECK_EQUAL(version, 2);

    uint16_t layoutCount;
    readBits(ss, layoutCount);

    BOOST_CHECK_EQUAL(layoutCount, 1);

    uint16_t creatureCount;
    readBits(ss, creatureCount);

    BOOST_CHECK_EQUAL(creatureCount, 0);

    std::vector<libs::core::Layout> layouts;
    layouts.reserve(layoutCount);
    for(auto layout = 0u; layout < layoutCount; ++layout)
    {
        char assetKindId[16];
        u_int16_t assetCount, reserved;
        readBits(ss, assetKindId);
        readBits(ss, assetCount);
        readBits(ss, reserved);

        BOOST_CHECK_EQUAL(assetCount, 1);

        layouts.emplace_back(std::string(assetKindId), assetCount, reserved);
    }

    for(auto& layout : layouts)
    {
        char rawAsset[64];
        readBits(ss, rawAsset);

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

    libs::core::print((ss.eof() ? "end" : "not end"));
}

BOOST_AUTO_TEST_SUITE_END()