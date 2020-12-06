#pragma once

#include "ray_def.h"

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

class Timer {
  clock_t _start, _end;

 public:
  Timer() {}
  void start() { _start = clock(); }
  double stop() {
    _end = clock();
    return ((double)_start - (double)_end) / CLK_TCK;
  }
};

}  // namespace raycer