#include <Eigen/Core>

Eigen::Vector3d reflect(const Eigen::Vector3d & in, const Eigen::Vector3d & n)
{
  ////////////////////////////////////////////////////////////////////////////
  // Replace with your code here:
  ////////////////////////////////////////////////////////////////////////////
  // Mirror reflection formula: r = in - 2 * (in · n) * n
  return in - 2.0 * in.dot(n) * n;
}
