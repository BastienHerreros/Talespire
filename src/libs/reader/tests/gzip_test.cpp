#define BOOST_TEST_MODULE testReader

#include <libs/reader/gzip.hpp>

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(test_gzip)

BOOST_AUTO_TEST_CASE(test_gzip_compress_decompress)
{
    const std::string original{"test="};
    const auto compressed = libs::reader::gzip::compress(original);
    const auto decompressed = libs::reader::gzip::decompress(compressed);
    BOOST_CHECK_EQUAL(decompressed, original);
}

BOOST_AUTO_TEST_SUITE_END()