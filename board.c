#include <stdio.h>
#include <stdlib.h>

#include "board.h"

Board * newBoard() {
	int i;
	Board * board = malloc(sizeof(Board));

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

			printf("|%c%c ", color < 0 ? '*' : ' ', c);
			
		}	
		printf("|");
	}
	printf("\n   +---+---+---+---+---+---+---+---+");
	printf("\n     A   B   C   D   E   F   G   H\n\n");
}

void printBBoards(Board *board) {
	for(int i = 0;i < 12;i++) {
		printf("board %s\n", piece_name(i < 6 ? -(i+1) : i - 5));
		for(int row = 7;row >= 0;row--) {
			for(int col = 0;col < 8;col++) {
				printf("%llu", board->bitboards[i] >> (row * 8 + col) & 1);
			}
			printf("\n");
		}
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
}

Move * get_legal_moves(Board *board) {
	int start_idx = board->turn == BLACK ? 0 : 1;
	unsigned long long pieces = 0;
	for(int i = 0;i < 6;i++){
		pieces |= board->bitboards[i + start_idx];
	}

	while(pieces > 0) {
		pieces &= pieces - 1;

	}
	return NULL;
}

