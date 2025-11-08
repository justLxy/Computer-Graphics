#include "fast_mass_springs_precomputation_sparse.h"
#include "signed_incidence_matrix_sparse.h"
#include <vector>

bool fast_mass_springs_precomputation_sparse(
  const Eigen::MatrixXd & V,
  const Eigen::MatrixXi & E,
  const double k,
  const Eigen::VectorXd & m,
  const Eigen::VectorXi & b,
  const double delta_t,
  Eigen::VectorXd & r,
  Eigen::SparseMatrix<double>  & M,
  Eigen::SparseMatrix<double>  & A,
  Eigen::SparseMatrix<double>  & C,
  Eigen::SimplicialLLT<Eigen::SparseMatrix<double> > & prefactorization)
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

  // Mass matrix (diagonal) via triplets
  {
    std::vector<Eigen::Triplet<double>> triplets;
    triplets.reserve(static_cast<size_t>(num_vertices));
    for(int i = 0; i < num_vertices; ++i)
    {
      triplets.emplace_back(i, i, m(i));
    }
    M.resize(num_vertices, num_vertices);
    M.setFromTriplets(triplets.begin(), triplets.end());
  }

  // Signed incidence matrix
  signed_incidence_matrix_sparse(num_vertices, E, A);

  // Selection matrix for pinned vertices
  {
    std::vector<Eigen::Triplet<double>> triplets;
    triplets.reserve(static_cast<size_t>(num_pins));
    for(int pi = 0; pi < num_pins; ++pi)
    {
      triplets.emplace_back(pi, b(pi), 1.0);
    }
    C.resize(num_pins, num_vertices);
    C.setFromTriplets(triplets.begin(), triplets.end());
  }

  // Quadratic matrix Q = k A^T A + (1/dt^2) M + w C^T C
  const double inv_dt2 = 1.0/(delta_t*delta_t);
  const double w = 1e10;
  Eigen::SparseMatrix<double> Q =
    k * (A.transpose() * A) +
    inv_dt2 * M +
    w * (C.transpose() * C);

  prefactorization.compute(Q);
  return prefactorization.info() != Eigen::NumericalIssue;
}
