#include "libs/core/AssetsDatabase.hpp"

#include "libs/core/log.hpp"
#include "libs/core/utils.hpp"

#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <opencv2/imgcodecs.hpp>

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

            const auto spriteSheetRoot = root.get_child("IconsAtlases");

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

                    // Read icon
                    {
                        const auto iconRoot = child.get_child("Icon");
                        const auto spriteSheetId = iconRoot.get_child("AtlasIndex").get_value<size_t>();

                        auto it = spriteSheetRoot.begin();
                        std::advance(it, spriteSheetId);

                        const auto spritesheetFilename = it->second.get_child("Path").get_value<std::string>();

                        const cv::Mat spriteSheet =
                          cv::imread(dirIt.path().parent_path() / spritesheetFilename, cv::IMREAD_UNCHANGED);

                        // Coordinates of the bottom left corner from the bottom left
                        const double relativeX = iconRoot.get_child("Region").get_child("x").get_value<double>(); // col
                        const double relativeY = iconRoot.get_child("Region").get_child("y").get_value<double>(); // row
                        const double relativeW = iconRoot.get_child("Region").get_child("width").get_value<double>();
                        const double relativeH = iconRoot.get_child("Region").get_child("height").get_value<double>();

                        const double wSpriteSheet = static_cast<double>(spriteSheet.cols);
                        const double hSpriteSheet = static_cast<double>(spriteSheet.rows);

                        const int x = static_cast<int>(relativeX * hSpriteSheet); // col
                        const int y =
                          static_cast<int>(hSpriteSheet - relativeY * wSpriteSheet - 128); // row => top left corner
                        const int w = static_cast<int>(relativeW * wSpriteSheet);          // nb col
                        const int h = static_cast<int>(relativeH * hSpriteSheet);          // nb row

                        info.m_icon = spriteSheet(cv::Rect(x, y, w, h)).clone();
                    }

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