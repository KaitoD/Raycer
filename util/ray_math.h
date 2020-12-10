#pragma once

#include "ray_def.h"

namespace raycer {

struct vec3 {
  union {
    struct {
      fnum x, y, z;
    };
    struct {
      fnum r, g, b;
    };
    fnum vec[3];
  };

  fnum operator[](int i) const { return vec[i]; }
  fnum& operator[](int i) { return vec[i]; }

  vec3(fnum a = 0, fnum b = 0, fnum c = 0) : x(a), y(b), z(c) {}
  vec3(const vec3& v) : x(v.x), y(v.y), z(v.z) {}

  vec3& operator=(const vec3& v) {
    x = v.x;
    y = v.y;
    z = v.z;
    return *this;
  }

  vec3 neg() const { return vec3(-x, -y, -z); }
  vec3 operator-() const { return neg(); }
  vec3 add(const vec3& v) const { return vec3(x + v.x, y + v.y, z + v.z); }
  vec3 operator+(const vec3& v) const { return add(v); }
  vec3 sub(const vec3& v) const { return vec3(x - v.x, y - v.y, z - v.z); }
  vec3 operator-(const vec3& v) const { return sub(v); }

  vec3& addeq(const vec3& v) {
    x += v.x;
    y += v.y;
    z += v.z;
    return *this;
  }
  vec3& operator+=(const vec3& v) { return addeq(v); }

  vec3& subeq(const vec3& v) {
    x -= v.x;
    y -= v.y;
    z -= v.z;
    return *this;
  }
  vec3& operator-=(const vec3& v) { return subeq(v); }
};

// inner product
inline fnum iprod(const vec3& a, const vec3& b) {
  return a.x * b.x + a.y * b.y + a.z * b.z;
}
// outer product
inline vec3 oprod(const vec3& a, const vec3& b) {
  return vec3(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z,
              a.x * b.y - a.y * b.x);
}

// num * vec
inline vec3 operator*(fnum a, const vec3& v) {
  return vec3(a * v.x, a * v.y, a * v.z);
}

}  // namespace raycer