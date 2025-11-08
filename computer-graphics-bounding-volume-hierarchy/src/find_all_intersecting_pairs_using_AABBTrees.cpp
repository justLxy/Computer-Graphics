#include "find_all_intersecting_pairs_using_AABBTrees.h"
#include "box_box_intersect.h"
#include "AABBTree.h"
// Hint: use a list as a queue
#include <list> 

void find_all_intersecting_pairs_using_AABBTrees(
  const std::shared_ptr<AABBTree> & rootA,
  const std::shared_ptr<AABBTree> & rootB,
  std::vector<std::pair<std::shared_ptr<Object>,std::shared_ptr<Object> > > & 
    leaf_pairs)
{
  ////////////////////////////////////////////////////////////////////////////
  using NodePair = std::pair<std::shared_ptr<Object>, std::shared_ptr<Object>>;
  std::list<NodePair> queue;
  
  // Pruning: If root boxes do not intersect, none of their child nodes can intersect.
  if (!box_box_intersect(rootA->box, rootB->box)) {
    return;
  }
  
  queue.push_back({rootA, rootB});
  
  while (!queue.empty()) {
    auto [nodeA, nodeB] = queue.front();
    queue.pop_front();
    
    std::shared_ptr<AABBTree> aabbA = std::dynamic_pointer_cast<AABBTree>(nodeA);
    std::shared_ptr<AABBTree> aabbB = std::dynamic_pointer_cast<AABBTree>(nodeB);
    
    bool isLeafA = !aabbA;
    bool isLeafB = !aabbB;
    
    if (isLeafA && isLeafB) {
      // Both are leaves
      leaf_pairs.push_back({nodeA, nodeB});
    } else if (isLeafA) {
      // Only A is a leaf
      if (aabbB->left && box_box_intersect(nodeA->box, aabbB->left->box)) {
        queue.push_back({nodeA, aabbB->left});
      }
      if (aabbB->right && box_box_intersect(nodeA->box, aabbB->right->box)) {
        queue.push_back({nodeA, aabbB->right});
      }
    } else if (isLeafB) {
      // Only B is a leaf
      if (aabbA->left && box_box_intersect(aabbA->left->box, nodeB->box)) {
        queue.push_back({aabbA->left, nodeB});
      }
      if (aabbA->right && box_box_intersect(aabbA->right->box, nodeB->box)) {
        queue.push_back({aabbA->right, nodeB});
      }
    } else {
      // Both are internal nodes
      if (aabbA->left && aabbB->left && 
          box_box_intersect(aabbA->left->box, aabbB->left->box)) {
        queue.push_back({aabbA->left, aabbB->left});
      }
      if (aabbA->left && aabbB->right && 
          box_box_intersect(aabbA->left->box, aabbB->right->box)) {
        queue.push_back({aabbA->left, aabbB->right});
      }
      if (aabbA->right && aabbB->left && 
          box_box_intersect(aabbA->right->box, aabbB->left->box)) {
        queue.push_back({aabbA->right, aabbB->left});
      }
      if (aabbA->right && aabbB->right && 
          box_box_intersect(aabbA->right->box, aabbB->right->box)) {
        queue.push_back({aabbA->right, aabbB->right});
      }
    }
  }
  ////////////////////////////////////////////////////////////////////////////
}
