#define BOOST_TEST_MODULE testReader

#include <libs/core/log.hpp>

#include <boost/iostreams/filter/gzip.hpp>
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(test_reader)

BOOST_AUTO_TEST_CASE(test_reader)
{
    typedef std::vector<char> buffer_t;

    const std::string slabCode = "```H4sIAAAAAAAACjv369xFJgYmBgaGi1nmuhvOiThsyVdoiOU9X8EMFGP54+"
                                 "9t4qri2XJlvvU8595GkDpGhiXsDGA6gRVCO4CUAgEHkwEDAzOIYwDmAwDgtvXTXAAAAA==```";

    const auto cleanedStr = slabCode.substr(3, slabCode.size() - 6);

    buffer_t compressed;
    std::copy(cleanedStr.begin(), cleanedStr.end(), std::back_inserter(compressed));

    buffer_t decompressed;

    boost::iostreams::filtering_ostream os;

    os.push(boost::iostreams::gzip_decompressor());
    os.push(boost::iostreams::back_inserter(decompressed));

    boost::iostreams::write(os, &compressed[0], static_cast<std::streamsize>(compressed.size()));

    std::string res(decompressed.begin(), decompressed.end());

    libs::core::print(libs::core::LogLevel::Debug, res);
}

BOOST_AUTO_TEST_SUITE_END()