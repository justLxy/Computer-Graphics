#include "signed_incidence_matrix_dense.h"

void signed_incidence_matrix_dense(
  const int n,
  const Eigen::MatrixXi & E,
  Eigen::MatrixXd & A)
{
  A = Eigen::MatrixXd::Zero(E.rows(), n);
  const int num_edges = static_cast<int>(E.rows());
  for(int e = 0; e < num_edges; ++e)
  {
    const int vi = E(e,0);
    const int vj = E(e,1);
    A(e,vi) =  1.0;
    A(e,vj) = -1.0;
  }
}
