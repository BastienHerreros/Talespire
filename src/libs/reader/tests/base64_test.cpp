#define BOOST_TEST_MODULE testReader

#include <libs/reader/base64.hpp>

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(test_base64)

BOOST_AUTO_TEST_CASE(test_base64_encode_decode)
{
    const std::string original{"test="};
    const auto encoded = libs::reader::base64::encode(original);
    const auto decoded = libs::reader::base64::decode(encoded);

    BOOST_CHECK_EQUAL(decoded, original);
}

BOOST_AUTO_TEST_SUITE_END()