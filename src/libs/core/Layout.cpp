#include "libs/core/Layout.hpp"

#include "libs/core/utils.hpp"

namespace libs::core {

Layout::Layout()
  : m_assetKindId(libs::core::convertStringToUuid("00000000-0000-0000-0000-000000000000"))
  , m_assetsCount(0)
  , m_reserved(0)
{
    m_assets.reserve(m_assetsCount);
}

Layout::Layout(const boost::uuids::uuid& assetKindId, uint16_t assetsCount, uint16_t reserved)
  : m_assetKindId(assetKindId)
  , m_assetsCount(assetsCount)
  , m_reserved(reserved)
{
    m_assets.reserve(assetsCount);
}

Layout::Layout(Layout&& other)
  : m_assetKindId(std::move(other.m_assetKindId))
  , m_assetsCount(std::move(other.m_assetsCount))
  , m_reserved(std::move(other.m_reserved))
  , m_assets(std::move(other.m_assets))
{}

Layout::Layout(const Layout& other)
  : m_assetKindId(other.m_assetKindId)
  , m_assetsCount(other.m_assetsCount)
  , m_reserved(other.m_reserved)
  , m_assets(other.m_assets)
{}

Layout& Layout::operator=(Layout&& other)
{
    const_cast<boost::uuids::uuid&>(m_assetKindId) = std::move(other.m_assetKindId);
    const_cast<uint16_t&>(m_assetsCount) = std::move(other.m_assetsCount);
    const_cast<uint16_t&>(m_reserved) = std::move(other.m_reserved);
    m_assets = std::move(other.m_assets);

    return *this;
}

Layout& Layout::operator=(const Layout& other)
{
    const_cast<boost::uuids::uuid&>(m_assetKindId) = other.m_assetKindId;
    const_cast<uint16_t&>(m_assetsCount) = other.m_assetsCount;
    const_cast<uint16_t&>(m_reserved) = other.m_reserved;
    m_assets = other.m_assets;

    return *this;
}

void Layout::replaceUUID(const boost::uuids::uuid& newId)
{
    boost::uuids::uuid& id = const_cast<boost::uuids::uuid&>(m_assetKindId);
    id = newId;
}

}