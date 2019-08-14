#include <stdio.h>

#include "board.h"

int main() {
	CheckerBoard *board = newBoard();

	printBoard(board);

	return 0;
}
