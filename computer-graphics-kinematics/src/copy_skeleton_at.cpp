#include "copy_skeleton_at.h"
Skeleton copy_skeleton_at(
  const Skeleton & skeleton, 
  const Eigen::VectorXd & A)
{
  /////////////////////////////////////////////////////////////////////////////
  Skeleton copy = skeleton;
  assert(A.size() == static_cast<int>(skeleton.size())*3);
  for(int i = 0; i < static_cast<int>(skeleton.size()); ++i)
  {
    copy[i].xzx = A.segment<3>(i*3);
  }
  return copy;
  /////////////////////////////////////////////////////////////////////////////
}
