#pragma once
#include "nicefp.h"
#include "vec3.h"
#include <math.h>

namespace geom {

struct mat3 {

  vec3 cols[3];

  mat3(const vec3 &col1, const vec3 &col2, const vec3 &col3);

  mat3 transpose();

  mat3 leftmult(mat3 &other);
};

mat3 genrotz(const nicefp &theta);

mat3 genroty(const nicefp &theta);

mat3 genrotx(const nicefp &theta);
} // namespace geom