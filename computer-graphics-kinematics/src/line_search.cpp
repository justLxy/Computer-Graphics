#include "line_search.h"
#include <iostream>

double line_search(
  const std::function<double(const Eigen::VectorXd &)> & f,
  const std::function<void(Eigen::VectorXd &)> & proj_z,
  const Eigen::VectorXd & z,
  const Eigen::VectorXd & dz,
  const double max_step)
{
  /////////////////////////////////////////////////////////////////////////////
  // Backtracking line search with projection.
  Eigen::VectorXd z0 = z;
  proj_z(z0);
  const double f0 = f(z0);

  double step = max_step;
  const double min_step = 1e-12;
  const double decay = 0.5;
  for(int iter = 0; iter < 64 && step > min_step; ++iter)
  {
    Eigen::VectorXd zt = z + step * dz;
    proj_z(zt);
    const double ft = f(zt);
    if(ft < f0) { return step; }
    step *= decay;
  }
  return 0.0;
  /////////////////////////////////////////////////////////////////////////////
}
