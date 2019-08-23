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
	int start;
	int end;
} Move;

Board * newBoard();
void printBoard(Board *board);
void printBBoards(Board *board);
void make_move(Board *board, Move *move);
