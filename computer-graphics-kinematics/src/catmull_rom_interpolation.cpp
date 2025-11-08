#include "catmull_rom_interpolation.h"
#include <Eigen/Dense>
#include <cmath>

Eigen::Vector3d catmull_rom_interpolation(
  const std::vector<std::pair<double, Eigen::Vector3d> > & keyframes,
  double t)
{
  /////////////////////////////////////////////////////////////////////////////
  // Handle trivial cases
  if(keyframes.empty()) { return Eigen::Vector3d(0,0,0); }
  if(keyframes.size() == 1) { return keyframes.front().second; }

  // Periodic wrap over the last key time if positive
  const double t_min = keyframes.front().first;
  const double t_max = keyframes.back().first;
  double tt = t;
  if(t_max > t_min)
  {
    const double period = t_max - t_min;
    // Shift to [t_min, t_max]
    tt = std::fmod(tt - t_min, period);
    if(tt < 0) tt += period;
    tt += t_min;
  }

  // Find segment [i,i+1] such that t_i <= tt <= t_{i+1}
  int i = 0;
  for(int k = 0; k < static_cast<int>(keyframes.size())-1; ++k)
  {
    if(tt >= keyframes[k].first && tt <= keyframes[k+1].first)
    {
      i = k; break;
    }
  }
  // Clamp to last segment if at end
  if(tt >= keyframes.back().first) { i = static_cast<int>(keyframes.size())-2; }

  const auto tk = [&](int idx)->double
  {
    idx = std::max(0,std::min(idx, static_cast<int>(keyframes.size())-1));
    return keyframes[idx].first;
  };
  const auto pk = [&](int idx)->Eigen::Vector3d
  {
    idx = std::max(0,std::min(idx, static_cast<int>(keyframes.size())-1));
    return keyframes[idx].second;
  };

  const double t0 = tk(i-1);
  const double t1 = tk(i);
  const double t2 = tk(i+1);
  const double t3 = tk(i+2);
  const Eigen::Vector3d p0 = pk(i-1);
  const Eigen::Vector3d p1 = pk(i);
  const Eigen::Vector3d p2 = pk(i+1);
  const Eigen::Vector3d p3 = pk(i+2);

  const double dt = std::max(1e-12, t2 - t1);
  const double u = std::max(0.0, std::min(1.0, (tt - t1)/dt));

  // Non-uniform Catmull-Rom tangents
  const double dt10 = std::max(1e-12, t2 - t0);
  const double dt32 = std::max(1e-12, t3 - t1);
  const Eigen::Vector3d m1 = (p2 - p0) / dt10;
  const Eigen::Vector3d m2 = (p3 - p1) / dt32;

  const double u2 = u*u;
  const double u3 = u2*u;
  const double h00 = 2*u3 - 3*u2 + 1;
  const double h10 = u3 - 2*u2 + u;
  const double h01 = -2*u3 + 3*u2;
  const double h11 = u3 - u2;

  const Eigen::Vector3d p = 
    h00*p1 + h10*(t2 - t1)*m1 + h01*p2 + h11*(t2 - t1)*m2;
  return p;
  /////////////////////////////////////////////////////////////////////////////
}
