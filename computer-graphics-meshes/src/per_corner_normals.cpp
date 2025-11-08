#include "per_corner_normals.h"
#include "triangle_area_normal.h"
// Hint:
#include "vertex_triangle_adjacency.h"
#include <iostream>
#include <cmath>

void per_corner_normals(
  const Eigen::MatrixXd & V,
  const Eigen::MatrixXi & F,
  const double corner_threshold,
  Eigen::MatrixXd & N)
{
  N = Eigen::MatrixXd::Zero(F.rows()*3,3);
  
  std::vector<std::vector<int>> VF;
  vertex_triangle_adjacency(F, V.rows(), VF);
  
  // Compute per-face normals
  Eigen::MatrixXd face_normals(F.rows(), 3);
  for (int f = 0; f < F.rows(); f++) {
    Eigen::RowVector3d a = V.row(F(f, 0));
    Eigen::RowVector3d b = V.row(F(f, 1));
    Eigen::RowVector3d c = V.row(F(f, 2));
    face_normals.row(f) = triangle_area_normal(a, b, c).normalized();
  }
  
  double threshold_cos = cos(corner_threshold * M_PI / 180.0);
  
  // For each corner
  for (int f = 0; f < F.rows(); f++) {
    for (int c = 0; c < 3; c++) {
      int vertex_idx = F(f, c);
      Eigen::RowVector3d corner_normal = Eigen::RowVector3d::Zero();
      Eigen::RowVector3d face_normal = face_normals.row(f);
      
      // Sum area-weighted normals from similar adjacent faces
      for (int adj_face_idx : VF[vertex_idx]) {
        Eigen::RowVector3d adj_face_normal = face_normals.row(adj_face_idx);
        
        if (face_normal.dot(adj_face_normal) >= threshold_cos) {
          Eigen::RowVector3d a = V.row(F(adj_face_idx, 0));
          Eigen::RowVector3d b = V.row(F(adj_face_idx, 1));
          Eigen::RowVector3d c = V.row(F(adj_face_idx, 2));
          corner_normal += triangle_area_normal(a, b, c);
        }
      }
      
      N.row(f * 3 + c) = corner_normal.normalized();
    }
  }
}
