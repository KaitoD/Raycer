#pragma once

#include "ray_def.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

namespace raycer {

class StackTracer;
static StackTracer* curStack = nullptr;
class StackTracer {
  const char* name;
  StackTracer* prevStack;

 public:
  StackTracer(const char* stackName) : name(stackName) {
    prevStack = curStack;
    curStack = this;
  }
  const char* getName() const { return name; }
  StackTracer* getPrev() const { return prevStack; }
  ~StackTracer() { curStack = curStack->prevStack; }
};

#ifdef RAYCER_STACKTRACE
#define TRACE(_Name) raycer::StackTracer __detail_stack_tracer(_Name)
#else
#define TRACE(_Name)
#endif

inline void panic(const char* fmt, ...) {
  TRACE("void panic(const char* fmt, ...)");
  fprintf(stderr, "\nexit with panic: ");
  va_list ap;
  va_start(ap, fmt);
  vfprintf(stderr, fmt, ap);
  va_end(ap);
  fputc('\n', stderr);

#ifdef RAYCER_STACKTRACE
  fprintf(stderr, "\n===== stack rewind =====\n");
  int ind = 2;
  while (curStack) {
    int i = ind;
    while (--i) fputc(' ', stderr);
    ind += 2;
    fprintf(stderr, "   %s\n", curStack->getName());
    curStack = curStack->getPrev();
  }
#endif

  exit(EXIT_FAILURE);
}

}  // namespace raycer