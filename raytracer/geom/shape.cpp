#include "shape.h"
#include <math.h>

namespace geom {

uv::uv(nicefp au, nicefp av) {

  v = av;
  u = au;
}
uv::uv(double au, double av) { uv(nicefp(au), nicefp(av)); }

interception::interception(const nicefp distance, const vec3 position, const vec3 normal,
               const uv uv) {

  this->distance = distance;
  this->position = position;
  this->normal = normal;
  this->atuv = uv;
}

triangle::triangle(vec3 aposition, vec3 aside_a, vec3 aside_b, vec3 anormal,
                   uv uv_a, uv uv_b, uv uv_c) {

  position = aposition;
  side_a = aside_a;
  side_b = aside_b;
  this->uv_a = uv_a;
  this->uv_b = uv_b;
  this->uv_c = uv_c;

  normal = anormal;
  area = normal.norm() / 2;
}

triangle::~triangle() {};

void triangle::translate(const vec3 &by) { position = position + by; }

void triangle::rotate(
    const mat3 &by) { // don't rotate triangles unless to absolutely must

  position.rotate(by);
  side_a.rotate(by);
  side_b.rotate(by);

  normal.rotate(
      by); // update normal!!! (could rotate, but this prevents errors?)
}

void triangle::scale(const vec3 &by) {

  position = position.mult(by);
  side_a = side_a.mult(by);
  side_b = side_b.mult(by);
  normal = normal.mult(by).mult(by);
  area = normal.norm() / 2;
}

bool triangle::calc_interception(ray &ray, nicefp within_d, interception &out) {

  // this uses the Moller-Trumbore algorithm
  // note i'm not doing backface culling, since I'll need to intersect with
  // backfaces for refraction algorithm was mostly copied from the original
  // article, with some mods

  if (ray.direction.dot(normal) == 0)
    return false;

  nicefp t, u, v;

  vec3 edge1 = side_a;
  vec3 edge2 = side_b;
  vec3 origin_to_tri = ray.origin - position;

  nicefp determinant = ray.direction.cross(edge2).dot(edge1);

  if (determinant == 0)
    return false; // parallel to triangle

  vec3 p = ray.direction.cross(edge2);

  vec3 q = origin_to_tri.cross(edge1);

  u = p.dot(origin_to_tri);

  if (u < 0 || determinant < u)
    return false;

  v = ray.direction.dot(q);

  if (v < 0 || determinant < u + v)
    return false;

  t = q.dot(edge2);
  if (t < 0)
    return false; // missed triangle

  if (within_d < t)
    return false;
  out.distance = t;
  out.position = position + ((edge1 * u) + (edge2 * v)) / determinant;
  out.normal = get_normal(out.position);
  out.atuv = getuv(out.position);
  return true;
}

uv triangle::getuv(vec3 &intersect) { // using barycentric cooredinate

  if (area == 0)
    return uv_a; // cut out early for really small triangles

  vec3 delta_p = intersect - position;

  vec3 APBcross = side_a.cross(delta_p);
  vec3 APCcross = side_b.cross(delta_p);

  nicefp area2 = area * area;

  nicefp beta = APBcross.dot(normal) / area2;
  nicefp gamma = APCcross.dot(normal) / area2;
  nicefp alpha = nicefp(1) - beta - gamma;

  nicefp u = uv_a.u * alpha + uv_b.u * beta + uv_c.u * gamma;
  nicefp v = uv_a.v * alpha + uv_b.v * beta + uv_c.v * gamma;

  return uv(u, v);
}

vec3 triangle::get_normal(const vec3 &position) {

  return (normal * 2) / area; // area is half the norm of the normal
}

sphere::sphere(vec3 acenter, vec3 orientationvec, vec3 orientation_equatorvec,
               nicefp amaxangle, nicefp aradius) { // maxangle in cos nicefps

  center = acenter;
  orientation = orientationvec;
  orientation_equator = orientation_equatorvec;
  maxangle = amaxangle;
  radius = aradius;
}

sphere::~sphere() {};

vec3 sphere::get_normal(const vec3 &position) {

  return (position - center) /
         (radius); // i hate hate hate that you cant declare const this!
}

void sphere::translate(const vec3 &by) { center.translate(by); }

void sphere::rotate(const mat3 &by) {

  center.rotate(by);
  orientation.rotate(by);
  orientation_equator.rotate(by);
}

void sphere::scale(const vec3 &by) { // note elipses are not supported!!!!  make
                                     // sure all scales are diagonal

  center = center.mult(by);
  radius = radius * by.x; // acchch this is wrong!!
}

bool sphere::calc_interception(ray &ray, const nicefp within_d,
                               interception &out) {

  // uses the algorithm on page 76 of Shirley and Marschner
  bool didintercept = false;

  vec3 delta_o_c = ray.origin - center;
  nicefp d_dot_d = ray.direction.norm2();
  nicefp d_dot_o_c = ray.direction.dot(delta_o_c);

  nicefp descriminant =
      (d_dot_o_c * d_dot_o_c) -
      ((d_dot_d * (delta_o_c.norm2()) - (radius * radius)));

  if (descriminant < 0)
    return false;

  nicefp sd = nicesqrt(descriminant);

  nicefp distance_p = -sd + d_dot_o_c; // note direction is always normalized!

  nicefp distance_m = -sd - d_dot_o_c;
  nicefp possible_distance;
  vec3 possible_position;

  if ((distance_m < distance_p || distance_p < 0) && distance_m.value > 0 &&
      distance_m < within_d) {
    nicefp possible_distance = distance_m;
    vec3 possible_position =
        ray.origin + ray.direction*distance_m;
    if (maxangle < (possible_position - center).dot(orientation)) {
      out.distance = possible_distance;
      out.position = possible_position;
      didintercept = true;
    }
  } else {

    if (distance_p.value > 0 && distance_p < within_d) {
        possible_distance = distance_p;
        possible_position =
          ray.origin +
          ray.direction * distance_p; // i.e. orign + direction* distance - center

      if (maxangle < (possible_position - center).dot(orientation)) {
        out.distance = possible_distance;
        out.position = possible_position;
        didintercept = true;
      }
    }
  }

  if (didintercept) {

    out.normal = get_normal(out.position);
    out.atuv = getuv(out.position);
    return true;
  }

  return false;
}


uv sphere::getuv(vec3 &position) {

  // vec3 delta = (center - position) / radius;
  // double odotd = orientation.dot(delta).value;

  // double v = acos(odotd) / M_PI;
  // vec3 equat_delta = delta - orientation * odotd; // project delta onto plane;
  // double u = atan2(orientation_equator.cross(equat_delta).norm().value,
  //                  orientation_equator.dot(equat_delta).value) /
  //            (2 * M_PI);

  // return uv(u, v);

  return uv(nicefp(0), nicefp(0));
}

plane::plane(vec3 acenter, vec3 anormal, vec3 au_axis, vec3 av_axis) {

  center = acenter;
  normal = anormal / anormal.norm();
  ; // should be normalized!
  u_axis = au_axis / au_axis.norm();
  v_axis = av_axis / au_axis.norm();
}

bool plane::calc_interception(ray &ray, nicefp within_d, interception &out) {

  nicefp denom = ray.direction.dot(normal);
  if (denom == 0)
    return false; // parralel to plane

  out.distance = (center - ray.origin).dot(normal) / denom;
  out.position = ray.origin + ray.direction * out.distance;
  out.normal = normal;

  return true;
}

void plane::translate(const vec3 &by) { // always translate after rotating

  center.translate(by);
}
void plane::rotate(const mat3 &by) { // always rotate before translation

  normal.rotate(by);
  u_axis.rotate(by);
  v_axis.rotate(by);
}

void plane::scale(const vec3 &by){}; // do nothing

uv plane::getuv(vec3 &position) {

  vec3 deltap = position - center;

  nicefp u = deltap.dot(u_axis);
  nicefp v = deltap.dot(v_axis);

  return uv(u, v);
}

vec3 plane::get_normal(const vec3 &position) { return normal; }
} // namespace geom