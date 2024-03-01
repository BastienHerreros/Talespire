#include "libs/core/utils.hpp"

#include "libs/core/log.hpp"

#include <boost/lexical_cast.hpp>
#include <boost/uuid/uuid_io.hpp>

namespace libs::core {

boost::uuids::uuid convertStringToUuid(const std::string& id) { return boost::lexical_cast<boost::uuids::uuid>(id); }

boost::uuids::uuid convertBinToUuid(const char bin[16])
{
    const auto swap = [](const boost::uuids::uuid& original, boost::uuids::uuid& result, size_t begin, size_t end) {
        for(auto i = 0u; i < end - begin + 1; ++i)
        {
            result.data[begin + i] = original.data[end - i];
        }
    };

    boost::uuids::uuid uuid;
    memcpy(&uuid, bin, 16);

    // Swap first bytes
    boost::uuids::uuid result = uuid;

    swap(uuid, result, 0, 3);
    swap(uuid, result, 4, 5);
    swap(uuid, result, 6, 7);

    libs::core::print("uuid");
    libs::core::print(result);

    return result;
}

}