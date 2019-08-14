#include <stdio.h>
#include <stdlib.h>

#include "board.h"

CheckerBoard * newBoard() {
	int i;
	CheckerBoard * board = malloc(sizeof(CheckerBoard));

	board->pos[0] = board->pos[7] = WROOK;
	board->pos[1] = board->pos[6] = WKNIGHT;
	board->pos[2] = board->pos[5] = WBISHOP;
	board->pos[3] = WQUEEN;
	board->pos[4] = WKING;
	board->pos[8] =
		board->pos[9]  =
		board->pos[10] =
		board->pos[11] =
		board->pos[12] = 
		board->pos[13] =
		board->pos[14] =
		board->pos[15] = WPAWN;

	for (i = 16;i < 48;i++) {
		board->pos[i] = 50;
	}

	board->pos[56] = board->pos[63] = BROOK;
	board->pos[57] = board->pos[62] = BKNIGHT;
	board->pos[58] = board->pos[61] = BBISHOP;
	board->pos[59] = BQUEEN;
	board->pos[60] = BKING;
	board->pos[48] =
		board->pos[49]  =
		board->pos[50] =
		board->pos[51] =
		board->pos[52] = 
		board->pos[53] =
		board->pos[54] =
		board->pos[55] = BPAWN;

	return board;
}


void printBoard(CheckerBoard *board) {
	for (int row = 7;row >= 0; row--) {
		printf("\n   +---+---+---+---+---+---+---+---+\n %d ", row+1);
		for (int col = 0;col < 8;col++) {
			char c;
			int piece = ((int)board->pos[row*8+col]);
		 	int type = piece % 6;
			int color = piece / 6;
			switch(type) {
				case WKING: c = 'K'; break; 
				case WQUEEN: c = 'Q'; break; 
				case WROOK: c = 'R'; break; 
				case WKNIGHT: c = 'N'; break; 
				case WBISHOP: c = 'B'; break; 
				case WPAWN: c = 'P'; break;
			}
			if (piece == 50) {
				color = 0;
				c = ' ';
			}

			printf("|%c%c ", color == 0 ? ' ' : '*', c);
			
		}	
		printf("|");
	}
	printf("\n   +---+---+---+---+---+---+---+---+");
	printf("\n     A   B   C   D   E   F   G   H\n\n");
}

