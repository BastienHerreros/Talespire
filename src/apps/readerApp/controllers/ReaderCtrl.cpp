#include "controllers/ReaderCtrl.hpp"

#include <libs/core/log.hpp>
#include <libs/reader/reader.hpp>
#include <libs/reader/writer.hpp>

#include <QApplication>
#include <QClipboard>
#include <QSettings>
#include <QUrl>

#include <boost/uuid/uuid_io.hpp>

#include <exception>

namespace app::controllers {

ReaderCtrl::ReaderCtrl()
  : QObject(nullptr)
  , m_database(libs::core::AssetsDatabase::getInstance())
{
    qRegisterMetaType<libs::core::AssetInfo>("libs::core::AssetInfo");
    qRegisterMetaType<libs::core::Layout>("libs::core::Layout");
    QObject::connect(this, &ReaderCtrl::newAssetLoaded, this, &ReaderCtrl::onNewAssetLoaded);
    QObject::connect(
      this, &ReaderCtrl::initDatabaseEnd, this, &ReaderCtrl::onInitDatabaseEnd, Qt::ConnectionType::QueuedConnection);
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

ReaderCtrl::LayoutModel* ReaderCtrl::getModel() { return &m_model; }

ReaderCtrl::LayoutModel* ReaderCtrl::getFullModel() { return &m_fullModel; }

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

    m_model.clear();
    m_lastLoadedSlab.clear();

    m_tread = std::thread(
      [this](QString localSlabCode) {
          try
          {
              m_lastLoadedSlab = libs::reader::getLayouts(localSlabCode.toStdString());

              for(const auto& layout : m_lastLoadedSlab)
              {
                  const auto& assetId = layout.m_assetKindId;
                  const auto assetOpt = m_database.getAsset(assetId);

                  if(assetOpt.has_value())
                  {
                      emit newAssetLoaded(assetOpt.value(), layout);
                  }
                  else
                  {
                      libs::core::AssetInfo info;
                      info.m_name = boost::uuids::to_string(assetId);
                      info.m_icon = cv::Mat::zeros(128, 128, CV_8UC3);

                      emit newAssetLoaded(info, layout);
                  }
              }
          }
          catch(const std::exception& e)
          {
              emit error(QString(e.what()));
              return;
          }
      },
      slabCode);
}

void ReaderCtrl::replaceAsset(int indexFrom, int indexTo)
{
    if(indexFrom < 0 || indexTo < 0)
    {
        return;
    }

    libs::core::print("Replace " + std::to_string(indexFrom) + " to " + std::to_string(indexTo));

    const auto uIndexFrom = static_cast<size_t>(indexFrom);
    const auto uIndexTo = static_cast<size_t>(indexTo);

    if(uIndexFrom >= m_lastLoadedSlab.size() || uIndexTo >= m_allAssets.size())
    {
        return;
    }

    m_lastLoadedSlab.at(uIndexFrom).replaceUUID(m_allAssets.at(uIndexTo).m_assetKindId);

    const QString newCode = QString::fromStdString(libs::reader::saveLayouts(m_lastLoadedSlab));

    loadSlab(newCode);

    emit newSlabCode(newCode);
}

void ReaderCtrl::removeAsset(int index)
{
    if(index < 0)
    {
        return;
    }

    libs::core::print("Remove " + std::to_string(index));

    const auto uIndex = static_cast<size_t>(index);

    if(uIndex >= m_lastLoadedSlab.size())
    {
        return;
    }

    m_lastLoadedSlab.erase(m_lastLoadedSlab.begin() + index);

    const QString newCode = QString::fromStdString(libs::reader::saveLayouts(m_lastLoadedSlab));

    loadSlab(newCode);

    emit newSlabCode(newCode);
}

void ReaderCtrl::copyToClipboard(const QString& text)
{
    QClipboard* clipboard = QApplication::clipboard();
    clipboard->setText(text);
}

void ReaderCtrl::onNewAssetLoaded(const libs::core::AssetInfo& asset, const libs::core::Layout& layout)
{
    m_model.insertLayout(layout, asset);
    emit modelChanged();
}

void ReaderCtrl::onInitDatabaseEnd()
{
    for(const auto& [assetId, asset] : m_database.map())
    {
        if(asset.m_type == libs::core::AssetType::Creature)
        {
            continue;
        }

        libs::core::Layout layout(assetId, 1, 0);
        m_fullModel.insertLayout(layout, asset);

        m_allAssets.emplace_back(layout);
    }
    emit fullModelChanged();
}

}