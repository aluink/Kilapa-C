#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "board.h"

#define COMMAND_COUNT 6
#define _CRT_SECURE_NO_WARNINGS

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
		char startCol = (lms->moves[i].start % (short)8) + 'a';
		char startRow = (lms->moves[i].start / (short)8) + '1';
		char endCol = (lms->moves[i].end % (short)8) + 'a';
		char endRow = (lms->moves[i].end / (short)8) + '1';
		printf("%c%c%c%c\n", startCol, startRow, endCol, endRow);
	}
	printf("EndLMS Print:\n");
}

void test_command(ClientState *state) { }

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
