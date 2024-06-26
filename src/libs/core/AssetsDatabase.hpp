#pragma once

#include <boost/functional/hash.hpp>
#include <boost/uuid/uuid.hpp>
#include <opencv2/core/mat.hpp>

#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

namespace libs::core {

using Tag = std::string;

/**
 * @enum AssetType
 * @brief Define the type of the loaded asset
 */
enum class AssetType
{
    Tile,
    Prop,
    Creature
};

/**
 * @struct AssetInfo
 * @brief Define the information of an assets retrieved in the json file
 */
struct AssetInfo
{
    /// The type of the asset
    AssetType m_type;

    /// The name of the asset
    std::string m_name;

    /// The in-game icon of the asset
    cv::Mat m_icon;

    /// Tags
    std::vector<Tag> m_tags;

    /// Tag of the group
    std::string m_groupTag;

    /// In-game folder name
    std::string m_folder;

    /// Can the asset be interact with
    bool m_isInteractable;
};

/**
 * @class AssetsDatabase (Singleton)
 * @brief Stores all the information relarted to the assets from the json file
 */
class AssetsDatabase
{
  public:
    /// Type of the map that will store the json data
    using AssetMap = std::unordered_map<boost::uuids::uuid, AssetInfo, boost::hash<boost::uuids::uuid>>;

    /**
     * @brief Get the Instance object
     * @return The unique instance
     */
    static AssetsDatabase& getInstance();

    /**
     * @brief Load the data from the jsons
     * @param [in] taleweaverFolderPath The path to the taleweaver folder
     */
    void init(const std::string& taleweaverFolderPath);

    /**
     * @brief Retrieve the information about an asset
     * @param [in] id The uuid of the asset
     * @return The information about an asset
     */
    std::optional<AssetInfo> getAsset(const boost::uuids::uuid& id) const;

    /**
     * @brief Check whether the database was initialized or not
     * @return Whether the database was initialized or not
     */
    bool isInitialized() const;

    /**
     * @brief Get the full map
     * @return The full map
     */
    const AssetMap& map() const;

  private:
    /**
     * @brief Construct a new Assets Database object
     */
    AssetsDatabase() = default;

    /// The name of the json file that contains the information about the assets
    static constexpr auto s_DATA_FILENAME{"index.json"};

    /// Extracted information from the json file
    AssetMap m_assetsInfos;

    /// Whether the database was initialized or not
    bool m_isInitialize{false};
};

}