#pragma once

#include "models/LayoutModel.hpp"

#include <libs/core/AssetsDatabase.hpp>

#include <QObject>
#include <QString>

#include <thread>

namespace app::controllers {

/**
 * @class ReaderCtrl
 * @brief Controller used to manager the parsing/writing of a slab code
 */
class ReaderCtrl : public QObject
{
    Q_OBJECT
  private:
    using LayoutModel = app::models::LayoutModel; // needed by qt

    Q_PROPERTY(LayoutModel* model READ getModel NOTIFY modelChanged)
    Q_PROPERTY(LayoutModel* fullModel READ getFullModel NOTIFY fullModelChanged)
    Q_PROPERTY(bool dataBaseInitialized READ isDatabaseInitialized NOTIFY databaseStatusChanged)

  public:
    /**
     * @brief Construct a new Reader Ctrl object
     */
    ReaderCtrl();

    /**
     * @brief Destroy the Reader Ctrl object
     */
    ~ReaderCtrl();

    /**
     * @brief Get the model that contains the layouts from the slab code
     * @return The model that contains the layouts from the slab code
     */
    LayoutModel* getModel();

    /**
     * @brief Get the model that contains all the available layouts
     * @return The model that contains all the available layouts
     */
    LayoutModel* getFullModel();

    /**
     * @brief Whether database of assets is initialized or not
     * @return Whether database of assets is initialized or not
     */
    bool isDatabaseInitialized() const;

    /**
     * @brief Init the asset database
     * @param [in] path  The path to the taleweaver folder
     */
    Q_INVOKABLE void initDatabase(const QUrl& path);

    /**
     * @brief Load list of layouts froma  slab code
     * @param [in] slabCode The slab code from talespire
     */
    Q_INVOKABLE void loadSlab(const QString& slabCode);

    /**
     * @brief Replace an asset with another one
     * @param [in] indexFrom The id (in the model) of the asset to replace
     * @param [in] indexTo The id (in the full model) of the asset to set
     */
    Q_INVOKABLE void replaceAsset(int indexFrom, int indexTo);

  signals:
    /**
     * @brief Signal used to notify that the model has changed
     */
    void modelChanged() const;

    /**
     * @brief Signal used to notify that the full model has changed
     */
    void fullModelChanged() const;

    /**
     * @brief Signal used to notify that the database status (loaded or not) has changed
     */
    void databaseStatusChanged() const;

    /**
     * @brief Signal used to display an error
     */
    void error(const QString& message) const;

    /**
     * @brief Signal used to notify that the loading has begun
     */
    void initDatabaseBegin() const;

    /**
     * @brief Signal used to notify that the loading has ended
     */
    void initDatabaseEnd() const;

    /**
     * @brief Signal used to notify that a new asset has been loaded from a slab code
     * @param [in] asset The loaded asset (image, name, ...)
     * @param [in] layout The layout related to the asset (pos, rot, ...)
     */
    void newAssetLoaded(const libs::core::AssetInfo& asset, const libs::core::Layout& layout) const;

    /**
     * @brief Signal used to notify that a new slab code has been generated
     */
    void newSlabCode(const QString& newCode) const;

  private slots:
    /**
     * @brief Slot called when a new asset has been loaded from a slab code
     * @param [in] asset The loaded asset (image, name, ...)
     * @param [in] layout The layout related to the asset (pos, rot, ...)
     */
    void onNewAssetLoaded(const libs::core::AssetInfo& asset, const libs::core::Layout& layout);

    /**
     * @brief Slot called when the loading of the database has ended
     */
    void onInitDatabaseEnd();

  private:
    /// Thread used to load a slab code
    std::thread m_tread;

    /// Thread used to initialized the asset database
    std::thread m_databaseTread;

    /// The model that contains the layouts from the slab code
    LayoutModel m_model;

    /// The model that contains all the available layouts
    LayoutModel m_fullModel;

    /// Asset database (singleton)
    libs::core::AssetsDatabase& m_database;

    /// List of the last layouts loaded from a slab code
    std::vector<libs::core::Layout> m_lastLoadedSlab;

    /// List of the all the available layouts
    std::vector<libs::core::Layout> m_allAssets;
};

}