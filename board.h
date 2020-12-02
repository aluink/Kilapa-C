#include "magic.h"

typedef enum _Piece {
	EMPTY = 0,
	KING, QUEEN, ROOK, KNIGHT, BISHOP, PAWN
} Piece;

typedef enum _Color {
	BLACK = -1, NONE, WHITE
} Color;

typedef struct {
	int pos[64];
	unsigned long long bitboards[12];
	Color turn;
} Board;

typedef struct {
	short start;
	short end;
	short enpassent;
	short capturePiece;
} Move;

typedef struct {
	Move *moves;
	int count;
} LegalMoves;

typedef unsigned long long BitBoard;

Board * newBoard();
void printBoard(Board *board);
void printBBoards(Board *board);
void make_move(Board *board, Move *move);
void print_move(Move *move);
LegalMoves * get_legal_moves(Board *board);
void printBBoard(unsigned long long board);
