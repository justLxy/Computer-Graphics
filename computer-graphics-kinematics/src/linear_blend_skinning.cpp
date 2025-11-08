#include "linear_blend_skinning.h"

void linear_blend_skinning(
  const Eigen::MatrixXd & V,
  const Skeleton & skeleton,
  const std::vector<Eigen::Affine3d,Eigen::aligned_allocator<Eigen::Affine3d> > & T,
  const Eigen::MatrixXd & W,
  Eigen::MatrixXd & U)
{
  /////////////////////////////////////////////////////////////////////////////
  // Deform vertices using weighted sum of bone transforms.
  U.resize(V.rows(), 3);
  U.setZero();
  for(int v = 0; v < V.rows(); ++v)
  {
    Eigen::Vector4d v_rest;
    v_rest << V.row(v).transpose(), 1.0;
    Eigen::Vector3d accum = Eigen::Vector3d::Zero();
    double wsum = 0.0;
    for(int i = 0; i < static_cast<int>(skeleton.size()); ++i)
    {
      const int wi = skeleton[i].weight_index;
      if(wi < 0 || wi >= W.cols()) { continue; }
      const double w = W(v, wi);
      if(w == 0.0) { continue; }
      const Eigen::Vector3d vi = (T[i] * v_rest).head<3>();
      accum += w * vi;
      wsum += w;
    }
    // If weights don't sum to 1, leave as accumulated value (usual input sums to 1)
    U.row(v) = accum.transpose();
  }
  /////////////////////////////////////////////////////////////////////////////
}
