#pragma once

#include "ray_def.hh"

namespace raycer {
  inline void panic(const char* fmt, ...) {
    fprintf(stderr, "\nexit with panic: ");
    va_list ap;
    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    va_end(ap);
    fputc('\n', stderr);
    exit(EXIT_FAILURE);
  }

  class timer {
    clock_t _start, _end;

   public:
    timer() {}
    void start() { _start = clock(); }
    double stop() {
      _end = clock();
      return ((double)_start - (double)_end);
    }
  };

  template <typename _t>
  void swap(_t& a, _t& b) {
    auto tmp = a;
    a = b;
    b = tmp;
  }
}  // namespace raycer