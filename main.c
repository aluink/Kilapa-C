#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "board.h"

typedef struct {
	CheckerBoard *board;
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

int main() {
	ClientState *state = malloc(sizeof(ClientState));
	char buffer[256];
	const int command_count = 2;
	void (*funcs[])(ClientState *) = {
		*print_command,
		*new_command
	};

	const char command_names[][16] = {
		"print",
		"new"
	};


	while(1) {
		scanf("%s", &buffer);
		printf("%s\n", buffer);

		if (!strcmp(buffer, "quit")) {
			break;
		} else if (!strcmp(buffer, "print_commands")) {
			for (int i = 0;i < command_count;i++) {
				command_names[i];
			}
		}

		for (int i = 0;i < 2;i++){
			if (!strcmp(buffer, command_names[i])) {
				funcs[i](state);
			}
		}
	}
	return 0;
}
