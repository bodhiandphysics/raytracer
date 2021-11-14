#pragma once
#include "nicefp.h"
#include "vec3.h"

namespace geom {
struct ray {

  vec3 origin;
  vec3 direction; // should always be normalized;

  ray(const vec3 &anorigin, vec3 &unnormalized);
  ray() = default;
  ray reflect(vec3 &point, vec3 &normal);
  bool refract(vec3 &point, vec3 &normal, nicefp &inref_index,
               nicefp &outref_index, ray &result);
};
} // namespace geom