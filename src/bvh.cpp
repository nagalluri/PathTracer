#include "bvh.h"

#include "CGL/CGL.h"
#include "static_scene/triangle.h"

#include <iostream>
#include <stack>

using namespace std;

namespace CGL { namespace StaticScene {

BVHAccel::BVHAccel(const std::vector<Primitive *> &_primitives,
                   size_t max_leaf_size) {

  root = construct_bvh(_primitives, max_leaf_size);

}

BVHAccel::~BVHAccel() {
  if (root) delete root;
}

BBox BVHAccel::get_bbox() const {
  return root->bb;
}

void BVHAccel::draw(BVHNode *node, const Color& c) const {
  if (node->isLeaf()) {
    for (Primitive *p : *(node->prims))
      p->draw(c);
  } else {
    draw(node->l, c);
    draw(node->r, c);
  }
}

void BVHAccel::drawOutline(BVHNode *node, const Color& c) const {
  if (node->isLeaf()) {
    for (Primitive *p : *(node->prims))
      p->drawOutline(c);
  } else {
    drawOutline(node->l, c);
    drawOutline(node->r, c);
  }
}

BVHNode *BVHAccel::construct_bvh(const std::vector<Primitive*>& prims, size_t max_leaf_size) {
  
  // TODO Part 2, task 1:
  // Construct a BVH from the given vector of primitives and maximum leaf
  // size configuration. The starter code build a BVH aggregate with a
  // single leaf node (which is also the root) that encloses all the
  // primitives.


  BBox centroid_box, bbox;
  int counter = 0;
  for (Primitive *p : prims) {
    BBox bb = p->get_bbox();
    bbox.expand(bb);
    Vector3D c = bb.centroid();
    centroid_box.expand(c);
    counter += 1;
  }
  //printf("%d\n", node->prims->size());
  //node->prims->size()
  // You'll want to adjust this code.
  // Right now we just return a single node containing all primitives.
  BVHNode *node = new BVHNode(bbox);
  //printf("%s\n", "here1aaa");
  if (counter <= max_leaf_size) {
    node->prims = new vector<Primitive *>(prims);
    //printf("%s\n", "base");
    return node;
  } else {
    //printf("%s\n", "here1aa");
    double mdpt = 0.0;
    int axis = 0;
    bbox = centroid_box;
    //printf("%s\n", "here1a");
    if (bbox.extent.x >= bbox.extent.y && bbox.extent.x >= bbox.extent.z) {
      axis = 1;
      mdpt = (bbox.min.x + bbox.max.x)/ 2;
      //printf("%s\n", "here2a");
    } else if (bbox.extent.y >= bbox.extent.x && bbox.extent.y >= bbox.extent.z) {
      axis = 2;
      mdpt = (bbox.min.y + bbox.max.y)/ 2;
      //printf("%s\n", "here2b");
    } else {
      axis = 3;
      mdpt = (bbox.min.z + bbox.max.z)/ 2;
      //printf("%s\n", "here2c");
    }
    //printf("%s\n", "here2");
    vector<Primitive *> left;
    vector<Primitive *> right;
    int leftcount = 0;
    int rightcount = 0;
    for (Primitive *p : prims) {
      double center = 0.0;
      Vector3D c_axis = p->get_bbox().centroid();
      if (axis == 1) {
        center = c_axis.x;
      } else if (axis == 2) {
        center = c_axis.y;
      } else if (axis == 3) {
        center = c_axis.z;
      }
      if (center <= mdpt) {
        left.push_back(p);
        leftcount += 1;
      } else {
        right.push_back(p);
        rightcount += 1;
      }
    }
    //printf("%s\n", "here3");
    if (rightcount == 0) {
      node->prims = new vector<Primitive *>(left);
      //node = construct_bvh(left, max_leaf_size);
      return node;
    } else if (leftcount == 0) {
      node->prims = new vector<Primitive *>(right);
      //node = construct_bvh(right, max_leaf_size);
      return node;
    } else {
      node->l = construct_bvh(left, max_leaf_size);
      node->r = construct_bvh(right, max_leaf_size);
      return node;
    }
    
  }
}


// bool BVHAccel::intersect(const Ray& ray, BVHNode *node) const {

//   // TODO Part 2, task 3:
//   // Implement BVH intersection.
//   // Currently, we just naively loop over every primitive.

//   for (Primitive *p : *(root->prims)) {
//     total_isects++;
//     if (p->intersect(ray)) 
//       return true;
//   }
//   return false;

// }

// bool BVHAccel::intersect(const Ray& ray, Intersection* i, BVHNode *node) const {

//   // TODO Part 2, task 3:
//   // Implement BVH intersection.
//   // Currently, we just naively loop over every primitive.

//   bool hit = false;
//   for (Primitive *p : *(root->prims)) {
//     total_isects++;
//     if (p->intersect(ray, i)) 
//       hit = true;
//   }
//   return hit;

// }

bool BVHAccel::intersect(const Ray& ray, BVHNode *node) const {

  // TODO Part 2, task 3:
  // Implement BVH intersection.
  // Currently, we just naively loop over every primitive.

  double minTime = ray.min_t;
  double maxTime = ray.max_t;

  bool result = node->bb.intersect(ray, minTime, maxTime);
  if (result == false) {
    return false;
  } 
  bool hit = false;
  if (node->isLeaf()) {
    for (Primitive *p : *(node->prims)) {
      total_isects++;
      if (p->intersect(ray)) {
        //printf("%s\n", "good");
        return true;
      }
    }
    return false;
  } 

  bool hit1 = intersect(ray, node->l);
  bool hit2 = intersect(ray, node->r);

  if (hit1) {
    return true;
  } else if (hit2) {
    return true;
  } else {
    return false;
  }
}

bool BVHAccel::intersect(const Ray& ray, Intersection* i, BVHNode *node) const {

  // TODO Part 2, task 3:
  // Implement BVH intersection.
  // Currently, we just naively loop over every primitive.

  double minTime = ray.min_t;
  double maxTime = ray.max_t;

  bool result = node->bb.intersect(ray, minTime, maxTime);
  if (result == false) {
    return false;
  } 
  bool hit = false;
  if (node->isLeaf() == true) {
    for (Primitive *p : *(node->prims)) {
      total_isects++;
      if (p->intersect(ray, i)) {
        hit = true;
      }
      //printf("%s\n", "hmph?");
    }
    return hit;
  } 

  bool hit1 = intersect(ray, i, node->l);
  bool hit2 = intersect(ray, i, node->r);
  if (hit1) {
    return true;
  } else if (hit2) {
    return true;
  } else {
    return false;
  }
}

}  // namespace StaticScene
}  // namespace CGL
