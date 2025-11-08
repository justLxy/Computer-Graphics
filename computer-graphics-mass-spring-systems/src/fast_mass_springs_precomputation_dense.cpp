#include "fast_mass_springs_precomputation_dense.h"
#include "signed_incidence_matrix_dense.h"
#include <Eigen/Dense>

bool fast_mass_springs_precomputation_dense(
  const Eigen::MatrixXd & V,
  const Eigen::MatrixXi & E,
  const double k,
  const Eigen::VectorXd & m,
  const Eigen::VectorXi & b,
  const double delta_t,
  Eigen::VectorXd & r,
  Eigen::MatrixXd & M,
  Eigen::MatrixXd & A,
  Eigen::MatrixXd & C,
  Eigen::LLT<Eigen::MatrixXd> & prefactorization)
{
  const int num_vertices = static_cast<int>(V.rows());
  const int num_edges    = static_cast<int>(E.rows());
  const int num_pins     = static_cast<int>(b.size());

  // Edge rest lengths
  r.resize(num_edges);
  for(int e = 0; e < num_edges; ++e)
  {
    const int i = E(e,0);
    const int j = E(e,1);
    r(e) = (V.row(i) - V.row(j)).norm();
  }

  // Mass matrix
  M = m.asDiagonal();

  // Signed incidence matrix
  signed_incidence_matrix_dense(num_vertices, E, A);

  // Selection matrix for pinned vertices
  C = Eigen::MatrixXd::Zero(num_pins, num_vertices);
  for(int pi = 0; pi < num_pins; ++pi)
  {
    const int vi = b(pi);
    C(pi, vi) = 1.0;
  }

  // Quadratic matrix Q = k A^T A + (1/dt^2) M + w C^T C
  const double inv_dt2 = 1.0/(delta_t*delta_t);
  const double w = 1e10;
  Eigen::MatrixXd Q =
    k * (A.transpose() * A) +
    inv_dt2 * M +
    w * (C.transpose() * C);

  prefactorization.compute(Q);
  return prefactorization.info() != Eigen::NumericalIssue;
}
