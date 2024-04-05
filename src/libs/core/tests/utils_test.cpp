#define BOOST_TEST_MODULE testCore

#include <libs/core/utils.hpp>

#include <boost/test/unit_test.hpp>
#include <boost/uuid/uuid_io.hpp>

BOOST_AUTO_TEST_SUITE(test_utils)

BOOST_AUTO_TEST_CASE(test_utils_convert_string)
{
    const std::string input{"857fea21-3c24-47e1-8901-5df910301dac"};

    const auto uuid = libs::core::convertStringToUuid(input);

    BOOST_CHECK_EQUAL(boost::uuids::to_string(uuid), input);
}

BOOST_AUTO_TEST_CASE(test_utils_convert_bin)
{
    const std::string input{"857fea21-3c24-47e1-8901-5df910301dac"};

    const auto uuid = libs::core::convertStringToUuid(input);

    BOOST_CHECK_EQUAL(boost::uuids::to_string(uuid), input);

    const auto bin = libs::core::convertUuidToBin(uuid);

    const auto uuidFromBin = libs::core::convertBinToUuid(bin.data());

    BOOST_CHECK_EQUAL(boost::uuids::to_string(uuidFromBin), input);
}

BOOST_AUTO_TEST_SUITE_END()