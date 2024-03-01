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
    const std::array<AssetType, 3> typesToRead = {AssetType::Tile, AssetType::Prop, AssetType::Creature};

    libs::core::print("Loading database from:");
    libs::core::print(taleweaverFolderPath);

    m_assetsInfos.clear();

    std::unordered_map<std::string, cv::Mat> cache;

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

            for(auto keyIt = 0u; keyIt < keysToRead.size(); ++keyIt)
            {
                const auto& key = keysToRead.at(keyIt);
                const auto type = typesToRead.at(keyIt);

                for(const auto& [name, child] : root.get_child(key))
                {
                    const boost::uuids::uuid id = convertStringToUuid(child.get_child("Id").get_value<std::string>());

                    if(m_assetsInfos.count(id) != 0)
                    {
                        throw std::logic_error("Id already exists");
                    }

                    AssetInfo info;
                    info.m_type = type;
                    info.m_name = child.get_child("Name").get_value<std::string>();
                    info.m_folder = child.get_child("Folder").get_value<std::string>();
                    info.m_groupTag = child.get_child("GroupTag").get_value<std::string>();

                    if(key == "Props")
                    {
                        info.m_isInteractable = child.get_child("IsInteractable").get_value<bool>();
                    }
                    else
                    {
                        info.m_isInteractable = false;
                    }

                    // Read icon
                    {
                        const auto iconRoot = child.get_child("Icon");
                        const auto spriteSheetId = iconRoot.get_child("AtlasIndex").get_value<size_t>();

                        auto it = spriteSheetRoot.begin();
                        std::advance(it, spriteSheetId);

                        const auto spritesheetFilename = it->second.get_child("Path").get_value<std::string>();

                        if(cache.count(spritesheetFilename) == 0)
                        {
                            const auto image =
                              cv::imread(dirIt.path().parent_path() / spritesheetFilename, cv::IMREAD_COLOR);

                            cache.insert({spritesheetFilename, image.clone()});
                        }

                        const cv::Mat& spriteSheet = cache.at(spritesheetFilename);

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

                    // Read tags
                    {
                        for(const auto& tag : child.get_child("Tags"))
                        {
                            info.m_tags.emplace_back(tag.second.get_value<std::string>());
                        }
                    }

                    m_assetsInfos.insert({id, info});
                }
            }
        }
    }

    libs::core::print("Loading done");

    m_isInitialize = true;
}

std::optional<AssetInfo> AssetsDatabase::getAsset(const boost::uuids::uuid& id) const
{
    if(!m_isInitialize)
    {
        throw std::runtime_error("You must initialize the database before using it");
    }

    if(m_assetsInfos.count(id) == 0)
    {
        return std::nullopt;
    }

    return m_assetsInfos.at(id);
}

bool AssetsDatabase::isInitialized() const { return m_isInitialize; }

const AssetsDatabase::AssetMap& AssetsDatabase::map() const { return m_assetsInfos; }

}