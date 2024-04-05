#pragma once

#include <Eigen/Dense>

namespace libs::core {

/**
 * @struct Asset
 * @brief Store all the information of an asset
 */
struct Asset
{
    /**
     * @brief Construct a new Asset object
     * @param [in] rot The rotation value from the slab code (rot in degree / 15)
     * @param [in] scale The scale value from the slab code (pos * 100)
     */
    Asset(double rot, const Eigen::Vector3d& scale);

    /**
     * @brief Get the raw scale (pos / 100)
     * @return The raw scale (pos / 100)
     */
    Eigen::Vector3d getRawScale() const;

    /**
     * @brief Get the raw rotation (rot in degree * 15)
     * @return The raw rotation (rot in degree * 15)
     */
    double getRawRot() const;

    /// The rotation in degree
    const double m_rot;

    /// The position on the board
    const Eigen::Vector3d m_pos;
};

}