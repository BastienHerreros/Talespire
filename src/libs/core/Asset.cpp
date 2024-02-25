#include "libs/core/Asset.hpp"

#include "libs/core/log.hpp"

namespace libs::core {

Asset::Asset(double rot, const Eigen::Vector3d& scale)
  : m_rot(rot * 15.0)
  , m_pos(scale / 100.0)
{}

}