#ifndef __core_h_
#define __core_h_

#define TRUE 1
#define FALSE 0

typedef unsigned long long u64;

u64 getRandom();

void printDebug(const char *fmt, ...);
void setXboardMode(char val);

#endif