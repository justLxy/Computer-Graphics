#include "transformed_tips.h"
#include "forward_kinematics.h"

Eigen::VectorXd transformed_tips(
  const Skeleton & skeleton, 
  const Eigen::VectorXi & b)
{
  /////////////////////////////////////////////////////////////////////////////
  // Compute FK once, then transform each requested tip.
  std::vector<Eigen::Affine3d,Eigen::aligned_allocator<Eigen::Affine3d> > T;
  forward_kinematics(skeleton,T);
  Eigen::VectorXd X(3*b.size());
  for(int i = 0; i < b.size(); ++i)
  {
    const int bi = b[i];
    const Eigen::Affine3d & rTb = skeleton[bi].rest_T;
    const double len = skeleton[bi].length;
    const Eigen::Vector3d tip_rest = rTb * Eigen::Vector3d(len,0,0);
    const Eigen::Vector3d tip_pose = T[bi] * tip_rest;
    X.segment<3>(i*3) = tip_pose;
  }
  return X;
  /////////////////////////////////////////////////////////////////////////////
}
