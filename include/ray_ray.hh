#pragma once

#include "ray_math.hh"

namespace raycer {
  struct ray {
    vec3 origin, direction;

    ray() {}
    ray(const vec3& ori, const vec3& dir) : origin(ori), direction(dir) {}
    vec3 get_path(fnum k) const { return origin + k * direction; }
  };
}  // namespace raycer