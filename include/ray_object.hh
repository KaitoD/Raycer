#pragma once

#include <float.h>

#include "ray_math.hh"
#include "ray_misc.hh"
#include "ray_ray.hh"

namespace raycer {
  struct intersection;

  class material {
   public:
    virtual bool scatter(const ray& ray_in, const intersection& rec,
                         vec3& attenuation, ray& scattered) const = 0;
    virtual vec3 emitted(fnum u, fnum v, const vec3& p) const { return vec3(); }
    virtual ~material() {}
  };

  struct intersection {
    fnum t = 0;
    vec3 intersect_point;
    vec3 norm;
    material* material = nullptr;
    fnum u = 0, v = 0;
  };

  struct aabb {
    vec3 min, max;
    aabb() {}
    aabb(const vec3& a, const vec3& b) : min(a), max(b) {}

    bool hit(const ray& r, fnum t_min, fnum t_max) const {
      for (auto a = 0; a < 3; ++a) {
        fnum inv_dir = 1.0f / r.direction[a];
        fnum t0 = (min[a] - r.origin[a]) * inv_dir;
        fnum t1 = (max[a] - r.origin[a]) * inv_dir;
        if (inv_dir < 0.0f) swap(t0, t1);
        t_min = t0 > t_min ? t0 : t_min;
        t_max = t1 < t_max ? t1 : t_max;
        if (t_max <= t_min) return false;
      }
      return true;
    }
  };

  class object {
   public:
    virtual bool hit(const ray& r, fnum k_min, fnum k_max,
                     intersection& rec) const = 0;
    virtual bool bounding_box(fnum t0, fnum t1, aabb& box) const = 0;
    virtual ~object() {}
  };
}  // namespace raycer