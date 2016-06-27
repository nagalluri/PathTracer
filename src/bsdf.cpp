#include "bsdf.h"

#include <iostream>
#include <algorithm>
#include <utility>

using std::min;
using std::max;
using std::swap;

namespace CGL {

void make_coord_space(Matrix3x3& o2w, const Vector3D& n) {

    Vector3D z = Vector3D(n.x, n.y, n.z);
    Vector3D h = z;
    if (fabs(h.x) <= fabs(h.y) && fabs(h.x) <= fabs(h.z)) h.x = 1.0;
    else if (fabs(h.y) <= fabs(h.x) && fabs(h.y) <= fabs(h.z)) h.y = 1.0;
    else h.z = 1.0;

    z.normalize();
    Vector3D y = cross(h, z);
    y.normalize();
    Vector3D x = cross(z, y);
    x.normalize();

    o2w[0] = x;
    o2w[1] = y;
    o2w[2] = z;
}

// Diffuse BSDF //

Spectrum DiffuseBSDF::f(const Vector3D& wo, const Vector3D& wi) {
  return albedo * (1.0 / PI);
}

Spectrum DiffuseBSDF::sample_f(const Vector3D& wo, Vector3D* wi, float* pdf) {
  *wi = sampler.get_sample(pdf);
  return albedo * (1.0 / PI);
}

// Mirror BSDF //

Spectrum MirrorBSDF::f(const Vector3D& wo, const Vector3D& wi) {
  return Spectrum();
}

Spectrum MirrorBSDF::sample_f(const Vector3D& wo, Vector3D* wi, float* pdf) {

  // TODO Part 5:
  // Implement MirrorBSDF
  reflect(wo, wi);
  double cosine = abs_cos_theta(*wi);
  Spectrum result = reflectance/cosine;
  *pdf = 1;
  return result;
}

// Glossy BSDF //

/*
Spectrum GlossyBSDF::f(const Vector3D& wo, const Vector3D& wi) {
  return Spectrum();
}

Spectrum GlossyBSDF::sample_f(const Vector3D& wo, Vector3D* wi, float* pdf) {
  *pdf = 1.0f;
  return reflect(wo, wi, reflectance);
}
*/

// Refraction BSDF //

Spectrum RefractionBSDF::f(const Vector3D& wo, const Vector3D& wi) {
  return Spectrum();
}

Spectrum RefractionBSDF::sample_f(const Vector3D& wo, Vector3D* wi, float* pdf) {

  // TODO Part 5:
  // Implement RefractionBSDF
  return Spectrum();
}

// Glass BSDF //

Spectrum GlassBSDF::f(const Vector3D& wo, const Vector3D& wi) {
  return Spectrum();
}

Spectrum GlassBSDF::sample_f(const Vector3D& wo, Vector3D* wi, float* pdf) {

  // TODO Part 5:
  // Compute Fresnel coefficient and either reflect or refract based on it.
  double no, ni;
  if (wo.z < 0) {
    //r = ior/1;
    no = ior;
    ni = 1.0;
  } else {
    //r = 1.0/ior;
    no = 1.0;
    ni = ior;
  }

  bool totalIR = refract(wo, wi, ior);
  if (!totalIR) {

    reflect(wo, wi);
    *pdf = 1.0;
    return reflectance / abs_cos_theta(*wi);

  } else {
    float Ro = ((no - ni)/ (no + ni)) * ((no - ni)/ (no + ni));
    float R = Ro + (1.0 - Ro)*pow((1.0 - abs_cos_theta(wo)), 5.0);
    R = clamp(R, 0.0, 1.0);
    if (coin_flip(R)) {
      reflect(wo, wi);
      *pdf = R;
      return R * (reflectance / abs_cos_theta(*wi));
    } else {
      refract(wo, wi, ior);
      *pdf = 1.0 - R;
      //double r;

      //printf("%s\n", "here");
      return (1.0 - R) * (transmittance * no/ni * no/ni) / abs_cos_theta(*wi);
    }
  }
}

void BSDF::reflect(const Vector3D& wo, Vector3D* wi) {

  // TODO Part 5:
  // Implement reflection of wo about normal (0,0,1) and store result in wi.
  Vector3D n;
  n.x = 0.0;
  n.y = 0.0;
  n.z = 1.0;
  *wi = (2.0 * dot(wo, n) * n) - wo;
}

bool BSDF::refract(const Vector3D& wo, Vector3D* wi, float ior) {

  // TODO Part 5:
  // Use Snell's Law to refract wo surface and store result ray in wi.
  // Return false if refraction does not occur due to total internal reflection
  // and true otherwise. When dot(wo,n) is positive, then wo corresponds to a
  // ray entering the surface through vacuum.
  

  Vector3D n;
  n.x = 0.0;
  n.y = 0.0;
  n.z = 1.0;
  Vector3D l = -wo;
  float n_i, n_o; 
  double sign;
  if (wo.z < 0) {
    n_o = ior;
    n_i = 1.0f;
    //n = -n;
    sign = 1.0;
  } else {
    n_o = 1.0f;
    n_i = ior;
    sign = -1.0;
  }
  double r = n_o/n_i;
  double c = dot(-n, l);

  //*wi = r * l + (r*c - sqrt(1.0 - (pow(r, 2.0) * (1.0 - (pow(c, 2.0)))))) * n;
  double sin_theta_i = (n_o * sin_theta(wo))/n_i;
  //std::cout << "no: " << n_o << " n_i: " << n_i << std::endl;
  // if (sin_theta_i > 1.) {
  //   return false;
  // }

  
  Vector3D result;
  result.x = -r * wo.x;
  result.y = -r * wo.y;
  result.z = sign * fabs(sqrt((1.0 - pow(sin_theta_i, 2.0))));
  *wi = result;
  //std::cout << "wo: " << wo << " wi: " << *wi << std::endl;
  if (n_i * sin_theta_i >= n_o) {
    return false;
  } else {
    return true;
  }

}

// Emission BSDF //

Spectrum EmissionBSDF::f(const Vector3D& wo, const Vector3D& wi) {
  return Spectrum();
}

Spectrum EmissionBSDF::sample_f(const Vector3D& wo, Vector3D* wi, float* pdf) {
  *pdf = 1.0 / PI;
  *wi  = sampler.get_sample(pdf);
  return Spectrum();
}

} // namespace CGL
