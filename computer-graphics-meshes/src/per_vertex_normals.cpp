#include "per_vertex_normals.h"
#include "triangle_area_normal.h"

void per_vertex_normals(
  const Eigen::MatrixXd & V,
  const Eigen::MatrixXi & F,
  Eigen::MatrixXd & N)
{
  N = Eigen::MatrixXd::Zero(V.rows(),3);
  
  // Accumulate area-weighted normals
  for (int f = 0; f < F.rows(); f++) {
    Eigen::RowVector3d a = V.row(F(f, 0));
    Eigen::RowVector3d b = V.row(F(f, 1));
    Eigen::RowVector3d c = V.row(F(f, 2));
    
    Eigen::RowVector3d area_normal = triangle_area_normal(a, b, c);
    
    N.row(F(f, 0)) += area_normal;
    N.row(F(f, 1)) += area_normal;
    N.row(F(f, 2)) += area_normal;
  }
  
  // Normalize
  for (int i = 0; i < N.rows(); i++) {
    if (N.row(i).squaredNorm() > 1e-12) {
      N.row(i).normalize();
    }
  }
}
