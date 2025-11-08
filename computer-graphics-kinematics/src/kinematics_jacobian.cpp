#include "kinematics_jacobian.h"
#include "transformed_tips.h"
#include <iostream>

void kinematics_jacobian(
  const Skeleton & skeleton,
  const Eigen::VectorXi & b,
  Eigen::MatrixXd & J)
{
  /////////////////////////////////////////////////////////////////////////////
  // Finite differencing of transformed tips wrt Euler angles.
  const int nb = static_cast<int>(b.size());
  const int ns = static_cast<int>(skeleton.size());
  J.setZero(nb*3, ns*3);

  // Base positions
  const Eigen::VectorXd x0 = transformed_tips(skeleton, b);

  const double h = 1e-7;
  for(int j = 0; j < ns; ++j)
  {
    for(int a = 0; a < 3; ++a)
    {
      Skeleton sP = skeleton;
      sP[j].xzx[a] += h;
      const Eigen::VectorXd xP = transformed_tips(sP, b);
      const Eigen::VectorXd dx = (xP - x0) / h;
      J.col(j*3 + a) = dx;
    }
  }
  /////////////////////////////////////////////////////////////////////////////
}
