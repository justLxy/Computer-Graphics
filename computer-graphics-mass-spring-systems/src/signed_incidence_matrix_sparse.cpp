#include "signed_incidence_matrix_sparse.h"
#include <vector>

void signed_incidence_matrix_sparse(
  const int n,
  const Eigen::MatrixXi & E,
  Eigen::SparseMatrix<double>  & A)
{
  const int num_edges = static_cast<int>(E.rows());
  std::vector<Eigen::Triplet<double> > ijv;
  ijv.reserve(static_cast<size_t>(2 * std::max(0, num_edges)));
  for(int e = 0; e < num_edges; ++e)
  {
    const int vi = E(e,0);
    const int vj = E(e,1);
    ijv.emplace_back(e, vi,  1.0);
    ijv.emplace_back(e, vj, -1.0);
  }
  A.resize(num_edges, n);
  A.setFromTriplets(ijv.begin(), ijv.end());
}
