#include "libs/core/Asset.hpp"

#include "libs/core/log.hpp"

namespace libs::core {

Asset::Asset(double rot, const Eigen::Vector3d& scale)
  : m_rot(rot * 15.0)
  , m_pos(scale / 100.0)
{}

Asset::Asset(Asset&& other)
  : m_rot(std::move(other.m_rot))
  , m_pos(std::move(other.m_pos))
{}

Asset::Asset(const Asset& other)
  : m_rot(other.m_rot)
  , m_pos(other.m_pos)
{}

Asset& Asset::operator=(Asset&& other)
{
    const_cast<double&>(m_rot) = std::move(other.m_rot);
    const_cast<Eigen::Vector3d&>(m_pos) = std::move(other.m_pos);

    return *this;
}

Asset& Asset::operator=(const Asset& other)
{
    const_cast<double&>(m_rot) = other.m_rot;
    const_cast<Eigen::Vector3d&>(m_pos) = other.m_pos;

    return *this;
}

Eigen::Vector3d Asset::getRawScale() const { return m_pos * 100.0; }

double Asset::getRawRot() const { return m_rot / 15.0; }

}