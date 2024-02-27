#pragma once

#include "models/LayoutModel.hpp"

#include <libs/core/AssetsDatabase.hpp>

#include <QObject>
#include <QString>

#include <thread>

namespace app::controllers {

class ReaderCtrl : public QObject
{
    Q_OBJECT
  private:
    using LayoutModel = app::models::LayoutModel;
    Q_PROPERTY(LayoutModel* model READ getModel NOTIFY modelChanged)
    Q_PROPERTY(bool dataBaseInitialized READ isDatabaseInitialized NOTIFY databaseStatusChanged)

  public:
    ReaderCtrl();
    ~ReaderCtrl();

    LayoutModel* getModel();

    bool isDatabaseInitialized() const;

    Q_INVOKABLE void loadSlab(const QString& slabCode);

    Q_INVOKABLE void initDatabase(const QUrl& path);

  signals:
    void modelChanged() const;

    void databaseStatusChanged() const;

    void error(const QString& message) const;

    void initDatabaseBegin() const;

    void initDatabaseEnd() const;

    void newAssetLoaded(const libs::core::AssetInfo& asset, const libs::core::Layout& layout) const;

  private slots:
    void onNewAssetLoaded(const libs::core::AssetInfo& asset, const libs::core::Layout& layout);

  private:
    std::thread m_tread;
    std::thread m_databaseTread;

    LayoutModel m_model;

    libs::core::AssetsDatabase& m_database;
};

}