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
};

}  // namespace raycer