#include "libs/core/Asset.hpp"

#include "libs/core/log.hpp"

namespace libs::core {

Asset::Asset(double rot, const Eigen::Vector3d& scale)
  : m_rot(rot * 15.0)
  , m_pos(scale / 100.0)
{}

Eigen::Vector3d Asset::getRawScale() const { return m_pos * 100.0; }

double Asset::getRawRot() const { return m_rot / 15.0; }

}