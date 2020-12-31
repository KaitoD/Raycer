#pragma once

#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <xmmintrin.h>

namespace raycer {
  typedef float
      fnum;  // change the definition of num to gain higher precision, but
             // this is not necessary to use double since float is enough
}  // namespace raycer