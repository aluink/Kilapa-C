#include <stdarg.h>
#include <stdio.h>

#include <stdlib.h>

#include "core.h"

char xboardMode = 0;

u64 getRandom() {
  u64 lo = (u64)rand();
  u64 hi = (u64)rand();

  return (u64)((hi << 32 & 0xFFFFFFFF00000000ULL) | (lo & 0xFFFFFFFFULL));
}

void setXboard(char val) {
  xboardMode = val;
}

void printDebug(const char *fmt, ...) {
  va_list vargs;
  va_start(vargs, fmt);

  if (xboardMode) printf("# ");

  vprintf(fmt, vargs);
  fflush(stdout);
}