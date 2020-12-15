#include <stdarg.h>
#include <stdio.h>

#include "core.h"

void printDebug(const char *fmt, ...) {
  va_list vargs;
  va_start(vargs, fmt);

  printf("# ");
  vprintf(fmt, vargs);
  fflush(stdout);
}