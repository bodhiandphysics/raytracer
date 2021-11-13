#include "mat3.h"
#include "nicefp.h"
#include "ray.h"
#include "vec3.h"

namespace geom {

struct uv {

  nicefp u;
  nicefp v;

  uv() = default;
  uv(nicefp au, nicefp av);
  uv(double au, double av);
};

struct interception {

  nicefp distance;
  vec3 position; // vector from center of object to intercept point
  vec3 normal;
  uv atuv;

  interception(const nicefp &distance, const vec3 &position, const vec3 &normal,
               const uv &uv);
};

struct shape {

  virtual bool calc_interception(ray &theray, const nicefp within_d,
                                 interception &out);
  virtual void translate(const vec3 &by); // always translate after rotating
  virtual void rotate(const mat3 &by);    // always rotate before translation
  virtual void scale(const vec3 &by);
  virtual uv getuv(vec3 &position);
  virtual vec3 get_normal(const vec3 &position);
};

struct triangle : shape {

  vec3 position;
  vec3 side_a;
  vec3 side_b;
  uv uv_a, uv_b, uv_c;
  vec3 normal;
  nicefp area;

  triangle(vec3 aposition, vec3 aside_a, vec3 aside_b, vec3 anormal, uv uv_a,
           uv uv_b, uv uv_c);

  virtual void translate(const vec3 &by) override;
  virtual void rotate(const mat3 &by) override;
  virtual void scale(const vec3 &by) override;
  virtual bool calc_interception(ray &theray, const nicefp within_d,
                                 interception &out) override;
  virtual uv getuv(vec3 &intersect) override;
  virtual vec3 get_normal(const vec3 &position) override;
};

struct sphere : shape {

  vec3 center;
  vec3 orientation;
  vec3 orientation_equator;
  nicefp maxangle;
  nicefp radius;

  sphere(const vec3 acenter, vec3 orientationvec, vec3 orientation_equatorvec,
         nicefp amaxangle, nicefp aradius);

  virtual vec3 get_normal(const vec3 &position) override;
  virtual uv getuv(vec3 &position) override;
  virtual void translate(const vec3 &by) override;
  virtual void rotate(const mat3 &by) override;
  virtual void scale(const vec3 &by) override;
  virtual bool calc_interception(ray &theray, const nicefp within_d,
                                 interception &out) override;
};

struct plane : shape {

  vec3 center;
  vec3 normal;
  vec3 u_axis;
  vec3 v_axis;

  plane(vec3 acenter, vec3 anormal, vec3 au_axis, vec3 av_axis);
  virtual bool calc_interception(ray &theray, const nicefp within_d,
                                 interception &out) override;
  virtual void translate(const vec3 &by) override;
  virtual void rotate(const mat3 &by) override;
  virtual void scale(const vec3 &by) override;
  virtual uv getuv(vec3 &position) override;
  virtual vec3 get_normal(const vec3 &position) override;
};

} // namespace geom