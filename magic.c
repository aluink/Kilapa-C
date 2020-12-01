#include "stdlib.h"
#include "magic.h"
#include <stdio.h>

#define LEFT -1
#define RIGHT 1
#define UP 8
#define DOWN -8

void fooPrint(unsigned long long board) {
  for(int row = 7;row >= 0;row--) {
    for(int col = 0;col < 8;col++) {
      printf("%llu", board >> (row * 8 + col) & 1);
    }
    printf("\n");
  }               
  printf("\n");
}

int RShift[] = {
  12,11,11,11,11,11,11,12,
  11,10,10,10,10,10,10,11,
  11,10,10,10,10,10,10,11,
  11,10,10,10,10,10,10,11,
  11,10,10,10,10,10,10,11,
  11,10,10,10,10,10,10,11,
  11,10,10,10,10,10,10,11,
  12,11,11,11,11,11,11,12
};

/***************************
 * The number of positions *
 * in the Occupancy mask   *
 * ************************/
int BShift[] = {
  6,5,5,5,5,5,5,6,
  5,5,5,5,5,5,5,5,
  5,5,7,7,7,7,5,5,
  5,5,7,9,9,7,5,5,
  5,5,7,9,9,7,5,5,
  5,5,7,7,7,7,5,5,
  5,5,5,5,5,5,5,5,
  6,5,5,5,5,5,5,6
};

unsigned long long ROccMasks[] = {
  282578800148862ULL,
  565157600297596ULL,
  1130315200595066ULL,
  2260630401190006ULL,
  4521260802379886ULL,
  9042521604759646ULL,
  18085043209519166ULL,
  36170086419038334ULL,
  282578800180736ULL,
  565157600328704ULL,
  1130315200625152ULL,
  2260630401218048ULL,
  4521260802403840ULL,
  9042521604775424ULL,
  18085043209518592ULL,
  36170086419037696ULL,
  282578808340736ULL,
  565157608292864ULL,
  1130315208328192ULL,
  2260630408398848ULL,
  4521260808540160ULL,
  9042521608822784ULL,
  18085043209388032ULL,
  36170086418907136ULL,
  282580897300736ULL,
  565159647117824ULL,
  1130317180306432ULL,
  2260632246683648ULL,
  4521262379438080ULL,
  9042522644946944ULL,
  18085043175964672ULL,
  36170086385483776ULL,
  283115671060736ULL,
  565681586307584ULL,
  1130822006735872ULL,
  2261102847592448ULL,
  4521664529305600ULL,
  9042787892731904ULL,
  18085034619584512ULL,
  36170077829103616ULL,
  420017753620736ULL,
  699298018886144ULL,
  1260057572672512ULL,
  2381576680245248ULL,
  4624614895390720ULL,
  9110691325681664ULL,
  18082844186263552ULL,
  36167887395782656ULL,
  35466950888980736ULL,
  34905104758997504ULL,
  34344362452452352ULL,
  33222877839362048ULL,
  30979908613181440ULL,
  26493970160820224ULL,
  17522093256097792ULL,
  35607136465616896ULL,
  9079539427579068672ULL,
  8935706818303361536ULL,
  8792156787827803136ULL,
  8505056726876686336ULL,
  7930856604974452736ULL,
  6782456361169985536ULL,
  4485655873561051136ULL,
  9115426935197958144ULL,
};

/**************************************
 * An occupancy mask of positions     *
 * attacked by a Bishop at position   *
 * `index`. Excludes the square the   *
 * Bishop sits on and the outer edges *
 * of the board                       *
 *************************************/
unsigned long long BOccMasks[] = {
  18049651735527936ULL,
  70506452091904ULL,
  275415828992ULL,
  1075975168ULL,
  38021120ULL,
  8657588224ULL,
  2216338399232ULL,
  567382630219776ULL,
  9024825867763712ULL,
  18049651735527424ULL,
  70506452221952ULL,
  275449643008ULL,
  9733406720ULL,
  2216342585344ULL,
  567382630203392ULL,
  1134765260406784ULL,
  4512412933816832ULL,
  9024825867633664ULL,
  18049651768822272ULL,
  70515108615168ULL,
  2491752130560ULL,
  567383701868544ULL,
  1134765256220672ULL,
  2269530512441344ULL,
  2256206450263040ULL,
  4512412900526080ULL,
  9024834391117824ULL,
  18051867805491712ULL,
  637888545440768ULL,
  1135039602493440ULL,
  2269529440784384ULL,
  4539058881568768ULL,
  1128098963916800ULL,
  2256197927833600ULL,
  4514594912477184ULL,
  9592139778506752ULL,
  19184279556981248ULL,
  2339762086609920ULL,
  4538784537380864ULL,
  9077569074761728ULL,
  562958610993152ULL,
  1125917221986304ULL,
  2814792987328512ULL,
  5629586008178688ULL,
  11259172008099840ULL,
  22518341868716544ULL,
  9007336962655232ULL,
  18014673925310464ULL,
  2216338399232ULL,
  4432676798464ULL,
  11064376819712ULL,
  22137335185408ULL,
  44272556441600ULL,
  87995357200384ULL,
  35253226045952ULL,
  70506452091904ULL,
  567382630219776ULL,
  1134765260406784ULL,
  2832480465846272ULL,
  5667157807464448ULL,
  11333774449049600ULL,
  22526811443298304ULL,
  9024825867763712ULL,
  18049651735527936ULL,
};

unsigned long long magic_RMagicN[] = {
      0xa8002c000108020ULL,
      0x6c00049b0002001ULL,
      0x100200010090040ULL,
      0x2480041000800801ULL,
      0x280028004000800ULL,
      0x900410008040022ULL,
      0x280020001001080ULL,
      0x2880002041000080ULL,
      0xa000800080400034ULL,
      0x4808020004000ULL,
      0x2290802004801000ULL,
      0x411000d00100020ULL,
      0x402800800040080ULL,
      0xb000401004208ULL,
      0x2409000100040200ULL,
      0x1002100004082ULL,
      0x22878001e24000ULL,
      0x1090810021004010ULL,
      0x801030040200012ULL,
      0x500808008001000ULL,
      0xa08018014000880ULL,
      0x8000808004000200ULL,
      0x201008080010200ULL,
      0x801020000441091ULL,
      0x800080204005ULL,
      0x1040200040100048ULL,
      0x120200402082ULL,
      0xd14880480100080ULL,
      0x12040280080080ULL,
      0x100040080020080ULL,
      0x9020010080800200ULL,
      0x813241200148449ULL,
      0x491604001800080ULL,
      0x100401000402001ULL,
      0x4820010021001040ULL,
      0x400402202000812ULL,
      0x209009005000802ULL,
      0x810800601800400ULL,
      0x4301083214000150ULL,
      0x204026458e001401ULL,
      0x40204000808000ULL,
      0x8001008040010020ULL,
      0x8410820820420010ULL,
      0x1003001000090020ULL,
      0x804040008008080ULL,
      0x12000810020004ULL,
      0x1000100200040208ULL,
      0x430000a044020001ULL,
      0x280009023410300ULL,
      0xe0100040002240ULL,
      0x200100401700ULL,
      0x2244100408008080ULL,
      0x8000400801980ULL,
      0x2000810040200ULL,
      0x8010100228810400ULL,
      0x2000009044210200ULL,
      0x4080008040102101ULL,
      0x40002080411d01ULL,
      0x2005524060000901ULL,
      0x502001008400422ULL,
      0x489a000810200402ULL,
      0x1004400080a13ULL,
      0x4000011008020084ULL,
      0x26002114058042ULL,
};

unsigned long long magic_BMagicN[] = {
      0x89a1121896040240ULL,
      0x2004844802002010ULL,
      0x2068080051921000ULL,
      0x62880a0220200808ULL,
      0x4042004000000ULL,
      0x100822020200011ULL,
      0xc00444222012000aULL,
      0x28808801216001ULL,
      0x400492088408100ULL,
      0x201c401040c0084ULL,
      0x840800910a0010ULL,
      0x82080240060ULL,
      0x2000840504006000ULL,
      0x30010c4108405004ULL,
      0x1008005410080802ULL,
      0x8144042209100900ULL,
      0x208081020014400ULL,
      0x4800201208ca00ULL,
      0xf18140408012008ULL,
      0x1004002802102001ULL,
      0x841000820080811ULL,
      0x40200200a42008ULL,
      0x800054042000ULL,
      0x88010400410c9000ULL,
      0x520040470104290ULL,
      0x1004040051500081ULL,
      0x2002081833080021ULL,
      0x400c00c010142ULL,
      0x941408200c002000ULL,
      0x658810000806011ULL,
      0x188071040440a00ULL,
      0x4800404002011c00ULL,
      0x104442040404200ULL,
      0x511080202091021ULL,
      0x4022401120400ULL,
      0x80c0040400080120ULL,
      0x8040010040820802ULL,
      0x480810700020090ULL,
      0x102008e00040242ULL,
      0x809005202050100ULL,
      0x8002024220104080ULL,
      0x431008804142000ULL,
      0x19001802081400ULL,
      0x200014208040080ULL,
      0x3308082008200100ULL,
      0x41010500040c020ULL,
      0x4012020c04210308ULL,
      0x208220a202004080ULL,
      0x111040120082000ULL,
      0x6803040141280a00ULL,
      0x2101004202410000ULL,
      0x8200000041108022ULL,
      0x21082088000ULL,
      0x2410204010040ULL,
      0x40100400809000ULL,
      0x822088220820214ULL,
      0x40808090012004ULL,
      0x910224040218c9ULL,
      0x402814422015008ULL,
      0x90014004842410ULL,
      0x1000042304105ULL,
      0x10008830412a00ULL,
      0x2520081090008908ULL,
      0x40102000a0a60140ULL,
};

int initialized = 0;

void initmagic_BMagic() {
  for (int i = 0;i < 64;i++) {
    magic_BMagic[i].mask = BOccMasks[i];
    magic_BMagic[i].shift = BShift[i];
    magic_BMagic[i].magic = magic_BMagicN[i];
    int attSetCount = 1 << BShift[i];

    magic_BMagic[i].attSets = malloc(sizeof(unsigned long long)*attSetCount);
    for (int j = 0;j < attSetCount;j++) {
      magic_BMagic[i].attSets[j] = -1;
    }
    for (long j = 0;j < attSetCount;j++) {
      long bb = 0L;
      int bit = i;
      while ((bit / 8 > 1) && (bit % 8 < 6)) bit += DOWN + RIGHT;
      int s = (i - bit) / 7;
      long l = 0;
      for (;s != 0;s--, l++) {
        bb |= (j >> l & 1L) << bit;
        bit += UP + LEFT;
      }
      if ((bit / 8 < 6) && (bit % 8 > 1)) {
        bit += UP + LEFT;
        for (;(bit / 8 < 7) && (bit % 8 > 0);l++) {
          bb |= (j >> l & 1L) << bit;
          bit += UP + LEFT;
        }
      }
      bit = i;
      while ((bit / 8 > 1) && (bit % 8 > 1)) bit += DOWN + LEFT;
      s = (i - bit) / 9;
      for (;s != 0;s--, l++) {
        bb |= (j >> l & 1L) << bit;
        bit += UP + RIGHT;
      }
      if ((bit / 8 < 6) && (bit % 8 < 6)) {
        bit += UP + RIGHT;
        for (;(bit / 8 < 7) && (bit % 8 < 7);l++) {
          long t = (j >> l & 1L) << bit;
          bb |= t;
          bit += UP + RIGHT;
        }
      }
      long attSet = 0L;
      bit = i;

      while ((bit / 8 < 7) && (bit % 8 < 7)) {
        bit += UP + RIGHT;
        attSet |= 1L << bit;
        if ((bb >> bit & 1L) == 1)
          break;
      }
      bit = i;
      while ((bit / 8 < 7) && (bit % 8 > 0)) {
        bit += UP + LEFT;
        attSet |= 1L << bit;
        if ((bb >> bit & 1L) == 1)
          break;
      }
      bit = i;
      while ((bit / 8 > 0) && (bit % 8 < 7)) {
        bit += DOWN + RIGHT;
        attSet |= 1L << bit;
        if ((bb >> bit & 1L) == 1)
          break;
      }
      bit = i;
      while ((bit / 8 > 0) && (bit % 8 > 0)) {
        bit += DOWN + LEFT;
        attSet |= 1L << bit;
        if ((bb >> bit & 1L) == 1)
          break;
      }

      magic_BMagic[i].attSets[(int)((bb * magic_BMagic[i].magic) >> (64 - magic_BMagic[i].shift) & magic_ShiftMask[magic_BMagic[i].shift])] = attSet;

    }
  }
}

void initmagic_RMagic() {
  for (int i = 0;i < 64;i++) {
    magic_RMagic[i].mask = ROccMasks[i];
    magic_RMagic[i].shift = RShift[i];
    magic_RMagic[i].magic = magic_RMagicN[i];

    int attSetCount = 1 << RShift[i];

    magic_RMagic[i].attSets = malloc(sizeof(unsigned long long)*attSetCount);
    for (int j = 0;j < attSetCount;j++) {
      magic_RMagic[i].attSets[j] = -1;
    }
    for (long j = 0;j < attSetCount;j++) {
      long bb = 0L;
      int bit = i;
      while ((bit / 8 > 1)) bit += DOWN;
      int s = (i - bit) / 8;
      long l = 0;
      for (;s != 0;s--, l++) {
        bb |= (j >> l & 1L) << bit;
        bit += UP;
      }
      if ((bit / 8 < 6)) {
        bit += UP;
        for (;(bit / 8 < 7);l++) {
          bb |= (j >> l & 1L) << bit;
          bit += UP;
        }
      }
      bit = i;
      while ((bit % 8 > 1)) bit += LEFT;
      s = i - bit;
      for (;s != 0;s--, l++) {
        bb |= (j >> l & 1L) << bit;
        bit += RIGHT;
      }
      if ((bit % 8 < 6)) {
        bit += RIGHT;
        for (;(bit % 8 < 7);l++) {
          long t = (j >> l & 1L) << bit;
          bb |= t;
          bit += RIGHT;
        }
      }
      long attSet = 0L;
      bit = i;

      while ((bit / 8 < 7)) {
        bit += UP;
        attSet |= 1L << bit;
        if ((bb >> bit & 1L) == 1)
          break;
      }
      bit = i;
      while ((bit % 8 > 0)) {
        bit += LEFT;
        attSet |= 1L << bit;
        if ((bb >> bit & 1L) == 1)
          break;
      }
      bit = i;
      while ((bit % 8 < 7)) {
        bit += RIGHT;
        attSet |= 1L << bit;
        if ((bb >> bit & 1L) == 1)
          break;
      }
      bit = i;
      while ((bit / 8 > 0)) {
        bit += DOWN;
        attSet |= 1L << bit;
        if ((bb >> bit & 1L) == 1)
          break;
      }

      magic_RMagic[i].attSets[(int)((bb * magic_RMagic[i].magic) >> (64 - magic_RMagic[i].shift) & magic_ShiftMask[magic_RMagic[i].shift])] = attSet;
    }
  }
}

void magic_init() {
  if (!initialized) {
    for (int i = 0;i < 17;i++) {
      magic_ShiftMask[i] = (1 << i) - 1;
    }

    magic_RMagic = malloc(sizeof(Magic) * 64);
    magic_BMagic = malloc(sizeof(Magic) * 64);
    initmagic_BMagic();
    initmagic_RMagic();
    initialized = 1;
  }
}