#include "point_AABBTree_squared_distance.h"
#include "point_box_squared_distance.h"
#include "AABBTree.h"
#include "CloudPoint.h"
#include <queue> // std::priority_queue
#include <limits>

bool point_AABBTree_squared_distance(
    const Eigen::RowVector3d & query,
    const std::shared_ptr<AABBTree> & root,
    const double min_sqrd,
    const double max_sqrd,
    double & sqrd,
    std::shared_ptr<Object> & descendant)
{
  ////////////////////////////////////////////////////////////////////////////
  // Priority queue
  using QueueElement = std::pair<double, std::shared_ptr<Object>>;
  
  // Min-heap: smaller distances have higher priority
  std::priority_queue<QueueElement, std::vector<QueueElement>, std::greater<QueueElement>> queue;
  
  double d_root = point_box_squared_distance(query, root->box);
  queue.push({d_root, root});
  
  sqrd = std::numeric_limits<double>::infinity();
  bool found = false;
  
  while (!queue.empty()) {
    auto [d_s, subtree] = queue.top();
    queue.pop();
    
    if (d_s >= sqrd) {
      continue;
    }
    
    std::shared_ptr<AABBTree> aabb = std::dynamic_pointer_cast<AABBTree>(subtree);
    
    if (aabb) {
      // Internal node
      if (aabb->left) {
        double d_left = point_box_squared_distance(query, aabb->left->box);
        queue.push({d_left, aabb->left});
      }
      if (aabb->right) {
        double d_right = point_box_squared_distance(query, aabb->right->box);
        queue.push({d_right, aabb->right});
      }
    } else {
      // Leaf node
      double temp_sqrd;
      std::shared_ptr<Object> temp_desc;
      if (subtree->point_squared_distance(query, min_sqrd, sqrd, temp_sqrd, temp_desc)) {
        if (temp_sqrd < sqrd) {
          sqrd = temp_sqrd;
          descendant = temp_desc ? temp_desc : subtree;
          found = true;
        }
      }
    }
  }
  
  return found && sqrd >= min_sqrd && sqrd < max_sqrd;
  ////////////////////////////////////////////////////////////////////////////
}
