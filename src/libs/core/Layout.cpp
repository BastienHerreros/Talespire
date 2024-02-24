#include "libs/core/Layout.hpp"

namespace libs::core {

Layout::Layout(const std::string& assetKindId, uint16_t assetsCount, uint16_t reserved)
  : m_assetKindId(assetKindId)
  , m_assetsCount(assetsCount)
  , m_reserved(reserved)
{
    m_assets.reserve(assetsCount);
}

}