#include "end_effectors_objective_and_gradient.h"
#include "transformed_tips.h"
#include "kinematics_jacobian.h"
#include "copy_skeleton_at.h"
#include <iostream>

void end_effectors_objective_and_gradient(
  const Skeleton & skeleton,
  const Eigen::VectorXi & b,
  const Eigen::VectorXd & xb0,
  std::function<double(const Eigen::VectorXd &)> & f,
  std::function<Eigen::VectorXd(const Eigen::VectorXd &)> & grad_f,
  std::function<void(Eigen::VectorXd &)> & proj_z)
{
  /////////////////////////////////////////////////////////////////////////////
  f = [&](const Eigen::VectorXd & A)->double
  {
    Skeleton sA = copy_skeleton_at(skeleton, A);
    const Eigen::VectorXd xb = transformed_tips(sA, b);
    const Eigen::VectorXd r = xb - xb0;
    return r.squaredNorm();
  };
  grad_f = [&](const Eigen::VectorXd & A)->Eigen::VectorXd
  {
    Skeleton sA = copy_skeleton_at(skeleton, A);
    Eigen::MatrixXd J;
    kinematics_jacobian(sA, b, J);
    const Eigen::VectorXd xb = transformed_tips(sA, b);
    const Eigen::VectorXd r = xb - xb0;
    // Gradient of sum of squared residuals: 2 * J^T r
    return 2.0 * J.transpose() * r;
  };
  proj_z = [&](Eigen::VectorXd & A)
  {
    assert(skeleton.size()*3 == A.size());
    for(int i = 0; i < static_cast<int>(skeleton.size()); ++i)
    {
      for(int k = 0; k < 3; ++k)
      {
        const double lo = skeleton[i].xzx_min[k];
        const double hi = skeleton[i].xzx_max[k];
        double & v = A(i*3 + k);
        if(v < lo) v = lo;
        if(v > hi) v = hi;
      }
    }
  };
  /////////////////////////////////////////////////////////////////////////////
}
