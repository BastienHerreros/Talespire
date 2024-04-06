#define BOOST_TEST_MODULE testReader

#include <libs/core/Layout.hpp>
#include <libs/core/log.hpp>
#include <libs/core/utils.hpp>
#include <libs/reader/reader.hpp>
#include <libs/reader/utils.hpp>
#include <libs/reader/writer.hpp>

#include <boost/test/unit_test.hpp>

#include <sstream>

BOOST_AUTO_TEST_SUITE(test_writer)

BOOST_AUTO_TEST_CASE(test_writer_read_write)
{
    uint32_t magicNumber{0xD1CEFACE};
    uint16_t version{2};
    uint16_t numCreatures{0};

    std::vector<libs::core::Layout> layouts;
    layouts.reserve(1);
    layouts.emplace_back(libs::core::convertStringToUuid("01c3a210-94fb-449f-8c47-993eda3e7126"), 1, 12);
    layouts.back().m_assets.emplace_back(15, Eigen::Vector3d{1, 2, 3});

    uint16_t numLayouts{static_cast<uint16_t>(layouts.size())};

    std::stringstream stream{libs::reader::getUncompressedCode(layouts)};

    {
        uint32_t read;
        libs::reader::readBits(stream, read);
        BOOST_CHECK_EQUAL(read, magicNumber);
    }

    {
        uint16_t read;
        libs::reader::readBits(stream, read);
        BOOST_CHECK_EQUAL(read, version);
    }

    {
        uint16_t read;
        libs::reader::readBits(stream, read);
        BOOST_CHECK_EQUAL(read, numLayouts);
    }

    {
        uint16_t read;
        libs::reader::readBits(stream, read);
        BOOST_CHECK_EQUAL(read, numCreatures);
    }

    {
        char assetKindId[16];
        uint16_t assetCount, reserved;
        libs::reader::readBits(stream, assetKindId);
        libs::reader::readBits(stream, assetCount);
        libs::reader::readBits(stream, reserved);

        boost::uuids::uuid uuid = libs::core::convertBinToUuid(assetKindId);

        BOOST_CHECK_EQUAL(uuid, layouts.front().m_assetKindId);
    }

    {
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

        int64_t rawAsset;
        libs::reader::readBits(stream, rawAsset);

        [[maybe_unused]] int64_t unused;
        int64_t rot;
        Eigen::Vector3<int64_t> scale;

        scale.x() = (rawAsset >> scaleXOffset) & libs::reader::getMask(scaleXSize);
        scale.y() = (rawAsset >> scaleYOffset) & libs::reader::getMask(scaleYSize);
        scale.z() = (rawAsset >> scaleZOffset) & libs::reader::getMask(scaleZSize);
        rot = (rawAsset >> rotOffset) & libs::reader::getMask(rotSize);
        unused = (rawAsset >> unusedOffset) & libs::reader::getMask(unusedSize);

        BOOST_CHECK_EQUAL(rot, 15);
        BOOST_CHECK_EQUAL(scale.x(), 1);
        BOOST_CHECK_EQUAL(scale.y(), 2);
        BOOST_CHECK_EQUAL(scale.z(), 3);
    }
}

// BOOST_AUTO_TEST_CASE(test_reader_unique_grass)
// {
//     // const std::string slabCode = "```H4sIAAAAAAAACjv369xFJgZGBgYGgUWHGX9Pme/S4z7T7pZdoRpIDAqYGRgAPkTNoSgAAAA=```";
//     const std::string slabCode = "```H4sIAAAAAAAACjv369xFJgYmBgYGV8sOe+Zcb6+FuwOFhA/"
//                                  "vvMUMFGP54+9t4qri2XJlvvU8595GkDpGhg1sCmA6hgVKM0FoDpA0SBsDiM/AAAD/PzDjXAAAAA==```";

//     const auto layouts = libs::reader::getLayouts(slabCode);

//     const auto res = libs::reader::saveLayouts(layouts);

//     libs::core::print("slabCode");
//     libs::core::print(slabCode);
//     libs::core::print("res");
//     libs::core::print(res);

//     BOOST_CHECK_EQUAL(res, slabCode);
// }

BOOST_AUTO_TEST_SUITE_END()