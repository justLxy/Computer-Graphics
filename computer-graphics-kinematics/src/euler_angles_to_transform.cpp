#include "euler_angles_to_transform.h"

Eigen::Affine3d euler_angles_to_transform(
  const Eigen::Vector3d & xzx)
{
  /////////////////////////////////////////////////////////////////////////////
  // Build extrinsic X-Z-X rotation (twist-bend-twist) from degrees.
  const auto deg2rad = [](double deg)->double { return deg * M_PI / 180.0; };
  const double a1 = deg2rad(xzx[0]);
  const double a2 = deg2rad(xzx[1]);
  const double a3 = deg2rad(xzx[2]);
  const Eigen::AngleAxisd Rx1(a1, Eigen::Vector3d::UnitX());
  const Eigen::AngleAxisd Rz (a2, Eigen::Vector3d::UnitZ());
  const Eigen::AngleAxisd Rx3(a3, Eigen::Vector3d::UnitX());
  Eigen::Affine3d A = Eigen::Affine3d::Identity();
  A.linear() = (Rx3 * Rz * Rx1).toRotationMatrix();
  A.translation().setZero();
  return A;
  /////////////////////////////////////////////////////////////////////////////
}
