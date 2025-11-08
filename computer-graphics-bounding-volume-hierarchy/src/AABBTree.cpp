#include "AABBTree.h"
#include "insert_box_into_box.h"

AABBTree::AABBTree(
  const std::vector<std::shared_ptr<Object> > & objects,
  int a_depth): 
  depth(std::move(a_depth)), 
  num_leaves(objects.size())
{
  ////////////////////////////////////////////////////////////////////////////
  // Base case: if we have 0 or 1 objects, create a leaf
  if (objects.empty()) {
    return;
  }
  
  if (objects.size() == 1) {
    left = objects[0];
    right = nullptr;
    box = objects[0]->box;
    return;
  }
  
  // Compute bounding box of all objects
  box = objects[0]->box;
  for (size_t i = 1; i < objects.size(); ++i) {
    insert_box_into_box(objects[i]->box, box);
  }
  
  // Find the longest axis
  Eigen::RowVector3d extent = box.max_corner - box.min_corner;
  int longest_axis = 0;
  if (extent(1) > extent(0)) longest_axis = 1;
  if (extent(2) > extent(longest_axis)) longest_axis = 2;
  
  // Calculate midpoint along longest axis
  double midpoint = (box.min_corner(longest_axis) + box.max_corner(longest_axis)) / 2.0;
  
  // Partition objects into left and right based on their center along longest axis
  std::vector<std::shared_ptr<Object>> left_objects;
  std::vector<std::shared_ptr<Object>> right_objects;
  
  for (const auto& obj : objects) {
    double center = (obj->box.min_corner(longest_axis) + obj->box.max_corner(longest_axis)) / 2.0;
    if (center < midpoint) {
      left_objects.push_back(obj);
    } else {
      right_objects.push_back(obj);
    }
  }
  
  // Handle case where all objects end up on one side
  if (left_objects.empty()) {
    left_objects.push_back(right_objects.back());
    right_objects.pop_back();
  } else if (right_objects.empty()) {
    right_objects.push_back(left_objects.back());
    left_objects.pop_back();
  }
  
  // Recursively build left and right subtrees
  if (left_objects.size() == 1) {
    left = left_objects[0];
  } else {
    left = std::make_shared<AABBTree>(left_objects, depth + 1);
  }
  
  if (right_objects.size() == 1) {
    right = right_objects[0];
  } else {
    right = std::make_shared<AABBTree>(right_objects, depth + 1);
  }
  ////////////////////////////////////////////////////////////////////////////
}
