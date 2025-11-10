#include "forward_kinematics.h"
#include "euler_angles_to_transform.h"
#include <functional> // std::function

void forward_kinematics(
  const Skeleton & skeleton,
  std::vector<Eigen::Affine3d,Eigen::aligned_allocator<Eigen::Affine3d> > & T)
{
  /////////////////////////////////////////////////////////////////////////////
  // Compute global rest-to-pose transforms for each bone.
  const int n = static_cast<int>(skeleton.size());
  T.resize(n);
  for(int i = 0; i < n; ++i)
  {
    const int parent = skeleton[i].parent_index;
    const Eigen::Affine3d R_rel = euler_angles_to_transform(skeleton[i].xzx);
    const Eigen::Affine3d & rTi = skeleton[i].rest_T;
    const Eigen::Affine3d  rTi_inv = rTi.inverse();
    if(parent < 0)
    {
      // Root: no parent transform, but still apply own relative rotation
      T[i] = rTi * R_rel * rTi_inv;
    }else
    {
      // Apply parent's pose, then change of coordinates to canonical frame,
      // apply relative rotation, then map back to rest frame.
      T[i] = T[parent] * rTi * R_rel * rTi_inv;
    }
  }
  /////////////////////////////////////////////////////////////////////////////
}
