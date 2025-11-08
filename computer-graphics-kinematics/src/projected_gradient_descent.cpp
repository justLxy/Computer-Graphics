#include "projected_gradient_descent.h"
#include "line_search.h"

void projected_gradient_descent(
  const std::function<double(const Eigen::VectorXd &)> & f,
  const std::function<Eigen::VectorXd(const Eigen::VectorXd &)> & grad_f,
  const std::function<void(Eigen::VectorXd &)> & proj_z,
  const int max_iters,
  Eigen::VectorXd & z)
{
  /////////////////////////////////////////////////////////////////////////////
  // Initialize by projecting onto feasible set
  proj_z(z);
  double fz = f(z);
  for(int it = 0; it < max_iters; ++it)
  {
    const Eigen::VectorXd g = grad_f(z);
    const double gnorm = g.norm();
    if(!std::isfinite(gnorm) || gnorm < 1e-12) { break; }
    const Eigen::VectorXd dz = -g;
    const double step = line_search(f, proj_z, z, dz, 1e4);
    if(step <= 0.0) { break; }
    Eigen::VectorXd znew = z + step * dz;
    proj_z(znew);
    const double fnew = f(znew);
    if(fnew >= fz) { break; }
    z = znew;
    fz = fnew;
  }
  /////////////////////////////////////////////////////////////////////////////
}
