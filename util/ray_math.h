#pragma once

#include "ray_def.h"

namespace raycer {

inline float quakeRSqrt(float x) {
  float xhalf = 0.5 * x;
  int i = *(int*)&x;
  i = 0x5f3759df - (i >> 1);
  x = *(float*)&i;
  x = x * (1.5 - xhalf * x * x);
  return x;
}

inline float quakeSqrt(float x) { return quakeRSqrt(x) * x; }

inline float insSqrt(float x) {
  __m128 in = _mm_load_ss(&x);
  _mm_store_ss(&x, _mm_mul_ss(in, _mm_rsqrt_ss(in)));
  return x;
}

inline float insSqrt2(float x) {
  auto in = _mm_load_ss(&x);
  _mm_store_ss(&x, _mm_sqrt_ss(in));
  return x;
}

inline float insRSqrt(float x) {
  __m128 in = _mm_load_ss(&x);
  _mm_store_ss(&x, _mm_rsqrt_ss(in));
  return x;
}

inline float insDistance(float x, float y, float z) {
  __m128 in = _mm_load_ss(&x);
  __m128 acc = _mm_setzero_ps();
  acc = _mm_add_ss(acc, _mm_mul_ss(in, in));
  in = _mm_load_ss(&y);
  acc = _mm_add_ss(acc, _mm_mul_ss(in, in));
  in = _mm_load_ss(&z);
  acc = _mm_add_ss(acc, _mm_mul_ss(in, in));
  acc = _mm_sqrt_ss(acc);
  _mm_store_ss(&x, acc);
  return x;
}

inline float insSqrDistance(float x, float y, float z) {
  __m128 in = _mm_load_ss(&x);
  __m128 acc = _mm_setzero_ps();
  acc = _mm_add_ss(acc, _mm_mul_ss(in, in));
  in = _mm_load_ss(&y);
  acc = _mm_add_ss(acc, _mm_mul_ss(in, in));
  in = _mm_load_ss(&z);
  acc = _mm_add_ss(acc, _mm_mul_ss(in, in));
  _mm_store_ss(&x, acc);
  return x;
}

uint32_t xorshift128(void) {
  static uint32_t x = 123456789, y = 362436069, z = 521288629,
                  w = 521288629 ^ 123123123;
  uint32_t t = x ^ (x << 11);
  x = y;
  y = z;
  z = w;
  return w = w ^ (w >> 19) ^ t ^ (t >> 8);
}

template <typename T>
inline T gMin(T a, T b) {
  return a < b ? a : b;
}

template <typename T>
inline T gMax(T a, T b) {
  return a > b ? a : b;
}

#define xshf128() ((double)xorshift128() / (double)UINT32_MAX)
#define M_PI 3.14159265358979323846

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
  vec3 pos() const { return vec3(x, y, z); }
  vec3 operator+() const { return pos(); }
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

// vec * num
inline vec3 operator*(const vec3& v, fnum a) {
  return vec3(a * v.x, a * v.y, a * v.z);
}

}  // namespace raycer