#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "board.h"

#define COMMAND_COUNT 6
#define _CRT_SECURE_NO_WARNINGS

unsigned long long foo[] = {
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

typedef struct _ClientState {
	Board *board;
	void (**command_funcs) (struct _ClientState*); 
	char **command_names;
} ClientState;

void print_command(ClientState * state) {
	if (state->board) {
		printBoard(state->board);
	} else {
		printf("No board to print\n");
	}
}

void new_command (ClientState *state) {
	state->board = newBoard();
}

void print_commands_command(ClientState *state) {
	for (int i = 0;i < COMMAND_COUNT;i++) {
		printf("%s\n", state->command_names[i]);
	}
}

void printB_command(ClientState *state) {
	if (state->board) {
		printBBoards(state->board);
	} else {
		printf("No board to print\n");
	}
}

int handle_command(char *buffer, ClientState *state) {
	for (int i = 0;i < COMMAND_COUNT;i++){
		if (!strcmp(buffer, state->command_names[i])) {
			state->command_funcs[i](state);
			return 0;
		}
	}

	return 1;
}

void printLMs_command(ClientState *state) {
	LegalMoves *lms = get_legal_moves(state->board);

	printf("StartLMS Print:\n");
	for(int i = 0;i < lms->count;i++){
		char startCol = (lms->moves[i].start % 8) + 'a';
		char startRow = (lms->moves[i].start / 8) + '1';
		char endCol = (lms->moves[i].end % 8) + 'a';
		char endRow = (lms->moves[i].end / 8) + '1';
		printf("%c%c%c%c\n", startCol, startRow, endCol, endRow);
	}
	printf("EndLMS Print:\n");
}

void test_command(ClientState *state) {
	for(int i = 0;i < 64;i++) {
		for(int row = 7;row >= 0;row--) {
			for(int col = 0;col < 8;col++) {
				printf("%llu", foo[i] >> (row * 8 + col) & 1);
			}
			printf("\n");
		}
		printf("\n");
	}
}

int main() {
	ClientState *state = malloc(sizeof(ClientState));
	char *buffer = malloc(256);
	ssize_t read_size;
	size_t buffer_size = 256;

	void (*funcs[COMMAND_COUNT])(ClientState *) = {
		*print_command,
		*new_command,
		*print_commands_command,
		*printB_command,
		*printLMs_command,
		*test_command
	};

	char *command_names[COMMAND_COUNT] = {
		"print",
		"new",
		"print_commands",
		"printB",
		"printLMS",
		"test"
	};

	state->command_funcs = funcs;
	state->command_names = command_names;

	magic_init();

	while(1) {
		printf("Enter command: ");
		read_size = getline(&buffer, &buffer_size, stdin);
		buffer[read_size - 1] = (char)NULL;

		if (!strcmp(buffer, "quit")) {
			break;
		} else if (handle_command(buffer, state)) {
			int colStart = buffer[0] - 'a';
			int rowStart = buffer[1] - '1';
			int colEnd = buffer[2] - 'a';
			int rowEnd = buffer[3] - '1';

			Move *m = malloc(sizeof(Move));
			m->start = rowStart * 8 + colStart;
			m->end = rowEnd * 8 + colEnd;
			make_move(state->board, m);
			printBoard(state->board);
			continue;
		}
	}

	return 0;
}
