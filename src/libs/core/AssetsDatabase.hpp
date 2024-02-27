#pragma once

#include <boost/functional/hash.hpp>
#include <boost/uuid/uuid.hpp>
#include <opencv2/core/mat.hpp>

#include <optional>
#include <string>
#include <unordered_map>

namespace libs::core {

/**
 * @struct AssetInfo
 * @brief Define the information of an assets retrieved in the json file
 */
struct AssetInfo
{
    /// Tha name of the asset
    std::string m_name;

    /// The in-game icon of the asset
    cv::Mat m_icon;
};

/**
 * @class AssetsDatabase (Singleton)
 * @brief Stores all the information relarted to the assets from the json file
 */
class AssetsDatabase
{
  public:
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

  private:
    /**
     * @brief Construct a new Assets Database object
     */
    AssetsDatabase() = default;

    /// The name of the json file that contains the information about the assets
    static constexpr auto s_DATA_FILENAME{"index.json"};

    /// Extracted information from the json file
    std::unordered_map<boost::uuids::uuid, AssetInfo, boost::hash<boost::uuids::uuid>> m_assetsInfos;

    /// Whether the database was initialized or not
    bool m_isInitialize{false};
};

}