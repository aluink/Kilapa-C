#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <fcntl.h>

#include "board.h"

#define COMMAND_COUNT 8
#define _CRT_SECURE_NO_WARNINGS

typedef struct _ClientState {
    Board *board;
    void (**command_funcs) (struct _ClientState*); 
    char **command_names;
    char *command_buffer;
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
        if (!strncmp(buffer, state->command_names[i], strlen(state->command_names[i]))) {
            state->command_funcs[i](state);
            return 0;
        }
    }

    return 1;
}

void printLMs_command(ClientState *state) {
    LegalMoves lms;
    
    get_legal_moves(state->board, &lms);

    printf("StartLMS Print:\n");
    printf("Count: %d\n", lms.count);
    for(int i = 0;i < lms.count;i++){
        char startCol = (lms.moves[i].start % (short)8) + 'a';
        char startRow = (lms.moves[i].start / (short)8) + '1';
        char endCol = (lms.moves[i].end % (short)8) + 'a';
        char endRow = (lms.moves[i].end / (short)8) + '1';
        printf("%c%c%c%c\n", startCol, startRow, endCol, endRow);
    }
    printf("EndLMS Print:\n");
}

void xboard_command(ClientState *state) {}

void set_fen(ClientState *state) {
    int error;
    char *fen = &state->command_buffer[8];
    load_fen(state->board, fen, &error);

    if (error) {
        printf("Invalid fen\n");
    }
}

int move_is_legal(Move *move, LegalMoves *moves) {
    for (int i = 0;i < moves->count;i++) {
        if (moves->moves[i].start == move->start && moves->moves[i].end == move->end)
            return 1;
    }
    return 0;
}

void test_command(ClientState *state) { }

int main(int argc, char *argv[]) {
    ClientState *state = malloc(sizeof(ClientState));
    ssize_t read_size;
    size_t buffer_size = 256;
    // LegalMoves lms;
    
    signal(SIGTERM, SIG_IGN);
    signal(SIGTERM, SIG_IGN);

    state->command_buffer = malloc(256);

    void (*funcs[COMMAND_COUNT])(ClientState *) = {
        *new_command,
        *print_commands_command,
        *printB_command,
        *printLMs_command,
        *print_command,
        *set_fen,
        *test_command,
        *xboard_command
    };

    char *command_names[COMMAND_COUNT] = {
        "new",
        "print_commands",
        "printB",
        "printLMS",
        "print",
        "loadFen",
        "test",
        "xboard"
    };

    state->command_funcs = funcs;
    state->command_names = command_names;

    magic_init();

    while(1) {
        // printf("Enter command: ");
        read_size = getline(&state->command_buffer, &buffer_size, stdin);
        state->command_buffer[read_size - 1] = (char)NULL;

        if (!strcmp(state->command_buffer, "quit")) {
            break;
        } else if (handle_command(state->command_buffer, state)) {
            // int colStart = state->command_buffer[0] - 'a';
            // int rowStart = state->command_buffer[1] - '1';
            // int colEnd = state->command_buffer[2] - 'a';
            // int rowEnd = state->command_buffer[3] - '1';

            // Move *m = malloc(sizeof(Move));
            // m->start = rowStart * 8 + colStart;
            // m->end = rowEnd * 8 + colEnd;

            // get_legal_moves(state->board, &lms);
            // if (move_is_legal(m, &lms)) {
            //     make_move(state->board, m);
            //     printBoard(state->board);
            // } else {
            //     printf("Illegal Move\n");
            // }

            continue;
        }
    }

    return 0;
}
