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

void Layout::replaceUUID(const boost::uuids::uuid& newId)
{
    boost::uuids::uuid& id = const_cast<boost::uuids::uuid&>(m_assetKindId);
    id = newId;
}

}