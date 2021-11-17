
#include "mat3.h"
#include "nicefp.h"

namespace geom {

vec3::vec3(nicefp x, nicefp y, nicefp z) {

  this->x = x;
  this->y = y;
  this->z = z;
}

vec3::vec3(double x, double y, double z) {

  this->x = nicefp(x);
  this->y = nicefp(y);
  this->z = nicefp(z);
}

vec3 vec3::mult(const vec3 &other) { // a termwise mult, mostly used for colors

  return vec3(x * other.x, y * other.y, z * other.z);
}

nicefp vec3::norm() { // the length of the vector

  return nicesqrt(x * x + y * y + z * z);
}

nicefp vec3::norm2() { // the norm without the sqrt, to avoid slow ops

  return x * x + y * y + z * z;
}

vec3 vec3::normalize() { // return a normalization of the vector

  nicefp thenorm = norm();
  if (thenorm == 0) // the direction is arbitrary
    return vec3(nicefp(1), nicefp(0), nicefp(0));
  else
    return vec3(x / thenorm, y / thenorm, z / thenorm);
}

vec3 vec3::operator+(const vec3 &other) {

  return vec3(x + other.x, y + other.y, z + other.z);
}

vec3 vec3::operator-(const vec3 &other) {

  return vec3(x - other.x, y - other.y, z - other.z);
}

bool vec3::operator==(const vec3 &other) {

  return x == other.x && y == other.y && z == other.z;
}

vec3 vec3::operator*(const nicefp &mult) { // scalar multiplication

  return vec3(x * mult, y * mult, z * mult);
}

vec3 vec3::operator*(double mult) { // scalar multiplication

  return vec3(x * mult, y * mult, z * mult);
}

vec3 vec3::operator/(const nicefp &div) {
  return vec3(x / div, y / div, z / div);
}

vec3 vec3::opterator/(double &div) {

  return vec3(x/div, y/div, z/div);
}

nicefp vec3::dot(const vec3 &other) {

  return x * other.x + y * other.y + z * other.z;
}

vec3 vec3::cross(const vec3 &other) {

  nicefp returnz = x * other.y - y * other.x;
  nicefp returny = z * other.x - x * other.z;
  nicefp returnx = y * other.z - z * other.y;

  return vec3(returnx, returny, returnz);
}

vec3 vec3::nexp(const vec3 &vec) { // e ^ -x, userful for the beer equation

  return vec3(nicenexp(vec.x), nicenexp(vec.y), nicenexp(vec.z));
}

vec3 vec3::reflect(vec3 &normal) { // find the reflection of a vector on normal

  const nicefp vdotn = this->dot(normal);
  vec3 newdirection = *this - (normal * vdotn) * 2;
  return newdirection;
}

void vec3::translate(
    const vec3 &by) { // move the vector (note... move original vector!)

  x = x + by.x;
  y = y + by.y;
  z = z + by.z;
}

void vec3::rotate(const mat3 &rotation) { // rotate the vector

  nicefp xbuff, ybuff, zbuff;

  xbuff = this->dot(rotation.cols[0]);
  ybuff = this->dot(rotation.cols[1]);
  zbuff = this->dot(rotation.cols[2]);

  x = xbuff;
  y = ybuff;
  z = zbuff;
}

void vec3::scale(const vec3 &by) {

  x = x * by.x;
  y = y * by.y;
  z = z * by.z;
}

} // namespace geom