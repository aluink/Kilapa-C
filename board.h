typedef enum _Piece {
	WKING, WQUEEN, WROOK, WKNIGHT, WBISHOP, WPAWN,
	BKING, BQUEEN, BROOK, BKNIGHT, BBISHOP, BPAWN
} Piece;

typedef struct {
	int pos[64];	
} CheckerBoard;

CheckerBoard * newBoard();
void printBoard(CheckerBoard *board);
