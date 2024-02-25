#define BOOST_TEST_MODULE testReader

#include <libs/core/AssetsDatabase.hpp>
#include <libs/core/Layout.hpp>
#include <libs/core/log.hpp>
#include <libs/core/utils.hpp>
#include <libs/reader/base64.hpp>
#include <libs/reader/gzip.hpp>
#include <libs/reader/reader.hpp>

#include <boost/test/unit_test.hpp>

#include <sstream>

/**
 * @struct Fixture
 * @brief Struct used to run a function before all the tests
 */
struct Fixture
{
    /**
     * @brief Run before all the tests
     */
    Fixture()
    {
        libs::core::AssetsDatabase::getInstance().init("/mnt/d/SteamLibrary/steamapps/common/TaleSpire/Taleweaver");
    }
};

BOOST_AUTO_TEST_SUITE(test_reader)

BOOST_GLOBAL_FIXTURE(Fixture);

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

BOOST_AUTO_TEST_CASE(test_reader_encode_decode)
{
    const std::string original{"test="};
    const auto encoded = libs::reader::base64::encode(original);
    const auto decoded = libs::reader::base64::decode(encoded);

    BOOST_CHECK_EQUAL(decoded, original);
}
BOOST_AUTO_TEST_CASE(test_reader_compress_decompress)
{
    const std::string original{"test="};
    const auto compressed = libs::reader::gzip::compress(original);
    const auto decompressed = libs::reader::gzip::decompress(compressed);
    BOOST_CHECK_EQUAL(decompressed, original);
}

BOOST_AUTO_TEST_CASE(test_reader_full)
{
    const std::string original{"test="};
    const auto encoded = libs::reader::base64::encode(libs::reader::gzip::compress(original));
    const auto decompressed = libs::reader::gzip::decompress(libs::reader::base64::decode(encoded));
    BOOST_CHECK_EQUAL(decompressed, original);
}

BOOST_AUTO_TEST_CASE(test_reader_unique_grass)
{
    const std::string slabCode = "```H4sIAAAAAAAACjv369xFJgZGBgYGgUWHGX9Pme/S4z7T7pZdoRpIDAqYGRgAPkTNoSgAAAA=```";

    const auto layouts = libs::reader::getLayouts(slabCode);

    BOOST_CHECK_EQUAL(layouts.size(), 1);
    BOOST_CHECK_EQUAL(layouts.front().m_assetKindId,
                      libs::core::convertStringToUuid("01c3a210-94fb-449f-8c47-993eda3e7126"));
    BOOST_CHECK_EQUAL(layouts.front().m_assetsCount, 1);
    BOOST_CHECK_EQUAL(layouts.front().m_assets.size(), 1);
}

BOOST_AUTO_TEST_CASE(test_reader_unique_castle)
{
    const std::string slabCode = "```H4sIAAAAAAAACzv369xFJgZGBgYGjkvnjZIPm7jsEO9eOn/dW3GQGAIAAO5TP34oAAAA```";

    const auto layouts = libs::reader::getLayouts(slabCode);

    BOOST_CHECK_EQUAL(layouts.size(), 1);
    BOOST_CHECK_EQUAL(layouts.front().m_assetKindId,
                      libs::core::convertStringToUuid("32cfd208-c363-4434-b817-8ba59faeed17"));
    BOOST_CHECK_EQUAL(layouts.front().m_assetsCount, 1);
    BOOST_CHECK_EQUAL(layouts.front().m_assets.size(), 1);
}

BOOST_AUTO_TEST_CASE(test_reader_line)
{
    const std::string slabCode =
      "```H4sIAAAAAAAACjv369xFJgZGBgYGgUWHGX9Pme/S4z7T7pZdoRoLUCyCiQEEmCcwQugTDBCaAU4DAPuhm5lAAAAA```";

    const auto layouts = libs::reader::getLayouts(slabCode);

    BOOST_CHECK_EQUAL(layouts.size(), 1);
    BOOST_CHECK_EQUAL(layouts.front().m_assetsCount, 4);
    BOOST_CHECK_EQUAL(layouts.front().m_assets.size(), 4);
}

BOOST_AUTO_TEST_CASE(test_reader_multiple_layouts)
{
    const std::string slabCode =
      "```H4sIAAAAAAAACjv369xFJgYmBgYGgUWHGX9Pme/"
      "S4z7T7pZdoRoLUCwvwdB79kdlz65XBhlq2x9ag8QigIobeBiYJzBC6BMMEJoBSoPkQQAkDwInGNABAF7cA+l0AAAA```";

    const auto layouts = libs::reader::getLayouts(slabCode);

    BOOST_CHECK_EQUAL(layouts.size(), 2);

    BOOST_CHECK_EQUAL(layouts.at(0).m_assetsCount, 4);
    BOOST_CHECK_EQUAL(layouts.at(0).m_assets.size(), 4);

    BOOST_CHECK_EQUAL(layouts.at(1).m_assetsCount, 4);
    BOOST_CHECK_EQUAL(layouts.at(1).m_assets.size(), 4);
}

BOOST_AUTO_TEST_SUITE_END()