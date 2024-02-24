#define BOOST_TEST_MODULE testReader

#include <libs/core/log.hpp>
#include <libs/reader/reader.hpp>

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(test_reader)

BOOST_AUTO_TEST_CASE(test_reader)
{
    const std::string slabCode = "    ```H4sIAAAAAAAACjv369xFJgYmBgaGi1nmuhvOiThsyVdoiOU9X8EMFGP54+"
                                 "9t4qri2XJlvvU8595GkDpGhiXsDGA6gRVCO4CUAgEHkwEDAzOIYwDmAwDgtvXTXAAAAA==```  ";

    const auto res = libs::reader::readSlabCode(slabCode);

    libs::core::print(res.size());
}

BOOST_AUTO_TEST_SUITE_END()