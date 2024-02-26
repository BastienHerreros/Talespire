#pragma once

#include <QAbstractListModel>
#include <QImage>
#include <QVector>

#include <opencv2/core/mat.hpp>

#include <vector>

namespace app {
namespace models {

/**
 * @struct QtAsset
 * @brief Store the data of an asset in the Qt format
 */
struct QtAsset
{
    /// The image from Qt
    QImage m_qtImage;

    /// The name of the asset
    QString m_assetName;

    /// The number of instances of that asset
    int m_number;
};

/**
 * @class AssetModel
 * @brief Model used to display the assets
 */
class AssetModel : public QAbstractListModel
{
  private:
    Q_OBJECT
  public:
    /**
     * @enum AssetModelRoles
     * @brief Define the data stored in the model
     */
    enum class AssetModelRoles
    {
        QtImageRole = Qt::UserRole,
        NameRole,
        NumberRole
    };

    /**
     * @brief Construct a new AssetModel object
     * @param [in] parent The parent object
     */
    explicit AssetModel(QObject* parent = nullptr);

    /**
     * @brief Destroy the AssetModel object
     */
    virtual ~AssetModel() override = default;

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
     * @brief Insert a new asset
     * @param [in] assetName The name of the asset
     * @param [in] image The image to insert
     * @param [in] numberOfInstance The number of instances
     */
    void insertAsset(const std::string& assetName, const cv::Mat& image, int numberOfInstance);

    /**
     * @brief Clear the model
     */
    void clear();

  private:
    /// Sub-images
    std::vector<QtAsset> m_assets;
};
}
}