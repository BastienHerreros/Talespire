#include "models/AssetModel.hpp"

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
        case AssetModelRoles::PoseRole: {
            return QVariant::fromValue(m_assets.at(static_cast<size_t>(index.row())).m_pos);
        }
        case AssetModelRoles::RotationRole: {
            return QVariant::fromValue(m_assets.at(static_cast<size_t>(index.row())).m_rot);
        }
        default: {
            return QVariant();
        }
    }
}

QHash<int, QByteArray> AssetModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[static_cast<int>(AssetModelRoles::PoseRole)] = "position";
    roles[static_cast<int>(AssetModelRoles::RotationRole)] = "rotation";
    return roles;
}

void AssetModel::insertAsset(const libs::core::Asset& asset)
{
    const auto row = static_cast<int>(m_assets.size());

    beginInsertRows(QModelIndex(), row, row);

    const auto floatPos = asset.m_pos.cast<float>();

    QtAsset qtAsset;
    qtAsset.m_pos = QVector3D{floatPos.x(), floatPos.y(), floatPos.z()};
    qtAsset.m_rot = asset.m_rot;

    m_assets.emplace_back(qtAsset);

    endInsertRows();
}

void AssetModel::clear()
{
    beginRemoveRows(QModelIndex(), 0, rowCount());
    m_assets.clear();
    endRemoveRows();
}

}
}