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

BOOST_AUTO_TEST_CASE(test_reader_slab_new)
{
    const std::string slabCode = "```H4sIAAAAAAAACjv369xFJgZGBgaGNuWqSddSZjltrmjaz6rFexkkhgAA6ChE2CgAAAA```";

    const auto layouts = libs::reader::getLayouts(slabCode);

    BOOST_CHECK_EQUAL(layouts.size(), 1);
    BOOST_CHECK_EQUAL(layouts.front().m_assetsCount, 1);
    BOOST_CHECK_EQUAL(layouts.front().m_assets.size(), 1);
}

BOOST_AUTO_TEST_CASE(test_reader_multiple_same)
{
    const std::string slabCode =
      "```H4sIAAAAAAAACjv369xFJgZGBgaGNuWqSddSZjltrmjaz6rFe5mFAQ0YMTAoMEOYYNoIxAIALTK1lUAAAAA```";

    const auto layouts = libs::reader::getLayouts(slabCode);

    BOOST_CHECK_EQUAL(layouts.size(), 1);
    BOOST_CHECK_EQUAL(layouts.front().m_assetsCount, 4);
    BOOST_CHECK_EQUAL(layouts.front().m_assets.size(), 4);
}

BOOST_AUTO_TEST_CASE(test_reader_slab_old)
{
    const std::string slabCode = "```H4sIAAAAAAAACzv369xFJgZGBgYGjkvnjZIPm7jsEO9eOn/dW3GQGAIAAO5TP34oAAAA```";

    const auto layouts = libs::reader::getLayouts(slabCode);

    BOOST_CHECK_EQUAL(layouts.size(), 1);
    BOOST_CHECK_EQUAL(layouts.front().m_assetsCount, 1);
    BOOST_CHECK_EQUAL(layouts.front().m_assets.size(), 1);
}

BOOST_AUTO_TEST_SUITE_END()