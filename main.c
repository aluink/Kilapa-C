#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "board.h"

#define COMMAND_COUNT 3

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

int handle_command(char *buffer, ClientState *state) {
	for (int i = 0;i < COMMAND_COUNT;i++){
		if (!strcmp(buffer, state->command_names[i])) {
			state->command_funcs[i](state);
			return 0;
		}
	}

	return 1;
}

int main() {
	ClientState *state = malloc(sizeof(ClientState));
	char buffer[256];
	void (*funcs[COMMAND_COUNT])(ClientState *) = {
		*print_command,
		*new_command,
		*print_commands_command
	};

	char *command_names[COMMAND_COUNT] = {
		"print",
		"new",
		"print_commands"
	};

	state->command_funcs = funcs;
	state->command_names = command_names;

	while(1) {
		printf("Enter command: ");
		scanf("%s", &buffer);

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
