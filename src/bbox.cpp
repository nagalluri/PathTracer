#include "bbox.h"

#include "GL/glew.h"

#include <algorithm>
#include <iostream>

namespace CGL {

bool BBox::intersect(const Ray& r, double& t0, double& t1) const {

  // TODO Part 2, task 2:
  // Implement ray - bounding box intersection test
  // If the ray intersected the bounding box within the range given by
  // t0, t1, update t0 and t1 with the new intersection times.


  // bool intersected = false;

  // float tMin;
  // float tMax;

  // Vector3D o = r.o;
  // Vector3D d = r.d;

  // //calculate intersection points on each set of planes
  // float tMinX = (min.x - o.x) / d.x;
  // float tMaxX = (max.x - o.x) / d.x;
  // if (tMinX > tMaxX) {
  //   std::swap(tMinX, tMaxX);
  // }

  // float tMinY = (min.y - o.y) / d.y;
  // float tMaxY = (max.y - o.y) / d.y;
  // if (tMinY > tMaxY) {
  //   std::swap(tMinY, tMaxY);
  // }

  // float tMinZ = (min.z - o.z) / d.z;
  // float tMaxZ = (max.z - o.z) / d.z;
  // if (tMinZ > tMaxZ) {
  //   std::swap(tMinZ, tMaxZ);
  // }

  // //checks and calculate tMin/tMax intersection
  // if (tMinX > tMaxY || tMinY > tMaxX) {
  //   return false;
  // }

  // if (tMinX > tMinY) {
  //   tMin = tMinX;
  // } else {
  //   tMin = tMinY;
  // }

  // if (tMaxX < tMaxY) {
  //   tMax = tMaxX;
  // } else {
  //   tMax = tMaxY;
  // }

  // if (tMin > tMaxZ || tMinZ > tMax) {
  //   return false;
  // }

  // if (tMin < tMinZ) {
  //   tMin = tMinZ;
  // }
  // if (tMax > tMaxZ) {
  //   tMax = tMaxZ;
  // }

  // //is it in bounds?
  // if (tMin >= t0 && tMin <= t1) {
  //   t0 = tMin;
  //   intersected = true;
  // }
  // if (tMax <= t1 && tMax >= t0) {
  //   t1 = tMax;
  //   intersected = true;
  float tmin, tmax,tymin, tymax, tzmin, tzmax;

  Vector3D bounds[2];
  bounds[0] = min;
  bounds[1] = max;

  tmin = (bounds[r.sign[0]].x - r.o.x) * r.inv_d.x;
  tmax = (bounds[1-r.sign[0]].x - r.o.x) * r.inv_d.x;
  tymin = (bounds[r.sign[1]].y - r.o.y) * r.inv_d.y;
  tymax = (bounds[1-r.sign[1]].y - r.o.y) * r.inv_d.y;

  if ((tmin > tymax) || (tymin > tmax)) {
    return false;
  }
  if (tymin > tmin) {
    tmin = tymin;
    t0 = tmin;
  }
  if (tymax < tmax) {
    tmax = tymax;
    t1 = tmax;
  }
  tzmin = (bounds[r.sign[2]].z - r.o.z) * r.inv_d.z;
  tzmax = (bounds[1-r.sign[2]].z - r.o.z) * r.inv_d.z;
  if ((tmin > tzmax || tzmin > tmax)) {
    return false;
  }
  if (tzmin > tmin) {
    tmin = tzmin;
    t0 = tmin;
  } 
  if (tzmax < tmax) {
    tmax = tzmax;
    t1 = tmax;
  }

  return ((tmin < t1) && (tmax > t0));
}

void BBox::draw(Color c) const {

  glColor4f(c.r, c.g, c.b, c.a);

	// top
	glBegin(GL_LINE_STRIP);
	glVertex3d(max.x, max.y, max.z);
  glVertex3d(max.x, max.y, min.z);
  glVertex3d(min.x, max.y, min.z);
  glVertex3d(min.x, max.y, max.z);
  glVertex3d(max.x, max.y, max.z);
	glEnd();

	// bottom
	glBegin(GL_LINE_STRIP);
  glVertex3d(min.x, min.y, min.z);
  glVertex3d(min.x, min.y, max.z);
  glVertex3d(max.x, min.y, max.z);
  glVertex3d(max.x, min.y, min.z);
  glVertex3d(min.x, min.y, min.z);
	glEnd();

	// side
	glBegin(GL_LINES);
	glVertex3d(max.x, max.y, max.z);
  glVertex3d(max.x, min.y, max.z);
	glVertex3d(max.x, max.y, min.z);
  glVertex3d(max.x, min.y, min.z);
	glVertex3d(min.x, max.y, min.z);
  glVertex3d(min.x, min.y, min.z);
	glVertex3d(min.x, max.y, max.z);
  glVertex3d(min.x, min.y, max.z);
	glEnd();

}

std::ostream& operator<<(std::ostream& os, const BBox& b) {
  return os << "BBOX(" << b.min << ", " << b.max << ")";
}

} // namespace CGL
