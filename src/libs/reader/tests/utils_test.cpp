#define BOOST_TEST_MODULE testReader

#include <libs/reader/utils.hpp>

#include <boost/test/unit_test.hpp>

#include <string>

BOOST_AUTO_TEST_SUITE(test_utils)

BOOST_AUTO_TEST_CASE(test_utils_masks)
{
    {
        const auto mask = libs::reader::getMask(1);
        BOOST_CHECK_EQUAL(mask, 0b0001);
    }

    {
        const auto mask = libs::reader::getMask(2);
        BOOST_CHECK_EQUAL(mask, 0b0011);
    }

    {
        const auto mask = libs::reader::getMask(3);
        BOOST_CHECK_EQUAL(mask, 0b0111);
    }

    {
        const auto mask = libs::reader::getMask(4);
        BOOST_CHECK_EQUAL(mask, 0b1111);
    }
}

BOOST_AUTO_TEST_CASE(test_utils_read_bytes)
{
    std::string input{"abcdefghij"};
    std::stringstream stream(input);

    {
        char read[4];
        libs::reader::readBits(stream, read);
        const std::string res(read);
        BOOST_CHECK_EQUAL(res, "abcd");
    }

    {
        char read[5];
        libs::reader::readBits(stream, read);
        const std::string res(read);
        BOOST_CHECK_EQUAL(res, "efghi");
    }
}

BOOST_AUTO_TEST_CASE(test_utils_read_bytes_errors)
{
    std::string input{"abcdefghij"};
    std::stringstream stream(input);

    char read[50];
    BOOST_CHECK_THROW(libs::reader::readBits(stream, read), std::runtime_error);
}

BOOST_AUTO_TEST_CASE(test_utils_read_size)
{
    std::string input{"abcdefghij"};
    std::stringstream stream(input);

    {
        char read[4];
        libs::reader::readBits(stream, read, 4);
        const std::string res(read);
        BOOST_CHECK_EQUAL(res, "abcd");
    }

    {
        char read[5];
        libs::reader::readBits(stream, read, 4);
        const std::string res(read);
        BOOST_CHECK_EQUAL(res, "efgh");
    }
}

BOOST_AUTO_TEST_CASE(test_utils_read_bytes_size_errors)
{
    std::string input{"abcdefghij"};
    std::stringstream stream(input);

    {
        char read[50];
        BOOST_CHECK_THROW(libs::reader::readBits(stream, read, 50), std::runtime_error);
    }

    {
        char read[1];
        BOOST_CHECK_THROW(libs::reader::readBits(stream, read, 5), std::runtime_error);
    }
}

BOOST_AUTO_TEST_SUITE_END()