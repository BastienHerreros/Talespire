#include "models/LayoutModel.hpp"

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

LayoutModel::LayoutModel(QObject* parent)
  : QAbstractListModel(parent)
{}

int LayoutModel::rowCount(const QModelIndex& parent) const { return static_cast<int>(m_layouts.size()); }

QVariant LayoutModel::data(const QModelIndex& index, int role) const
{
    if(!index.isValid())
    {
        return QVariant();
    }

    switch(static_cast<LayoutModelRoles>(role))
    {
        case LayoutModelRoles::QtImageRole: {
            return QVariant::fromValue(m_layouts.at(static_cast<size_t>(index.row())).m_qtImage);
        }
        case LayoutModelRoles::NameRole: {
            return QVariant::fromValue(m_layouts.at(static_cast<size_t>(index.row())).m_assetName);
        }
        case LayoutModelRoles::NumberRole: {
            return QVariant::fromValue(m_layouts.at(static_cast<size_t>(index.row())).m_number);
        }
        case LayoutModelRoles::AssetRole: {
            return QVariant::fromValue(m_layouts.at(static_cast<size_t>(index.row())).m_assetModel.get());
        }
        default: {
            return QVariant();
        }
    }
}

QHash<int, QByteArray> LayoutModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[static_cast<int>(LayoutModelRoles::QtImageRole)] = "qtImage";
    roles[static_cast<int>(LayoutModelRoles::NameRole)] = "assetName";
    roles[static_cast<int>(LayoutModelRoles::NumberRole)] = "numberOfInstance";
    roles[static_cast<int>(LayoutModelRoles::AssetRole)] = "assets";
    return roles;
}

void LayoutModel::insertLayout(const libs::core::Layout& layout, const libs::core::AssetInfo& info)
{
    const auto row = static_cast<int>(m_layouts.size());

    beginInsertRows(QModelIndex(), row, row);

    QtLayout& qtLayout = m_layouts.emplace_back();
    // qtLayout.m_uuid = layout.m_assetKindId;
    qtLayout.m_assetName = QString::fromStdString(info.m_name);
    qtLayout.m_qtImage = cvMatToQImage(info.m_icon);
    qtLayout.m_number = static_cast<int>(layout.m_assetsCount);

    qtLayout.m_assetModel = std::make_unique<AssetModel>();
    for(const auto& asset : layout.m_assets)
    {
        qtLayout.m_assetModel->insertAsset(asset);
    }

    endInsertRows();
}

void LayoutModel::clear()
{
    beginRemoveRows(QModelIndex(), 0, rowCount());
    m_layouts.clear();
    endRemoveRows();
}

}
}