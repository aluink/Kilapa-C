#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

#include "board.h"
#include "core.h"

#define LEFT -1
#define RIGHT 1
#define UP 8
#define DOWN -8

#define LEGAL_PIECES 2

#define PIECE_TO_IDX(p,c) (p + (c == BLACK ? - 1 : 5))

unsigned long long kingMasks [] = {
		770L,
		1797L,
		3594L,
		7188L,
		14376L,
		28752L,
		57504L,
		49216L,
		197123L,
		460039L,
		920078L,
		1840156L,
		3680312L,
		7360624L,
		14721248L,
		12599488L,
		50463488L,
		117769984L,
		235539968L,
		471079936L,
		942159872L,
		1884319744L,
		3768639488L,
		3225468928L,
		12918652928L,
		30149115904L,
		60298231808L,
		120596463616L,
		241192927232L,
		482385854464L,
		964771708928L,
		825720045568L,
		3307175149568L,
		7718173671424L,
		15436347342848L,
		30872694685696L,
		61745389371392L,
		123490778742784L,
		246981557485568L,
		211384331665408L,
		846636838289408L,
		1975852459884544L,
		3951704919769088L,
		7903409839538176L,
		15806819679076352L,
		31613639358152704L,
		63227278716305408L,
		54114388906344448L,
		216739030602088448L,
		505818229730443264L,
		1011636459460886528L,
		2023272918921773056L,
		4046545837843546112L,
		8093091675687092224L,
		-2260560722335367168L,
		-4593460513685372928L,
		144959613005987840L,
		362258295026614272L,
		724516590053228544L,
		1449033180106457088L,
		2898066360212914176L,
		5796132720425828352L,
		-6854478632857894912L,
		4665729213955833856L
};

unsigned long long knightMasks [] = {
		132096L,
		329728L,
		659712L,
		1319424L,
		2638848L,
		5277696L,
		10489856L,
		4202496L,
		33816580L,
		84410376L,
		168886289L,
		337772578L,
		675545156L,
		1351090312L,
		2685403152L,
		1075839008L,
		8657044482L,
		21609056261L,
		43234889994L,
		86469779988L,
		172939559976L,
		345879119952L,
		687463207072L,
		275414786112L,
		2216203387392L,
		5531918402816L,
		11068131838464L,
		22136263676928L,
		44272527353856L,
		88545054707712L,
		175990581010432L,
		70506185244672L,
		567348067172352L,
		1416171111120896L,
		2833441750646784L,
		5666883501293568L,
		11333767002587136L,
		22667534005174272L,
		45053588738670592L,
		18049583422636032L,
		145241105196122112L,
		362539804446949376L,
		725361088165576704L,
		1450722176331153408L,
		2901444352662306816L,
		5802888705324613632L,
		-6913025356609880064L,
		4620693356194824192L,
		288234782788157440L,
		576469569871282176L,
		1224997833292120064L,
		2449995666584240128L,
		4899991333168480256L,
		-8646761407372591104L,
		1152939783987658752L,
		2305878468463689728L,
		1128098930098176L,
		2257297371824128L,
		4796069720358912L,
		9592139440717824L,
		19184278881435648L,
		38368557762871296L,
		4679521487814656L,
		9077567998918656L
};


#define LEFT -1
#define RIGHT 1
#define UP 8
#define DOWN -8

typedef struct _Magic {
  unsigned long long mask;
  unsigned long int shift;
  unsigned long long magic;
  unsigned long long *attSets;
} Magic;

Magic magic_RMagic[64];
Magic magic_BMagic[64];

unsigned long long magic_ShiftMask[] = {
  0x0,0x1,0x3,0x7,0xF,0x1F,0x3F,0x7F,0xFF,
  0x1FF,0x3FF,0x7FF,0xFFF,0x1FFF,0x3FFF,0x7FFF,0xFFFF
};

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

    initmagic_BMagic();
    initmagic_RMagic();
    initialized = 1;
  }
}

Board * newBoard() {
	int i, error;
	Board * board = malloc(sizeof(Board));

  load_fen(board, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w", &error);
  return board;


	board->bitboards[0] = 0x1000000000000000; // -KING
	board->bitboards[1] = 0x0800000000000000; // -QUEEN
	board->bitboards[2] = 0x8100000000000000; // -ROOK
	board->bitboards[3] = 0x4200000000000000; // -KNIGHT
	board->bitboards[4] = 0x2400000000000000; // -BISHOP
	board->bitboards[5] = 0x00FF000000000000; // -PAWN

	board->bitboards[6] = 0x10; // KING
	board->bitboards[7] = 0x8; // QUEEN
	board->bitboards[8] = 0x81; // ROOK
	board->bitboards[9] = 0x42; // KNIGHT
	board->bitboards[10] = 0x24; // BISHOP
	board->bitboards[11] = 0xFF00; // PAWN

	board->pos[0] = board->pos[7] = ROOK;
	board->pos[1] = board->pos[6] = KNIGHT;
	board->pos[2] = board->pos[5] = BISHOP;
	board->pos[3] = QUEEN;
	board->pos[4] = KING;
	board->pos[8] =
		board->pos[9]  =
		board->pos[10] =
		board->pos[11] =
		board->pos[12] = 
		board->pos[13] =
		board->pos[14] =
		board->pos[15] = PAWN;

	for (i = 16;i < 48;i++) {
		board->pos[i] = EMPTY;
	}

	board->pos[56] = board->pos[63] = -ROOK;
	board->pos[57] = board->pos[62] = -KNIGHT;
	board->pos[58] = board->pos[61] = -BISHOP;
	board->pos[59] = -QUEEN;
	board->pos[60] = -KING;
	board->pos[48] =
		board->pos[49]  =
		board->pos[50] =
		board->pos[51] =
		board->pos[52] = 
		board->pos[53] =
		board->pos[54] =
		board->pos[55] = -PAWN;

	board->turn = WHITE;
	return board;
}

void set_pos(Board *board, int pos, int piece, int color) {
  board->pos[pos] = piece * color;
  for (int i = 0;i < 12;i++) {
    board->bitboards[i] &= ~(1ULL << pos);
  }

  board->bitboards[piece + (color == BLACK ? -1 : 5)] |= 1ULL << pos;

}

void load_fen(Board *board, char * fen, int *error) {
  *error = 0;
  int row = 7, col = 0, idx = 0;
  char c;

  printDebug("loading fen: %s\n", fen);

  for (int i = 0;i < 64;i++) {
    board->pos[i] = EMPTY;
  }

  for (int i = 0;i < 12;i++) {
    board->bitboards[i] = 0ULL;
  }

  for (;row >= 0;row--) {
    for (col = 0;col < 8;) {
      c = fen[idx++];
      if (c >= '1' && c <= '8') {
        col += c - '0';
        continue;
      }

      switch (c) {
        case 'k': set_pos(board, row * 8 + col++, KING, BLACK); break;
        case 'q': set_pos(board, row * 8 + col++, QUEEN, BLACK); break;
        case 'r': set_pos(board, row * 8 + col++, ROOK, BLACK); break;
        case 'n': set_pos(board, row * 8 + col++, KNIGHT, BLACK); break;
        case 'b': set_pos(board, row * 8 + col++, BISHOP, BLACK); break;
        case 'p': set_pos(board, row * 8 + col++, PAWN, BLACK); break;
        case 'K': set_pos(board, row * 8 + col++, KING, WHITE); break;
        case 'Q': set_pos(board, row * 8 + col++, QUEEN, WHITE); break;
        case 'R': set_pos(board, row * 8 + col++, ROOK, WHITE); break;
        case 'N': set_pos(board, row * 8 + col++, KNIGHT, WHITE); break;
        case 'B': set_pos(board, row * 8 + col++, BISHOP, WHITE); break;
        case 'P': set_pos(board, row * 8 + col++, PAWN, WHITE); break;
      }
    }
    c = fen[idx++];

    if (!(row == 0 && c == ' ') && !(row >= 0 && c == '/')) {
      printDebug("idx: %d\nchar: %c\nrow: %d, col: %d\n", idx, c, row, col);
      *error = 1;
      return;
    }
  }

  if (fen[idx] == 'w') {
    board->turn = WHITE;
  } else {
    board->turn = BLACK;
  }

  printBoard(board);

}

const char * piece_name(int piece) {
	int color = piece > 0 ? 1 : -1;
	switch(piece * color) {
		case KING: return color > 0 ? "WKING" : "BKING";
		case QUEEN: return color > 0 ? "WQUEEN" : "BQUEEN";
		case BISHOP: return color > 0 ? "WBISHOP" : "BBISHOP";
		case KNIGHT: return color > 0 ? "WKNIGHT" : "BKNIGHT";
		case ROOK: return color > 0 ? "WROOK" : "BROOK";
		case PAWN: return color > 0 ? "WPAWN" : "BPAWN";
		default: return "UKNOWN";
	}
}

void printBoard(Board *board) {
  printf("# %s to move\n", board->turn == WHITE ? "WHITE" : "BLACK");
	for (int row = 7;row >= 0; row--) {
		printf("\n#   +---+---+---+---+---+---+---+---+\n# %d ", row+1);
		for (int col = 0;col < 8;col++) {
			char c;
			int piece = ((int)board->pos[row*8+col]);
			int color = piece < 0 ? -1 : 1;
			switch(piece * color) {
				case KING: c = 'K'; break; 
				case QUEEN: c = 'Q'; break; 
				case ROOK: c = 'R'; break; 
				case KNIGHT: c = 'N'; break; 
				case BISHOP: c = 'B'; break; 
				case PAWN: c = 'P'; break;
				default: c = ' '; break;
			}

			printf("|\033[%sm %c \033[0m", color < 0 ? "1;31" : "01;36", c);
			
		}	
		printf("|");
	}
	printf("\n#   +---+---+---+---+---+---+---+---+");
	printf("\n#     A   B   C   D   E   F   G   H\n\n");
  fflush(stdout);
}

void printBBoard(unsigned long long board) {
	for(int row = 7;row >= 0;row--) {
    printf("# ");
		for(int col = 0;col < 8;col++) {
			printf("%llu", board >> (row * 8 + col) & 1);
		}
		printf("\n");
	}
	printf("\n");
  fflush(stdout);
}

void printBBoards(Board *board) {
	for(int i = 0;i < 12;i++) {
		printf("board %s\n", piece_name(i < 6 ? -(i+1) : i - 5));
		printBBoard(board->bitboards[i]);
		printf("\n");
	}
  fflush(stdout);
}

void make_move(Board *board, Move *move) {
	int destPiece = board->pos[(int)move->end];
	int movingPiece = board->pos[(int)move->end] = board->pos[(int)move->start];
	board->pos[(int)move->start] = EMPTY;

	int bb_idx = movingPiece < 0 ? -movingPiece - 1 : movingPiece + 5;
	
	board->bitboards[bb_idx] &= ~(1ULL << move->start);
	board->bitboards[bb_idx] |= 1ULL << move->end;

  if (destPiece != EMPTY) {
    bb_idx = destPiece < 0 ? -destPiece - 1 : destPiece + 5;
    board->bitboards[bb_idx] &= ~(1ULL << move->end);
  }

  board->turn *= -1;

	// if (move->capturePiece > -1) {
	// 	board->bitboards[move->capturePiece] &= ~(1ULL << move->end);
	// }
}

void addAllMoves(LegalMoves *moves, int start, long mask){			
	while(mask != 0){
		int p = ffsll(mask) - 1;
		mask &= mask-1;

		moves->moves[moves->count].start = start;
		moves->moves[moves->count].end = p;
		moves->moves[moves->count].enpassent = -1;
		moves->moves[moves->count].promo = 0;
		moves->count++;
	}
}

int getBishopMoves(int attacking, LegalMoves* moves, Board *board, unsigned long long bishops, unsigned long long allBoard, unsigned long long otherBoard) {
	while(bishops != 0) {
		int start = ffsll(bishops) - 1;
		bishops &= bishops - 1;
		Magic m = magic_BMagic[start];
		unsigned long long occ = allBoard & m.mask;
		occ *= m.magic;
		occ >>= 64 - m.shift;
		occ &= magic_ShiftMask[m.shift];
		occ = m.attSets[(int)occ];
		unsigned long long attack = occ & otherBoard;
		

		if(attack != 0 && !attacking){
			attacking = 1;
			moves->count = 0;
		}
		
		if(!attacking){
			addAllMoves(moves, start, occ & ~allBoard);
		} else if(attack != 0){      
			addAllMoves(moves, start, attack);
		}
	}
	return attacking;
}

int getKnightMoves(int attacking, LegalMoves* moves, Board *board, unsigned long long knights, unsigned long long allBoard, unsigned long long otherBoard) {
	while(knights != 0){
		int start = ffsll(knights) - 1;
		knights &= knights - 1;
		unsigned long long mask = knightMasks[start];
		unsigned long long attack = mask & otherBoard;
		
		if(attack != 0 && !attacking){
			attacking = 1;
			moves->count = 0;
		}
		
		if(!attacking){
			addAllMoves(moves, start, mask & ~allBoard);
		} else if(attack != 0){
			addAllMoves(moves, start, attack);
		}
	}
	return attacking;
}

int getRookMoves(int attacking, LegalMoves* moves, Board *board, unsigned long long rooks, unsigned long long allBoard, unsigned long long otherBoard) {
	while(rooks != 0){
		int start = ffsll(rooks) - 1;
		rooks &= rooks - 1;
		Magic m = magic_RMagic[start];
		unsigned long long occ = allBoard & m.mask;
		occ *= m.magic;
		occ >>= 64 - m.shift;
		occ &= magic_ShiftMask[m.shift];
		occ = m.attSets[(int)occ];
		unsigned long long attack = occ & otherBoard;
		
		if(attack != 0 && !attacking){
			attacking = 1;
			moves->count = 0;
		}
		
		if(!attacking){
			addAllMoves(moves, start, occ & ~allBoard);
		} else if(attack != 0){
			addAllMoves(moves, start, attack);
		}
	}

	return attacking;
}

int getPawnAttack(int start, int tmp, Board *board, int attacking, LegalMoves * moves, int promo){
	int i;
  if(tmp == board->enpassant) {
		if(!attacking){
			attacking = 1;
			moves->count = 0;
		}
		moves->moves[moves->count].start = start;
		moves->moves[moves->count].end = tmp;
		moves->moves[moves->count].enpassent = -1;
		moves->moves[moves->count].promo = 0;
		moves->count++;

    return attacking;
	}

	if(board->pos[tmp] != EMPTY && board->pos[tmp] * board->turn < 0){
		if(!attacking){
			attacking = 1;
			moves->count = 0;
		}

		if(promo){
      for(i= 1;i <= 5;i++) {
        moves->moves[moves->count].start = start;
        moves->moves[moves->count].end = tmp;
		moves->moves[moves->count].enpassent = -1;
        moves->moves[moves->count].promo = i * board->turn;
        moves->count++;
      }
		} else {
		moves->moves[moves->count].start = start;
		moves->moves[moves->count].end = tmp;
		moves->moves[moves->count].enpassent = -1;
		moves->moves[moves->count].promo = 0;
		moves->count++;
		}
	}

  return attacking;
}

void getPawnMove(int start, int tmp, Board *board, LegalMoves * moves, int promo){
	if(board->pos[tmp] == EMPTY){
		if(promo){
      for(int i = 1;i <= 5;i++) {
        moves->moves[moves->count].start = start;
        moves->moves[moves->count].end = tmp;
		moves->moves[moves->count].enpassent = -1;
        moves->moves[moves->count].promo = i * board->turn;
        moves->count++;
      }
		} else {
      moves->moves[moves->count].start = start;
      moves->moves[moves->count].end = tmp;
	  moves->moves[moves->count].enpassent = -1;
	  moves->moves[moves->count].promo = 0;
	  
      moves->count++;
		}
	}
}

int getPawnMoves(int attacking, LegalMoves* moves, Board *board, unsigned long long pawns, unsigned long long allBoard, unsigned long long otherBoard) {
  int promo, start;
  int dir, row;		
  int startingRow, promoRow;

  dir = board->turn == BLACK ? -8 : 8;
  startingRow = board->turn == BLACK ? 6 : 1;
  promoRow = board->turn == BLACK ? 1 : 6;

	while(pawns != 0){
		start = ffsll(pawns) - 1;
		pawns &= pawns - 1;
		row = start / 8;

    promo = row == promoRow;
		
		if(start%8 != 0) // attack to the left
			attacking = getPawnAttack(start, start+dir+LEFT, board, attacking, moves, promo);
		if(start%8 != 7) // attack to the right
			attacking = getPawnAttack(start, start+dir+RIGHT, board, attacking, moves, promo);
		
		if(!attacking){
			getPawnMove(start, start+dir, board, moves, promo);
			if(row == startingRow && board->pos[start+dir] == EMPTY)
				getPawnMove(start, start+dir+dir, board, moves, promo);
		}
	}

  return attacking;
}

int getQueenMoves(int attacking, LegalMoves* moves, Board *board, unsigned long long queens, unsigned long long allBoard, unsigned long long otherBoard) {
	attacking = getBishopMoves(attacking, moves, board, queens, allBoard, otherBoard);
	return getRookMoves(attacking, moves, board, queens, allBoard, otherBoard);
}

int getKingMoves(int attacking, LegalMoves* moves, Board *board, unsigned long long kings, unsigned long long allBoard, unsigned long long otherBoard) {
	while(kings != 0){
		int start = ffsll(kings) - 1;
		kings &= kings - 1;
		unsigned long long mask = kingMasks[start];
    unsigned long long attack = mask & otherBoard;
    
		if(attack != 0 && !attacking){
			attacking = 1;
			moves->count = 0;
		}
		
		if(!attacking){
			addAllMoves(moves, start, mask & ~allBoard);
		} else if(attack != 0){
			addAllMoves(moves, start, attack);
		}
	}
	return attacking;
}

void get_legal_moves(Board *board, LegalMoves *lms) {
	int attacking = 0;
	unsigned long long allBoard = 0ULL;
	unsigned long long otherBoard = 0ULL;
	
	lms->count = 0;
	
	int (*funcs[6])(int attacking, LegalMoves* moves, Board *board, unsigned long long pieces, unsigned long long allBoard, unsigned long long otherBoard) = {
		getKingMoves,
		getQueenMoves,
		getRookMoves,
		getKnightMoves,
		getBishopMoves,
		getPawnMoves
	};


	for(int i = 0;i < 12;i++) {
		allBoard |= board->bitboards[i];
		
		if ((i < 6 && board->turn == WHITE) || (i >= 6 && board->turn == BLACK)) {
			otherBoard |= board->bitboards[i];
		}
	}

	for(int i = 0;i < 6;i++) {
		attacking = funcs[i](
			attacking,
			lms,
			board,
      board->bitboards[i + (board->turn == BLACK ? 0 : 6)],
			allBoard,
			otherBoard);
	}
}

int snprintMove(char * str, size_t size, Move * move) {
  return snprintf(str, size, "%c%c%c%c",
    move->start % 8 + 'a',
    move->start / 8 + '1',
    move->end % 8 + 'a',
    move->end / 8 + '1'
  );
}