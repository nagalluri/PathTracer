#include "sphere.h"

#include <cmath>

#include  "../bsdf.h"
#include "../misc/sphere_drawing.h"

namespace CGL { namespace StaticScene {

bool Sphere::test(const Ray& r, double& t1, double& t2) const {

  // TODO Part 1, task 4:
  // Implement ray - sphere intersection test.
  // Return true if there are intersections and writing the
  // smaller of the two intersection times in t1 and the larger in t2.

  double a = dot(r.d,r.d);
  double b = 2.0 * dot(r.o - o, r.d);
  double c = dot(r.o - o,r.o - o) - r2;
  double d = b * b - (4.0 * a * c);
  if (d < 0) {
    return false;
  }
  else if (d == 0) {
    t1 = -1 * (b/(2*a));
    return true;
  }
  else {
    double x = (-b + sqrt(d))/(2*a);
    double y = (-b - sqrt(d))/(2*a);
    if (x >= y) {
      t1 = y;
      t2 = x;
    } else {
      t1 = x;
      t2 = y;
    }
    if (t1 <= r.min_t || r.max_t <= t1) {
      if (t2 <= r.min_t || r.max_t <= t2) {
        return false;
      }
    }
    return true;
  }


}

bool Sphere::intersect(const Ray& r) const {

  // TODO Part 1, task 4:
  // Implement ray - sphere intersection.
  // Note that you might want to use the the Sphere::test helper here.
  double t1 = 0.0;
  double t2 = 0.0;
  bool doesInt = test(r, t1, t2);
  if (doesInt == false) {
    return false;
  } else {
    r.max_t = t1;
    return true;
  }
}

bool Sphere::intersect(const Ray& r, Intersection *i) const {

  // TODO Part 1m task 4:
  // Implement ray - sphere intersection.
  // Note again that you might want to use the the Sphere::test helper here.
  // When an intersection takes place, the Intersection data should be updated
  // correspondingly.
  double t1 = 0.0;
  double t2 = 0.0;
  bool doesInt = test(r, t1, t2);
  if (doesInt == false) {
    return false;
  } else {
    if (t1 < r.min_t || t1 > r.max_t) {
      if (t2 > r.min_t && t2 < r.max_t) {
        t1 = t2;
      } else {
        return false;
      }

    }
    r.max_t = t1;
    i->t = t1;
    Vector3D normal = (r.o + (t1 * r.d)) - o;
    normal.normalize();
    i->n = normal;
    i->primitive = this;
    i->bsdf = get_bsdf();
    return true;
  }

}

void Sphere::draw(const Color& c) const {
  Misc::draw_sphere_opengl(o, r, c);
}

void Sphere::drawOutline(const Color& c) const {
    //Misc::draw_sphere_opengl(o, r, c);
}


} // namespace StaticScene
} // namespace CGL
