#pragma once

#include <Eigen/Dense>

namespace libs::core {

struct Asset
{
    Asset(double rot, const Eigen::Vector3d& scale);

    const double m_rot;
    const Eigen::Vector3d m_pos;
};

}