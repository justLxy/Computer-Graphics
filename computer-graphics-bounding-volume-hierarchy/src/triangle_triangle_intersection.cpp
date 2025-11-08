#include "triangle_triangle_intersection.h"
#include <Eigen/Dense>
#include <cmath>

// compute signed distance from point to plane
double signed_distance_to_plane(
  const Eigen::RowVector3d & point,
  const Eigen::RowVector3d & plane_point,
  const Eigen::RowVector3d & plane_normal)
{
  return (point - plane_point).dot(plane_normal);
}

// Check if point P is inside 2D triangle ABC
bool point_in_triangle_2d(
  const Eigen::Vector2d & P,
  const Eigen::Vector2d & A,
  const Eigen::Vector2d & B,
  const Eigen::Vector2d & C)
{
  Eigen::Vector2d v0 = C - A;
  Eigen::Vector2d v1 = B - A;
  Eigen::Vector2d v2 = P - A;
  
  double dot00 = v0.dot(v0);
  double dot01 = v0.dot(v1);
  double dot02 = v0.dot(v2);
  double dot11 = v1.dot(v1);
  double dot12 = v1.dot(v2);
  
  double denom = dot00 * dot11 - dot01 * dot01;
  if (std::abs(denom) < 1e-10) return false;
  
  double u = (dot11 * dot02 - dot01 * dot12) / denom;
  double v = (dot00 * dot12 - dot01 * dot02) / denom;
  
  return (u >= -1e-10) && (v >= -1e-10) && (u + v <= 1.0 + 1e-10);
}

// Check if 2D line segment intersects 2D line segment
bool segment_segment_2d(
  const Eigen::Vector2d & p1, const Eigen::Vector2d & p2,
  const Eigen::Vector2d & q1, const Eigen::Vector2d & q2)
{
  Eigen::Vector2d r = p2 - p1;
  Eigen::Vector2d s = q2 - q1;
  double rxs = r.x() * s.y() - r.y() * s.x();
  Eigen::Vector2d qp = q1 - p1;
  double qpxr = qp.x() * r.y() - qp.y() * r.x();
  
  if (std::abs(rxs) < 1e-10) {
    // Parallel or collinear
    if (std::abs(qpxr) < 1e-10) {
      // Collinear
      double t0 = qp.dot(r) / r.dot(r);
      double t1 = t0 + s.dot(r) / r.dot(r);
      if (t0 > t1) std::swap(t0, t1);
      return !(t1 < -1e-10 || t0 > 1.0 + 1e-10);
    }
    return false;
  }
  
  double t = (qp.x() * s.y() - qp.y() * s.x()) / rxs;
  double u = qpxr / rxs;
  
  return (t >= -1e-10 && t <= 1.0 + 1e-10) && (u >= -1e-10 && u <= 1.0 + 1e-10);
}

// Test coplanar triangles in 2D
bool coplanar_triangle_triangle_2d(
  const Eigen::RowVector3d & A0,
  const Eigen::RowVector3d & A1,
  const Eigen::RowVector3d & A2,
  const Eigen::RowVector3d & B0,
  const Eigen::RowVector3d & B1,
  const Eigen::RowVector3d & B2,
  const Eigen::RowVector3d & normal)
{
  // Find the dominant axis of the normal to project onto 2D
  int axis = 0;
  double max_abs = std::abs(normal(0));
  if (std::abs(normal(1)) > max_abs) { axis = 1; max_abs = std::abs(normal(1)); }
  if (std::abs(normal(2)) > max_abs) { axis = 2; }
  
  // Project to 2D by dropping the dominant axis
  auto project = [axis](const Eigen::RowVector3d & p) -> Eigen::Vector2d {
    if (axis == 0) return Eigen::Vector2d(p(1), p(2));
    if (axis == 1) return Eigen::Vector2d(p(0), p(2));
    return Eigen::Vector2d(p(0), p(1));
  };
  
  Eigen::Vector2d a0 = project(A0), a1 = project(A1), a2 = project(A2);
  Eigen::Vector2d b0 = project(B0), b1 = project(B1), b2 = project(B2);
  
  // Check if any vertex of A is inside B
  if (point_in_triangle_2d(a0, b0, b1, b2)) return true;
  if (point_in_triangle_2d(a1, b0, b1, b2)) return true;
  if (point_in_triangle_2d(a2, b0, b1, b2)) return true;
  
  // Check if any vertex of B is inside A
  if (point_in_triangle_2d(b0, a0, a1, a2)) return true;
  if (point_in_triangle_2d(b1, a0, a1, a2)) return true;
  if (point_in_triangle_2d(b2, a0, a1, a2)) return true;
  
  // Check edge-edge intersections
  if (segment_segment_2d(a0, a1, b0, b1)) return true;
  if (segment_segment_2d(a0, a1, b1, b2)) return true;
  if (segment_segment_2d(a0, a1, b2, b0)) return true;
  
  if (segment_segment_2d(a1, a2, b0, b1)) return true;
  if (segment_segment_2d(a1, a2, b1, b2)) return true;
  if (segment_segment_2d(a1, a2, b2, b0)) return true;
  
  if (segment_segment_2d(a2, a0, b0, b1)) return true;
  if (segment_segment_2d(a2, a0, b1, b2)) return true;
  if (segment_segment_2d(a2, a0, b2, b0)) return true;
  
  return false;
}

// check if edge intersects with triangle
bool edge_triangle_intersection(
  const Eigen::RowVector3d & p1,
  const Eigen::RowVector3d & p2,
  const Eigen::RowVector3d & q0,
  const Eigen::RowVector3d & q1,
  const Eigen::RowVector3d & q2)
{
  Eigen::RowVector3d edge = p2 - p1;
  Eigen::RowVector3d normal = (q1 - q0).cross(q2 - q0);
  
  double denom = edge.dot(normal);
  if (std::abs(denom) < 1e-10) return false;
  
  double t = (q0 - p1).dot(normal) / denom;
  if (t < 0 || t > 1) return false;
  
  Eigen::RowVector3d intersection = p1 + t * edge;
  
  // check if intersection point is inside triangle
  Eigen::RowVector3d v0 = q1 - q0;
  Eigen::RowVector3d v1 = q2 - q0;
  Eigen::RowVector3d v2 = intersection - q0;
  
  double d00 = v0.dot(v0);
  double d01 = v0.dot(v1);
  double d11 = v1.dot(v1);
  double d20 = v2.dot(v0);
  double d21 = v2.dot(v1);
  
  double denom2 = d00 * d11 - d01 * d01;
  if (std::abs(denom2) < 1e-10) return false;
  
  double u = (d11 * d20 - d01 * d21) / denom2;
  double v = (d00 * d21 - d01 * d20) / denom2;
  
  return (u >= -1e-10) && (v >= -1e-10) && (u + v <= 1.0 + 2e-10);
}

bool triangle_triangle_intersection(
  const Eigen::RowVector3d & A0,
  const Eigen::RowVector3d & A1,
  const Eigen::RowVector3d & A2,
  const Eigen::RowVector3d & B0,
  const Eigen::RowVector3d & B1,
  const Eigen::RowVector3d & B2)
{
  ////////////////////////////////////////////////////////////////////////////
  // Check if triangles intersect
  
  // Compute normals
  Eigen::RowVector3d normalA = (A1 - A0).cross(A2 - A0);
  Eigen::RowVector3d normalB = (B1 - B0).cross(B2 - B0);
  
  double normA = normalA.norm();
  double normB = normalB.norm();
  
  if (normA < 1e-10 || normB < 1e-10) return false;
  
  normalA /= normA;
  normalB /= normB;
  
  // Check if all vertices of B are on the same side of plane A
  double d0 = signed_distance_to_plane(B0, A0, normalA);
  double d1 = signed_distance_to_plane(B1, A0, normalA);
  double d2 = signed_distance_to_plane(B2, A0, normalA);
  
  if ((d0 > 1e-10 && d1 > 1e-10 && d2 > 1e-10) ||
      (d0 < -1e-10 && d1 < -1e-10 && d2 < -1e-10)) {
    return false;
  }
  
  // Check if all vertices of A are on the same side of plane B
  double e0 = signed_distance_to_plane(A0, B0, normalB);
  double e1 = signed_distance_to_plane(A1, B0, normalB);
  double e2 = signed_distance_to_plane(A2, B0, normalB);
  
  if ((e0 > 1e-10 && e1 > 1e-10 && e2 > 1e-10) ||
      (e0 < -1e-10 && e1 < -1e-10 && e2 < -1e-10)) {
    return false;
  }
  
  // Check if triangles are coplanar
  bool coplanar = (std::abs(d0) < 1e-10 && std::abs(d1) < 1e-10 && std::abs(d2) < 1e-10) ||
                  (std::abs(e0) < 1e-10 && std::abs(e1) < 1e-10 && std::abs(e2) < 1e-10);
  
  if (coplanar) {
    // Use 2D coplanar test
    return coplanar_triangle_triangle_2d(A0, A1, A2, B0, B1, B2, normalA);
  }
  
  // Non-coplanar case: check edge-triangle intersections
  if (edge_triangle_intersection(A0, A1, B0, B1, B2)) return true;
  if (edge_triangle_intersection(A1, A2, B0, B1, B2)) return true;
  if (edge_triangle_intersection(A2, A0, B0, B1, B2)) return true;
  
  if (edge_triangle_intersection(B0, B1, A0, A1, A2)) return true;
  if (edge_triangle_intersection(B1, B2, A0, A1, A2)) return true;
  if (edge_triangle_intersection(B2, B0, A0, A1, A2)) return true;
  
  return false;
  ////////////////////////////////////////////////////////////////////////////
}
