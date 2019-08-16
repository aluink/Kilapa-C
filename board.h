typedef enum _Piece {
	BKING = -6, BQUEEN, BROOK, BKNIGHT, BBISHOP, BPAWN,
	EMPTY,
	WKING, WQUEEN, WROOK, WKNIGHT, WBISHOP, WPAWN
} Piece;

typedef struct {
	int pos[64];	
} Board;

typedef struct {
	int start;
	int end;
} Move;

Board * newBoard();
void printBoard(Board *board);
void make_move(Board *board, Move *move);
