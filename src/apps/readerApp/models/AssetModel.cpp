#include "models/AssetModel.hpp"

namespace {
/**
 * @brief Convert a colored cv mat to a QImage
 * @param [in] image The matrix to convert
 * @return The converted QImage
 */
QImage cvMatToQImage(const cv::Mat& image)
{
    return QImage(image.data, image.cols, image.rows, static_cast<int>(image.step), QImage::Format_BGR888).copy();
}
}

namespace app {
namespace models {

AssetModel::AssetModel(QObject* parent)
  : QAbstractListModel(parent)
{}

int AssetModel::rowCount(const QModelIndex& parent) const { return static_cast<int>(m_assets.size()); }

QVariant AssetModel::data(const QModelIndex& index, int role) const
{
    if(!index.isValid())
    {
        return QVariant();
    }

    switch(static_cast<AssetModelRoles>(role))
    {
        case AssetModelRoles::QtImageRole: {
            return QVariant::fromValue(m_assets.at(static_cast<size_t>(index.row())).m_qtImage);
        }
        case AssetModelRoles::NameRole: {
            return QVariant::fromValue(m_assets.at(static_cast<size_t>(index.row())).m_assetName);
        }
        default: {
            return QVariant();
        }
    }
}

QHash<int, QByteArray> AssetModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[static_cast<int>(AssetModelRoles::QtImageRole)] = "qtImage";
    roles[static_cast<int>(AssetModelRoles::NameRole)] = "assetName";
    return roles;
}

void AssetModel::insertAsset(const std::string& assetName, const cv::Mat& image)
{
    const auto row = static_cast<int>(m_assets.size());

    beginInsertRows(QModelIndex(), row, row);

    QtAsset asset;
    asset.m_assetName = QString::fromStdString(assetName);
    asset.m_qtImage = cvMatToQImage(image);

    m_assets.emplace_back(asset);

    endInsertRows();
}

}
}