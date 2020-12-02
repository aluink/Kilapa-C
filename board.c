#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

#include "board.h"

#define LEFT -1
#define RIGHT 1
#define UP 8
#define DOWN -8

#define LEGAL_PIECES 2

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

Board * newBoard() {
	int i;
	Board * board = malloc(sizeof(Board));

	board->bitboards[0] = 0x1000000000000000; // -KING
	board->bitboards[1] = 0x0800000000000000; // -QUEEN
	board->bitboards[2] = 0x8100000000002000; // -ROOK
	board->bitboards[3] = 0x4200000000000000; // -KNIGHT
	board->bitboards[4] = 0x2400000000000000; // -BISHOP
	board->bitboards[5] = 0x00FF000000000000; // -PAWN

	board->bitboards[6] = 0x10; // KING
	board->bitboards[7] = 0x8; // QUEEN
	board->bitboards[8] = 0x81; // ROOK
	board->bitboards[9] = 0x42; // KNIGHT
	board->bitboards[10] = 0x24; // BISHOP
	board->bitboards[11] = 0xFF0000; // PAWN

	board->pos[0] = board->pos[7] = ROOK;
	board->pos[1] = board->pos[6] = KNIGHT;
	board->pos[2] = board->pos[5] = BISHOP;
	board->pos[3] = QUEEN;
	board->pos[4] = KING;
	board->pos[16] =
		board->pos[17]  =
		board->pos[18] =
		board->pos[19] =
		board->pos[20] = 
		board->pos[21] =
		board->pos[22] =
		board->pos[23] = PAWN;

	for (i = 24;i < 48;i++) {
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
	for (int row = 7;row >= 0; row--) {
		printf("\n   +---+---+---+---+---+---+---+---+\n %d ", row+1);
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
	printf("\n   +---+---+---+---+---+---+---+---+");
	printf("\n     A   B   C   D   E   F   G   H\n\n");
}

void printBBoard(unsigned long long board) {
	for(int row = 7;row >= 0;row--) {
		for(int col = 0;col < 8;col++) {
			printf("%llu", board >> (row * 8 + col) & 1);
		}
		printf("\n");
	}
	printf("\n");
}

void printBBoards(Board *board) {
	for(int i = 0;i < 12;i++) {
		printf("board %s\n", piece_name(i < 6 ? -(i+1) : i - 5));
		printBBoard(board->bitboards[i]);
		printf("\n");
	}
}

void make_move(Board *board, Move *move) {
	int p = board->pos[move->end] = board->pos[move->start];
	board->pos[move->start] = EMPTY;

	int bb_idx = p < 0 ? p* -1 - 1 : p + 5;
	printf("%d %d\n", p, bb_idx);

	board->bitboards[bb_idx] &= ~(1ULL << move->start);
	board->bitboards[bb_idx] |= 1ULL << move->end;

	if (move->capturePiece > -1) {
		board->bitboards[move->capturePiece] &= ~(1ULL << move->end);
	}
}

void addAllMoves(LegalMoves *moves, int start, long mask){			
	while(mask != 0){
		int p = ffsll(mask) - 1;
		mask &= mask-1;

		moves->moves[moves->count].start = start;
		moves->moves[moves->count].end = p;
		moves->count++;
	}
}

int getBishopMoves(int attacking, LegalMoves* moves, Board *board, unsigned long long allBoard, unsigned long long otherBoard) {
	BitBoard bishops = board->bitboards[board->turn == BLACK ? 4 : 10];
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

int getKnightMoves(int attacking, LegalMoves* moves, Board *board, unsigned long long allBoard, unsigned long long otherBoard) {
	BitBoard knights = board->bitboards[board->turn == BLACK ? 3 : 9];
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

int getRookMoves(int attacking, LegalMoves* moves, Board *board, unsigned long long allBoard, unsigned long long otherBoard) {
	BitBoard rooks = board->bitboards[board->turn == BLACK ? 2 : 8];
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

// void getPawnAttack(int start, int tmp, int attacking, int * curMove, List<Move> moves, boolean promo){
// 	if(tmp == b.getEnpassantPos()){
// 		if(!attacking){
// 			attacking = 1;
// 			*curMove = 0;
// 		}
// 		moves.add(new Move(start, tmp, true));
// 	}
// 	if(b.getPos(tmp) != null && b.getPos(tmp).getColor() != b.getTurn()){
// 		if(&& !attacking){
// 			attacking = 1;
// 			*curMove = 0;
// 		}
// 		if(promo){
// 			moves.addAll(Arrays.asList(Move.promoSet(start, tmp, b.getTurn())));
// 		} else {
// 			moves.add(new Move(start, tmp));
// 		}
// 	} 
	
// }

// void getPawnMove(int start, int tmp, List<Move> moves, boolean promo){
// 	if(b.getPos(tmp) == null){
// 		if(promo){
// 			moves.addAll(Arrays.asList(Move.promoSet(start, tmp, b.getTurn())));
// 		} else {
// 			if(Math.abs(start-tmp) == 16){
// 				moves.add(new Move(start, tmp));
// 			} else {
// 				moves.add(new Move(start, tmp));
// 			}
// 		}
// 	}
// }

int getPawnMoves(int attacking, LegalMoves* moves, Board *board, unsigned long long allBoard, unsigned long long otherBoard) {
	return attacking;
	// while(pawns != 0){
	// 	int start = ffsll(pawns) - 1;
	// 	pawns &= pawns - 1;
	// 	int row = start/8;
	// 	int starting, promo;
	// 	int dir;		
	// 	if(b.getTurn() == WHITE){
	// 		dir = 8;
	// 		starting = row == 1;
	// 		promo = row == 6;
	// 	}
	// 	else{
	// 		dir = -8;
	// 		starting = row == 6;
	// 		promo = row == 1;
	// 	}
		
	// 	if(start%8 != 0)
	// 		getPawnAttack(start, start+dir+LEFT, attacking, moves, promo);
	// 	if(start%8 != 7)
	// 		getPawnAttack(start, start+dir+RIGHT, attacking, moves, promo);
		
	// 	if(!attacking){
	// 		getPawnMove(start, start+dir, moves, promo);
	// 		if(starting && b.getPos(start+dir) == null)
	// 			getPawnMove(start, start+dir+dir, moves, promo);
	// 	}
	// }		
}

int getQueenMoves(int attacking, LegalMoves* moves, Board *board, unsigned long long allBoard, unsigned long long otherBoard) {
	attacking = getBishopMoves(attacking, moves, board, allBoard, otherBoard);
	return getRookMoves(attacking, moves, board, allBoard, otherBoard);
}

int getKingMoves(int attacking, LegalMoves* moves, Board *board, unsigned long long allBoard, unsigned long long otherBoard) {
	BitBoard kings = board->bitboards[board->turn == BLACK ? 0 : 6];
	while(kings != 0){
		int start = ffsll(kings) - 1;
		kings &= kings - 1;
		unsigned long long mask = kingMasks[start];
		unsigned long long attack = mask & otherBoard;
		printBBoard(attack);
		
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

LegalMoves * get_legal_moves(Board *board) {
	int attacking = 0;
	LegalMoves *lms = malloc(sizeof(LegalMoves));
	unsigned long long allBoard = 0ULL;
	unsigned long long otherBoard = 0ULL;
	
	lms->moves = malloc(sizeof(Move)*64);
	lms->count = 0;
	
	int (*funcs[6])(int attacking, LegalMoves* moves, Board *board, unsigned long long allBoard, unsigned long long otherBoard) = {
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
			allBoard,
			otherBoard);
	}

	return lms;
}
