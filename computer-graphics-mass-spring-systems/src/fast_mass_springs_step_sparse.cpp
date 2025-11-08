#include "fast_mass_springs_step_sparse.h"
#include <igl/matlab_format.h>

void fast_mass_springs_step_sparse(
  const Eigen::MatrixXd & V,
  const Eigen::MatrixXi & E,
  const double k,
  const Eigen::VectorXi & b,
  const double delta_t,
  const Eigen::MatrixXd & fext,
  const Eigen::VectorXd & r,
  const Eigen::SparseMatrix<double>  & M,
  const Eigen::SparseMatrix<double>  & A,
  const Eigen::SparseMatrix<double>  & C,
  const Eigen::SimplicialLLT<Eigen::SparseMatrix<double> > & prefactorization,
  const Eigen::MatrixXd & Uprev,
  const Eigen::MatrixXd & Ucur,
  Eigen::MatrixXd & Unext)
{
  const int num_edges = static_cast<int>(E.rows());
  const double inv_dt2 = 1.0/(delta_t*delta_t);
  const double w = 1e10;
  const int max_iters = 50;
  const double tol = 1e-6;

  // Initialize iterate with current positions
  Eigen::MatrixXd Uiter = Ucur;

  // Terms independent of the local/global iteration
  const Eigen::MatrixXd y = inv_dt2 * (M * (2.0*Ucur - Uprev)) + fext;
  const Eigen::MatrixXd pinned_term = C.transpose() * (C * V);
  const Eigen::MatrixXd b_const = y + w * pinned_term;

  for(int iter = 0; iter < max_iters; ++iter)
  {
    // Local step: compute d for each edge
    Eigen::MatrixXd spring_vectors = A * Uiter; // #E x 3
    Eigen::VectorXd lengths = spring_vectors.rowwise().norm();
    lengths = lengths.array().max(1e-12);
    Eigen::ArrayXd scales = r.array() / lengths.array();
    Eigen::MatrixXd d = spring_vectors.array().colwise() * scales;

    // Global step: build right-hand side b
    Eigen::MatrixXd bvec = k * (A.transpose() * d) + b_const;

    // Solve for next positions
    Eigen::MatrixXd Unew = prefactorization.solve(bvec);
    const double denom = std::max(1.0, Uiter.norm());
    if((Unew - Uiter).norm() / denom < tol)
    {
      Uiter = Unew;
      break;
    }
    Uiter = Unew;
  }

  Unext = Uiter;
}
