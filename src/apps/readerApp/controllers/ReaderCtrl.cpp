#include "controllers/ReaderCtrl.hpp"

#include <libs/reader/reader.hpp>

#include <QSettings>
#include <QUrl>

#include <exception>

namespace app::controllers {

ReaderCtrl::ReaderCtrl()
  : QObject(nullptr)
  , m_database(libs::core::AssetsDatabase::getInstance())
{
    qRegisterMetaType<libs::core::AssetInfo>("libs::core::AssetInfo");
    QObject::connect(this, &ReaderCtrl::newAssetLoaded, this, &ReaderCtrl::onNewAssetLoaded);
}

ReaderCtrl::~ReaderCtrl()
{
    if(m_tread.joinable())
    {
        m_tread.join();
    }

    if(m_databaseTread.joinable())
    {
        m_databaseTread.join();
    }
}

ReaderCtrl::AssetModel* ReaderCtrl::getModel() { return &m_model; }

bool ReaderCtrl::isDatabaseInitialized() const { return m_database.isInitialized(); }

void ReaderCtrl::initDatabase(const QUrl& path)
{
    if(m_databaseTread.joinable())
    {
        m_databaseTread.join();
    }

    m_databaseTread = std::thread(
      [this](QUrl localUrl) {
          emit initDatabaseBegin();

          const QString settingCategory{"path/taleWeaver"};
          QSettings settings;
          settings.setValue(settingCategory, localUrl);

          try
          {
              QString localPath = localUrl.toString();
              if(localPath.contains("file://"))
              {
                  localPath = localUrl.toLocalFile();
              }

              m_database.init(localPath.toStdString());
              emit databaseStatusChanged();

              emit initDatabaseEnd();
          }
          catch(const std::exception& e)
          {
              emit error(QString(e.what()));
              return;
          }
      },
      path);
}

void ReaderCtrl::loadSlab(const QString& slabCode)
{
    if(m_tread.joinable())
    {
        m_tread.join();
    }

    m_tread = std::thread(
      [this](QString localSlabCode) {
          const auto layouts = libs::reader::getLayouts(localSlabCode.toStdString());

          for(const auto& layout : layouts)
          {
              const auto& assetId = layout.m_assetKindId;
              const auto& asset = m_database.getAsset(assetId);
              emit newAssetLoaded(asset);
          }
      },
      slabCode);
}

void ReaderCtrl::onNewAssetLoaded(const libs::core::AssetInfo& asset)
{
    m_model.insertAsset(asset.m_name, asset.m_icon);
}

}