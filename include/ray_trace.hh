#pragma once

#include "ray_object.hh"

namespace raycer {
  inline vec3 path_trace(const ray& r, object* w, int depth) {
    intersection rec;
    if (w->hit(r, 0.001, __FLT_MAX__, rec)) {
      ray scatter;
      vec3 attenuation;
      vec3 emitted = rec.material->emitted(rec.u, rec.v, rec.intersect_point);
      if (depth < RECURSE_DEPTH &&
          rec.material->scatter(r, rec, attenuation, scatter)) {
        auto result =
            emitted + attenuation.phong(path_trace(scatter, w, depth + 1));
        return result;
      } else
        return emitted;
    } else
      return vec3(0, 0, 0);
  }
}  // namespace raycer