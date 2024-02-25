#pragma once

#include <boost/functional/hash.hpp>
#include <boost/uuid/uuid.hpp>
#include <opencv2/core/mat.hpp>

#include <string>
#include <unordered_map>

namespace libs::core {

struct AssetInfo
{
    std::string m_name;

    cv::Mat m_icon;
};

class AssetsDatabase
{
  public:
    static AssetsDatabase& getInstance();

    void init(const std::string& taleweaverFolderPath);

    const AssetInfo& getAsset(const boost::uuids::uuid& id) const;

  private:
    AssetsDatabase() = default;

    static constexpr auto s_DATA_FILENAME{"index.json"};

    std::unordered_map<boost::uuids::uuid, AssetInfo, boost::hash<boost::uuids::uuid>> m_assetsInfos;
};

}