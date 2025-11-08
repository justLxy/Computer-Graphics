#include "catmull_clark.h"
#include <unordered_map>
#include <utility>
#include <functional>
#include <set>
#include <algorithm>

void catmull_clark(
  const Eigen::MatrixXd & V,
  const Eigen::MatrixXi & F,
  const int num_iters,
  Eigen::MatrixXd & SV,
  Eigen::MatrixXi & SF)
{
  SV = V;
  SF = F;
  
  for (int iter = 0; iter < num_iters; iter++) {
    int n_verts = SV.rows();
    int n_faces = SF.rows();
    
    // Build edge-to-faces map
    std::unordered_map<long long, std::vector<int>> edge_to_faces;
    auto edge_key = [](int v1, int v2) -> long long {
      int min_v = std::min(v1, v2);
      int max_v = std::max(v1, v2);
      return ((long long)min_v << 32) | (long long)max_v;
    };
    
    for (int f = 0; f < n_faces; f++) {
      for (int i = 0; i < 4; i++) {
        int v1 = SF(f, i);
        int v2 = SF(f, (i + 1) % 4);
        edge_to_faces[edge_key(v1, v2)].push_back(f);
      }
    }
    
    // Identify boundary edges and vertices
    std::set<long long> boundary_edges;
    std::set<int> boundary_vertices;
    for (const auto& entry : edge_to_faces) {
      if (entry.second.size() == 1) {
        boundary_edges.insert(entry.first);
        boundary_vertices.insert((int)(entry.first >> 32));
        boundary_vertices.insert((int)(entry.first & 0xFFFFFFFF));
      }
    }
    
    // Step 1: Face points (average of face vertices)
    Eigen::MatrixXd face_points(n_faces, 3);
    for (int f = 0; f < n_faces; f++) {
      Eigen::RowVector3d avg = Eigen::RowVector3d::Zero();
      for (int i = 0; i < 4; i++) {
        avg += SV.row(SF(f, i));
      }
      face_points.row(f) = avg / 4.0;
    }
    
    // Step 2: Edge points
    std::unordered_map<long long, Eigen::RowVector3d> edge_points;
    for (const auto& entry : edge_to_faces) {
      long long edge = entry.first;
      const std::vector<int>& faces = entry.second;
      
      int v1 = (int)(edge >> 32);
      int v2 = (int)(edge & 0xFFFFFFFF);
      
      Eigen::RowVector3d ep;
      if (boundary_edges.count(edge)) {
        ep = (SV.row(v1) + SV.row(v2)) / 2.0;
      } else {
        ep = SV.row(v1) + SV.row(v2);
        for (int face_idx : faces) {
          ep += face_points.row(face_idx);
        }
        ep /= (2.0 + faces.size());
      }
      
      edge_points[edge] = ep;
    }
    
    // Step 3: Update vertex positions
    // Formula: (F + 2R + (n-3)P) / n
    std::vector<std::vector<int>> vertex_faces(n_verts);
    for (int f = 0; f < n_faces; f++) {
      for (int i = 0; i < 4; i++) {
        vertex_faces[SF(f, i)].push_back(f);
      }
    }
    
    std::unordered_map<int, std::vector<int>> boundary_adjacency;
    for(long long edge : boundary_edges) {
      int v1 = (int)(edge >> 32);
      int v2 = (int)(edge & 0xFFFFFFFF);
      boundary_adjacency[v1].push_back(v2);
      boundary_adjacency[v2].push_back(v1);
    }

    Eigen::MatrixXd new_vertex_points(n_verts, 3);
    for (int v = 0; v < n_verts; v++) {
      if (boundary_vertices.count(v)) {
        if (boundary_adjacency.count(v) && boundary_adjacency[v].size() == 2) {
          int v_prev = boundary_adjacency[v][0];
          int v_next = boundary_adjacency[v][1];
          new_vertex_points.row(v) = (SV.row(v_prev) + 6.0 * SV.row(v) + SV.row(v_next)) / 8.0;
        } else {
          new_vertex_points.row(v) = SV.row(v);
        }
      } else {
        const std::vector<int>& incident_faces = vertex_faces[v];
        int n = incident_faces.size();
        
        // F: average of incident face points
        Eigen::RowVector3d F = Eigen::RowVector3d::Zero();
        for (int f : incident_faces) {
          F += face_points.row(f);
        }
        F /= n;
        
        // R: average of incident edge midpoints
        std::set<long long> incident_edges;
        for (int f : incident_faces) {
          for (int i = 0; i < 4; i++) {
            if (SF(f, i) == v) {
              int prev = SF(f, (i + 3) % 4);
              int next = SF(f, (i + 1) % 4);
              incident_edges.insert(edge_key(v, prev));
              incident_edges.insert(edge_key(v, next));
            }
          }
        }
        
        Eigen::RowVector3d R = Eigen::RowVector3d::Zero();
        for (long long edge : incident_edges) {
          int v1_edge = (int)(edge >> 32);
          int v2_edge = (int)(edge & 0xFFFFFFFF);
          R += (SV.row(v1_edge) + SV.row(v2_edge)) / 2.0;
        }
        R /= incident_edges.size();
        
        new_vertex_points.row(v) = (F + 2.0 * R + (n - 3.0) * SV.row(v)) / n;
      }
    }
    
    // Step 4-5: Build new mesh topology
    int new_n_verts = n_verts + n_faces + edge_points.size();
    int new_n_faces = n_faces * 4;
    
    Eigen::MatrixXd new_V(new_n_verts, 3);
    Eigen::MatrixXi new_F(new_n_faces, 4);
    
    for (int v = 0; v < n_verts; v++) {
      new_V.row(v) = new_vertex_points.row(v);
    }
    
    for (int f = 0; f < n_faces; f++) {
      new_V.row(n_verts + f) = face_points.row(f);
    }
    
    std::unordered_map<long long, int> edge_to_index;
    int edge_idx = 0;
    for (const auto& entry : edge_points) {
      new_V.row(n_verts + n_faces + edge_idx) = entry.second;
      edge_to_index[entry.first] = n_verts + n_faces + edge_idx;
      edge_idx++;
    }
    
    // Each old quad -> 4 new quads
    int new_face_idx = 0;
    for (int f = 0; f < n_faces; f++) {
      int face_point_idx = n_verts + f;
      
      for (int i = 0; i < 4; i++) {
        int v1 = SF(f, i);
        int v2 = SF(f, (i + 1) % 4);
        int edge1_idx_key_v1 = SF(f, (i+3)%4);
        int edge1_idx = edge_to_index[edge_key(edge1_idx_key_v1, v1)];
        int edge2_idx = edge_to_index[edge_key(v1, v2)];
        
        new_F(new_face_idx, 0) = v1;
        new_F(new_face_idx, 1) = edge2_idx;
        new_F(new_face_idx, 2) = face_point_idx;
        new_F(new_face_idx, 3) = edge1_idx;
        
        new_face_idx++;
      }
    }
    
    SV = new_V;
    SF = new_F;
  }
}
