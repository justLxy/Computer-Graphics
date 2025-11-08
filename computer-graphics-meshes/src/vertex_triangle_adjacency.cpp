#include "vertex_triangle_adjacency.h"

void vertex_triangle_adjacency(
  const Eigen::MatrixXi & F,
  const int num_vertices,
  std::vector<std::vector<int> > & VF)
{
  VF.clear();
  VF.resize(num_vertices);
  
  for (int f = 0; f < F.rows(); f++) {
    for (int i = 0; i < F.cols(); i++) {
      VF[F(f, i)].push_back(f);
    }
  }
}

