#include <stdarg.h>
#include <stdio.h>
#include <unistd.h>
#include <stdarg.h>

#include <stdlib.h>

#include "core.h"

char xboardMode = 0;

u64 getRandom() {
  u64 lo = (u64)rand();
  u64 hi = (u64)rand();

  return (u64)((hi << 32 & 0xFFFFFFFF00000000ULL) | (lo & 0xFFFFFFFFULL));
}

void setXboardMode(char val) {
  xboardMode = val;
}

int getXboardMode() {
  return xboardMode;
}

void printDebug(const char *fmt, ...) {
  char buffer[256];
  va_list vargs;
  va_start(vargs, fmt);

  // if (!xboardMode) 
  return;
  
  write(STDOUT_FILENO, "# ", 2);
  write(STDOUT_FILENO, buffer, vsnprintf(buffer, 256, fmt, vargs) + 1);
}