#include "libs/core/AssetsDatabase.hpp"

#include "libs/core/utils.hpp"

#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>

#include <filesystem>
#include <istream>

namespace libs::core {

AssetsDatabase& AssetsDatabase::getInstance()
{
    static AssetsDatabase instance;
    return instance;
}

void AssetsDatabase::init(const std::string& taleweaverFolderPath)
{
    const std::array<std::string, 3> keysToRead = {"Tiles", "Props", "Creatures"};

    for(const auto& dirIt : std::filesystem::recursive_directory_iterator(taleweaverFolderPath))
    {
        if(dirIt.is_directory())
        {
            continue;
        }

        if(dirIt.path().filename().string() == s_DATA_FILENAME)
        {
            std::ifstream input(dirIt.path());

            if(!input.is_open())
            {
                throw std::runtime_error("Cannot open " + dirIt.path().string());
            }

            boost::property_tree::ptree root;
            boost::property_tree::read_json(input, root);

            for(const auto& key : keysToRead)
            {
                for(const auto& [name, child] : root.get_child(key))
                {
                    const boost::uuids::uuid id = convertStringToUuid(child.get_child("Id").get_value<std::string>());

                    if(m_assetsInfos.count(id) != 0)
                    {
                        throw std::logic_error("Id already exists");
                    }

                    AssetInfo info;
                    info.m_name = child.get_child("Name").get_value<std::string>();

                    m_assetsInfos.insert({id, info});
                }
            }
        }
    }
}

const AssetInfo& AssetsDatabase::getAsset(const boost::uuids::uuid& id) const
{
    if(m_assetsInfos.count(id) == 0)
    {
        throw std::invalid_argument("Id not found");
    }

    return m_assetsInfos.at(id);
}
}