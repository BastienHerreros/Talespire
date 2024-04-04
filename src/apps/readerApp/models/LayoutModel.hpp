#pragma once

#include "models/AssetModel.hpp"

#include <libs/core/AssetsDatabase.hpp>
#include <libs/core/Layout.hpp>

#include <QAbstractListModel>
#include <QImage>
#include <QVector>

#include <opencv2/core/mat.hpp>

#include <memory>
#include <vector>

Q_DECLARE_METATYPE(const app::models::AssetModel*)

namespace app {
namespace models {

/**
 * @struct QtLayout
 * @brief Store the data of a layout in the Qt format
 */
struct QtLayout
{
    /// Index in the list
    int m_index;

    /// UUID of the asset
    boost::uuids::uuid m_uuid;

    /// The image from Qt
    QImage m_qtImage;

    /// The name of the asset
    QString m_assetName;

    /// The number of instances of that asset
    int m_number;

    /// Model that contains all the assets of a layout
    std::unique_ptr<AssetModel> m_assetModel;

    /// The type of the asset as a string
    QString m_assetType;

    /// The type of the asset as an enum
    libs::core::AssetType m_enumType;
};

/**
 * @class LayoutModel
 * @brief Model used to display the layouts
 */
class LayoutModel : public QAbstractListModel
{
  private:
    Q_OBJECT
  public:
    /**
     * @enum LayoutModelRoles
     * @brief Define the data stored in the model
     */
    enum class LayoutModelRoles
    {
        IndexRole = Qt::UserRole,
        QtImageRole,
        NameRole,
        NumberRole,
        AssetRole,
        AssetTypeRole
    };

    /**
     * @brief Construct a new LayoutModel object
     * @param [in] parent The parent object
     */
    explicit LayoutModel(QObject* parent = nullptr);

    /**
     * @brief Destroy the LayoutModel object
     */
    virtual ~LayoutModel() override = default;

    /**
     * @brief Returns the number of rows under the given parent
     * @param [in] parent The root node from where we will count
     * @return The number of rows under the given parent
     * @remark When the parent is valid it means that rowCount is returning the number of children of the parent.
     */
    int rowCount(const QModelIndex& parent = QModelIndex()) const final;

    /**
     * @brief  Returns the data stored under the given role for the item referred to by the index.
     * @param [in] index The index of the item
     * @param [in] role The id of the data that we want to retrieve
     * @return The data stored under the given role for the item referred to by the index
     */
    QVariant data(const QModelIndex& index, int role) const final;

    /**
     * @brief Returns the model's role names
     * @return The model's role names
     */
    QHash<int, QByteArray> roleNames() const final;

    /**
     * @brief Insert a new layout
     * @param [in] layout The layout to insert
     * @param [in] info The information of a layout
     */
    void insertLayout(const libs::core::Layout& layout, const libs::core::AssetInfo& info);

    /**
     * @brief Clear the model
     */
    void clear();

  private:
    /// Sub-images
    std::vector<QtLayout> m_layouts;
};
}
}