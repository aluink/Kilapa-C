#ifndef __board_h
#define __board_h

#include <stdio.h>

typedef enum _Piece {
	EMPTY = 0,
	KING, QUEEN, ROOK, KNIGHT, BISHOP, PAWN
} Piece;

typedef enum _Color {
	BLACK = -1, NONE, WHITE
} Color;

typedef enum _BBPieceIdx {
	BKING = 0, BQUEEN, BROOK, BKNIGHT, BBISHOP, BPAWN,
	WKING, WQUEEN, WROOK, WKNIGHT, WBISHOP, WWPAWN
} BBPieceIdx;

typedef struct {
	int pos[64];
	unsigned long long bitboards[12];
	Color turn;
	int enpassant;
} Board;

typedef struct {
	char start;
	char end;
	char enpassent; // the vicim piece's row
	char capturePiece;
	char promo;
} Move;

typedef struct {
	Move moves[64];
	int count;
} LegalMoves;

typedef unsigned long long BitBoard;

Board * newBoard();
void printBoard(Board *board);
void printBBoards(Board *board);
void make_move(Board *board, Move *move);
void unmake_move(Board *board, Move *move);
void print_move(Move *move);
void magic_init();
void load_fen(Board *board, char *fen, int *error);
void get_legal_moves(Board *board, LegalMoves *lms);
void printBBoard(unsigned long long board);
int snprintMove(char * str, size_t size, Move * move);

#endif