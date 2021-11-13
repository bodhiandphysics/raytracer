#include "geom.h"
#include <functional>
#include <math.h>

namespace geom {

// basic geometry routines

struct nicefp {

  // wrapper for doubles to avoid mistakes made in fp comparison

  nicefp(double avalue) { value = avalue; }

  nicefp(const nicefp &other) { // copy consturctor

    value = other.value;
  }

  nicefp() = default;

  nicefp operator+(const nicefp &other) {
    return nicefp((value + other.value));
  }
  nicefp operator-(const nicefp &other) {
    return nicefp((value - other.value));
  }
  nicefp operator-() { return nicefp(-value); }
  nicefp operator*(const nicefp &other) {
    return nicefp((value * other.value));
  }
  nicefp operator/(const nicefp &other) {
    return nicefp((value / other.value));
  }
  void operator=(const nicefp &other) { this->value = other.value; }

  nicefp operator+(const double other) { return value + other; }
  nicefp operator-(const double other) { return value - other; }
  nicefp operator*(const double other) { return value * other; }
  nicefp operator/(const double other) { return value / other; }

  bool operator<(const nicefp &other) {

    double difference = value - other.value;
    if (difference < FPCUTOFF)
      return true;
    return false;
  }
  bool operator==(const nicefp &other) {

    double difference = value - other.value;

    if (difference < FPCUTOFF && difference > -FPCUTOFF)
      return true;
    return false;
  }
};

nicefp sqrt(nicefp x) { return nicefp(sqrt(x.value)); }

// calculate the negative exp of a nicecp;
nicefp nicenexp(nicefp x) {

  if (x.value = 0)
    return 1;
  else
    return exp(-x.value);
}

// 3 vectors, used to represent locations, displacements, and colors

struct vec3 {

  nicefp x;
  nicefp y;
  nicefp z;

  vec3() = default;

  vec3(nicefp x, nicefp y, nicefp z) {

    this->x = x;
    this->y = y;
    this->z = z;
  }

  vec3(double x, double y, double z) {

    this->x = nicefp(x);
    this->y = nicefp(y);
    this->z = nicefp(z);
  }

  vec3 mult(const vec3 &other) { // a termwise mult, mostly used for colors

    return vec3(x * other.x, y * other.y, z * other.z);
  }

  nicefp norm() { // the length of the vector

    return sqrt(x * x + y * y + z * z);
  }

  nicefp norm2() { // the norm without the sqrt, to avoid slow ops

    return x * x + y * y + z * z;
  }

  vec3 normalize() { // return a normalization of the vector

    nicefp thenorm = norm();
    if (thenorm == 0) // the direction is arbitrary
      return vec3(nicefp(1), nicefp(0), nicefp(0));
    else
      return vec3(x / thenorm, y / thenorm, z / thenorm);
  }

  vec3 operator+(const vec3 &other) {

    return vec3(x + other.x, y + other.y, z + other.z);
  }

  vec3 operator-(const vec3 &other) {

    return vec3(x - other.x, y - other.y, z - other.z);
  }

  bool operator==(const vec3 &other) {

    return x == other.x && y == other.y && z == other.z;
  }

  vec3 operator*(const nicefp &mult) { // scalar multiplication

    return vec3(x * mult, y * mult, z * mult);
  }

  vec3 operator*(double mult) { // scalar multiplication

    return vec3(x * mult, y * mult, z * mult);
  }

  vec3 operator/(const nicefp &div) { return vec3(x / div, y / div, z / div); }

  nicefp dot(const vec3 &other) {

    return x * other.x + y * other.y + z * other.z;
  }

  vec3 cross(const vec3 &other) {

    nicefp returnz = x * other.y - y * other.x;
    nicefp returny = z * other.x - x * other.z;
    nicefp returnx = y * other.z - z * other.y;

    return vec3(returnx, returny, returnz);
  }

  vec3 nexp(const vec3 &vec) { // e ^ -x, userful for the beer equation

    return vec3(nicenexp(vec.x), nicenexp(vec.y), nicenexp(vec.z));
  }

  vec3
  reflect(const vec3 &normal) { // find the reflection of a vector on normal

    const nicefp vdotn = this->dot(normal);
    vec3 newdirection = *this - (normal * vdotn) * 2;
    return newdirection;
  }

  void
  translate(const vec3 &by) { // move the vector (note... move original vector!)

    x = x + by.x;
    y = y + by.y;
    z = z + by.z;
  }

  void rotate(const mat3 &rotation) { // rotate the vector

    nicefp xbuff, ybuff, zbuff;

    xbuff = this->dot(rotation.cols[0]);
    ybuff = this->dot(rotation.cols[1]);
    zbuff = this->dot(rotation.cols[2]);

    x = xbuff;
    y = ybuff;
    z = zbuff;
  }

  void scale(double by) {

    x = x * by;
    y = y * by;
    z = z * by;
  }
};

struct mat3 {

  // 3 by 3 matrices, represented as vectors of vec3s, because I'm insane

  vec3 cols[3];

  mat3(vec3 col1, vec3 col2, vec3 col3) {

    cols[0] = col1;
    cols[1] = col2;
    cols[2] = col3;
  }

  mat3 transpose() { // matrix transpose

    vec3 col1, col2, col3;

    col1.x = cols[0].x;
    col1.y = cols[1].x;
    col1.z = cols[2].x;

    col2.x = cols[0].y;
    col2.y = cols[1].y;
    col2.z = cols[2].y;

    col3.x = cols[0].z;
    col3.y = cols[1].z;
    col3.z = cols[2].z;

    return mat3(col1, col2, col3);
  }

  mat3 leftmult(mat3 other) { // apply as linear operator.
                              // all this is slow!!!!! But i'm not animating, so
                              // this will be used very rarely

    vec3 buffer1, buffer2, buffer3;
    mat3 trans = other.transpose();

    buffer1.x = trans.cols[0].dot(cols[0]);
    buffer1.y = trans.cols[1].dot(cols[0]);
    buffer1.z = trans.cols[2].dot(cols[0]);

    buffer2.x = trans.cols[0].dot(cols[1]);
    buffer2.y = trans.cols[1].dot(cols[1]);
    buffer2.z = trans.cols[2].dot(cols[1]);

    buffer3.x = trans.cols[0].dot(cols[2]);
    buffer3.y = trans.cols[1].dot(cols[2]);
    buffer3.z = trans.cols[2].dot(cols[2]);

    return mat3(buffer1, buffer2, buffer3);
  }
};

mat3 genrotz(
    const nicefp &theta) { // convert angle (in radians) to rotation matrix;

  double costheta = cos(theta.value);
  double sintheta = sin(theta.value);

  return mat3(vec3(costheta, sintheta, 0), vec3(sintheta, costheta, 0),
              vec3(0, 0, 1));
}

mat3 genroty(const nicefp &theta) {

  double costheta = cos(theta.value);
  double sintheta = sin(theta.value);

  return mat3(vec3(costheta, 0, sintheta), vec3(0, 1, 0),
              vec3(-sintheta, 0, costheta));
}

mat3 genrotx(const nicefp &theta) {

  double costheta = cos(theta.value);
  double sintheta = sin(theta.value);

  return mat3(vec3(1, 0, 0), vec3(0, costheta, -sintheta),
              vec3(0, sintheta, costheta));
}
struct ray { // the fundemental structure of the ray tracer.  A ray from a point
             // in a (normalized) direction

  vec3 origin;
  vec3 direction; // should always be normalize; but can be passed in
                  // unnormalized

  ray(vec3 anorigin, vec3 unnormalized) {

    origin = anorigin;
    direction = unnormalized.normalize();
  }

  ray reflect(vec3 &point, const vec3 &normal) { // create a reflection ray

    vec3 newdirection = direction.reflect(normal);
    vec3 peterbation = point + newdirection * .001;
    return ray(peterbation, newdirection); // perturb the new ray a little
  }

  // this is aweful!! Create a refraction ray.

  bool refract(vec3 &point, vec3 &normal, nicefp &inref_index,
               nicefp &outref_index, ray &result) {

    if (outref_index == 0)
      return false; // no refraction because no light transport

    double index_ratio = inref_index.value / outref_index.value;
    double cosincedent =
        -(direction.dot(normal)
              .value); // we're coming into the point, so invert direction
    double sinout2 = (1 - cosincedent * cosincedent) * index_ratio;
    double discriminant = 1 - sinout2;

    if (discriminant <= .00001)
      return false; // total internal reflection

    vec3 refractdir =
        direction * index_ratio +
        nicefp(index_ratio * cosincedent - sqrt(discriminant)) * normal;

    result.origin =
        point + refractdir * nicefp(.0001); // perturb the new ray a little
    result.direction = refractdir;

    return true;
  }
};

// interception structures hold the information from an interception

struct interception {

  nicefp distance;
  vec3 position; // vector from center of object to intercept point
  vec3 normal;
  uv uv;

  interception(nicefp distance, vec3 position, vec3 normal, uv uv) {

    this->distance = distance;
    this->postition = position;
    this->normal = normal;
    this->uv = uv;
  }
};

// u v coordinates for textures

struct uv {

  nicefp u;
  nicefp v;

  uv(nicefp au, nicefp av) {

    u = au;
    v = av;
  }

  uv(double au, double av) { uv(nicefp(au), nicefp(av)); }
};

// the basic geometric primative.  Currently only includes sphere and triables
// (probably should add cones and cyllinders)
struct shape {

  virtual bool calc_interception(const ray &ray, const nicefp &within_d,
                                 intercetion &out) = 0;
  virtual void translate(const vec3 &by) = 0; // always translate after rotating
  virtual void rotate(const vec3 &by) = 0; // always rotate before translation
  virtual void scale(double factor) = 0;

  virtual uv getuv(vec3 &position) = 0;
  virtual vec3 get_normal(vec3 &position) = 0;
};

struct triangle : shape {

  vec3 position;
  vec3 side_a;
  vec3 side_b uv uv_a, uv_b, uv_c;

  vec3 normal; // not normalized!!!! for optimization
  nicefp area2;
  nicefp area;

  triangle(vec3 aposition, vec3 aside_a, vec3 aside_b, vec3 anormal, uv uv_a,
           uv uv_b, uv uv_c) {

    position = aposition;
    side_a = aside_a;
    side_b = aside_b;
    this->uv_a = uv_a;
    this->uv_b = uv_b;
    this->uv_c = uv_c;

    normal = anormal;
    area2 = normal.norm2();
    area = sqrt(area2);
  }

  virtual void translate(const vec &by) override { position += by; }

  virtual void rotate(const mat3 &by)
      override { // don't rotate triangles unless to absolutely must

    position.rotate(by);
    point_b.rotate(by);
    point_c.rotate(by);

    normal = normal.rotate(
        by); // update normal!!! (could rotate, but this prevents errors?)
  }

  virtual void scale(double factor) override {

    position *= factor;
    side_a *= factor;
    side_b *= factor;
    area2 *= factor * factor;
    area *= factor;
  }

  virtual bool calc_interception(const ray &ray, const nicefp &within_d,
                                 &interception out) override {

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
      return false;

    vec3 p = ray.direction.cross(edge2);

    vec3 q = origin_to_tri.cross(edge1);

    u = p.dot(origin_to_tri);

    if (u < 0 || determinant < u)
      return false;

    v = ray.direction.dot(q);

    if (v < 0 || determinant < u + v)
      return false;

    t = q.dot(edge2);

    out.distance = t;
    out.position = position + ((u * edge1) + (v * edge2)) / sqrt(determinant);
    out.normal = normal;
    out.uv = getuv(out.position);
    return true;
  }

  uv getuv(vec3 &intersect) { // using barycentric cooredinate

    if (area2 = 0)
      return uv(uv_a, 0, 0); // cut out early for really small triangles

    vec3 delta_p = intersect - position;

    nicefp APBcross = side_a.cross(delta_p);
    nicefp APCcross = sibe_b.cross(delta_p);

    nicefp beta = APBcross.dot(normal) / area2;
    nicefp gamma = APCcross.dot(normal) / area2;
    nicefp alpha = 1 - beta - gamma;

    nicefp u = uv_a.u * alpha + uv_b.u * beta + uv_c.u * gamma;
    nicefp v = uv_a.v * alpha + uv_b.v * beta + uv_c.v * gamma;

    return uv(u, v);
  }

  vec3 get_normal(vec3 &position) { return normal / area; }
};

struct sphere : shape {

  vec3 center;
  vec3 orientation;
  vec3 orientation_equator;
  nicefp maxangle nicefp radius;

  virtual sphere(vec3 acenter, vec3 orientationvec, vec3 orientation_equatorvec,
                 nicefp amaxangle, nicefp aradius) { // maxangle in cos nicefps

    center = acenter;
    orientation = orientationvec;
    orientation_equator = orientation_equatorvec;
    maxangle = amaxangle;
    radius = aradius;
  }

  virtual vec3 get_normal(vec3 &position) override {

    return (position - center) / radius;
  }

  virtual void translate(const vec &by) { center.translate(by); }

  virtual void rotate(const vec3 &by) {

    center.rotate(by);
    orientationvec.rotate(by);
    orientation_equator.rotate(by);
  }

  virtual void scale(double factor) {

    center *= scale;
    radius *= scale;
  }

  vec3 get_normal(const vec3 &position) { // centered on origin of sphere!!!

    return (position - center) / radius;
  }

  virtual bool interception calc_interception(const &ray,
                                              const nicefp &within_d,
                                              &interception out) {

    // uses the algorithm on page 76 of Shirley and Marschner

    const vec3 delta_o_c = ray.origin - center;
    const nicefp d_dot_d = ray.direction.dot(ray.direction);
    const nicefp d_dot_o_c = ray.direction.dot(delta_o_c);

    const nicefp descriminant =
        (d_dot_o_c * d_dot_o_c) -
        (d_dot_d * (delta_o_c.dot(delta_o_c) - radius * radius));

    if (descriminant < 0)
      return false;

    const nicefp sd = sqrt(descriminant);

    const nicefp distance_p =
        sd - d_dot_o_c; // note direction is always normalized!

    const nicefp distance_m = -1 * sd - d_dot_o_c;

    if (distance_m < distance_p && distance_m.value > 0 &&
        distance_m < within_d) {
      out.distance = distance_m;
      out.position =
          delta_o_c +
          direction * distance_m; // i.e. orign + direction* distance - center
      if ((out.position - center).dot(orientation) > amaxangle)
        return true;
    }

    if (distance_p.value > 0 && distance_p < within_d)
      if (distance_p.value > 0 && distance_p < within_d) {
        out.distance = distance_m;
        out.position =
            delta_o_c +
            direction * distance_p; // i.e. orign + direction* distance - center
        out.normal = getnormal(out.position);
        out.uv = getuv(out.position);
        if ((out.position - center).dot(orientation) > amaxangle)
          return true;
      }

    return false;
  }

  virtual uv getuv(const vec3 &position) override { // fix this later!!!!

    vec3 delta = (center - position) / radius;
    double odotd = orientation.dot(delta).value;

    double v = acos(odotd) / M_PI;
    vec3 equat_delta = delta - orientation * odotd; // project delta onto plane;
    double u = atan2(orientation_equator.cross(equat_delta).norm() /
                     orientation_equator.dot(equat_delta)) /
               (2 * M_PI);
  }
};

struct plane : shape {

  vec3 center;
  vec3 normal;
  vec3 u_axis;
  vec3 v_axis;

  virtual plane(vec3 acenter, vec3 anormal, vec3 au_axis, vec3 av_axis) {

    center = acenter;
    normal = anormal / anormal.norm();
    ; // should be normalized!
    u_axis = au_axis / au_axis.norm();
    v_axis = av_axis / au_axis.norm();
  }

  virtual bool calc_interception(const &ray, const nicefp &within_d,
                                 &interception out) {

    nicefp denom = ray.direction.dot(normal);
    if (denom == 0)
      return false; // parralel to plane

    interception.distance = (center - ray.origin).dot(normal) / denom;
    interception.position = ray.origin + ray.direction * interception.distance;
    interception.normal = normal;

    return true;
  }

  virtual void translate(const vec &by) { // always translate after rotating

    center.translate(by);
  }
  virtual void rotate(const vec3 &by) { // always rotate before translation

    normal.rotate(by);
    u_axis.rotate(by);
    v_axis.rotate(by);
  }

  virtual void scale(double factor){}; // do nothing

  virtual uv getuv(const vec3 &position) {

    vec3 deltap = position - center;

    nicefp u = delta_p.dot(u_axis);
    nicefp v = delta_p.dot(v_axis);

    return uv(u, v);
  }

  virtual vec3 get_normal(const vec3 &position) { return normal; }
};
} // namespace geom
