#include "sphere.h"
#include <iostream>
#include <cmath>

void sphere(
  const int num_faces_u,
  const int num_faces_v,
  Eigen::MatrixXd & V,
  Eigen::MatrixXi & F,
  Eigen::MatrixXd & UV,
  Eigen::MatrixXi & UF,
  Eigen::MatrixXd & NV,
  Eigen::MatrixXi & NF)
{
  int num_verts_u = num_faces_u + 1;
  int num_verts_v = num_faces_v + 1;
  int num_vertices = num_verts_u * num_verts_v;
  int num_faces = num_faces_u * num_faces_v;
  
  V.resize(num_vertices, 3);
  UV.resize(num_vertices, 2);
  NV.resize(num_vertices, 3);
  F.resize(num_faces, 4);
  UF.resize(num_faces, 4);
  NF.resize(num_faces, 4);
  
  // Generate vertices using spherical coordinates
  for (int j = 0; j < num_verts_v; j++) {
    double theta = M_PI * j / num_faces_v;  // Latitude: 0 (north) to pi (south)
    
    for (int i = 0; i < num_verts_u; i++) {
      double phi = 2.0 * M_PI * i / num_faces_u;  // Longitude: 0 to 2*pi
      
      int vertex_idx = j * num_verts_u + i;
      
      // Spherical to Cartesian
      V(vertex_idx, 0) = sin(theta) * cos(phi);
      V(vertex_idx, 1) = cos(theta);
      V(vertex_idx, 2) = sin(theta) * sin(phi);
      
      // UV mapping
      UV(vertex_idx, 0) = (double)i / num_faces_u;
      UV(vertex_idx, 1) = 1.0 - (double)j / num_faces_v;
      
      // Unit sphere normals equal positions
      NV.row(vertex_idx) = V.row(vertex_idx);
    }
  }
  
  // Generate quad faces
  int face_idx = 0;
  for (int j = 0; j < num_faces_v; j++) {
    for (int i = 0; i < num_faces_u; i++) {
      int v0 = j * num_verts_u + i;
      int v1 = j * num_verts_u + (i + 1) % num_verts_u;
      int v2 = (j + 1) * num_verts_u + (i + 1) % num_verts_u;
      int v3 = (j + 1) * num_verts_u + i;
      
      F(face_idx, 0) = v0;
      F(face_idx, 1) = v1;
      F(face_idx, 2) = v2;
      F(face_idx, 3) = v3;
      
      UF(face_idx, 0) = v0;
      UF(face_idx, 1) = v1;
      UF(face_idx, 2) = v2;
      UF(face_idx, 3) = v3;
      
      NF(face_idx, 0) = v0;
      NF(face_idx, 1) = v1;
      NF(face_idx, 2) = v2;
      NF(face_idx, 3) = v3;
      
      face_idx++;
    }
  }
}
