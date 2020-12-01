typedef struct _Magic {
  unsigned long long mask;
  unsigned long int shift;
  unsigned long long magic;
  unsigned long long *attSets;
} Magic;

Magic *magic_RMagic;
Magic *magic_BMagic;

/**********************
 * A set mask of high *
 * bits `index` long  *
 * 0 = 0b             *
 * 1 = 1b             *
 * 5 = 11111b         *
 * ********************/
unsigned long long magic_ShiftMask[17];

void magic_init();