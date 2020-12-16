#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include <fcntl.h>

#include "board.h"
#include "core.h"
#include "kilapa.h"

#define COMMAND_COUNT 9
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
            return 1;
        }
    }

    return 0;
}

void printLMs_command(ClientState *state) {
    LegalMoves lms;
    
    get_legal_moves(state->board, &lms);

    printf("Count: %d\n", lms.count);
    for(int i = 0;i < lms.count;i++){
        char startCol = (lms.moves[i].start % (short)8) + 'a';
        char startRow = (lms.moves[i].start / (short)8) + '1';
        char endCol = (lms.moves[i].end % (short)8) + 'a';
        char endRow = (lms.moves[i].end / (short)8) + '1';
        printf("\t%c%c%c%c\n", startCol, startRow, endCol, endRow);
    }
    printf("\n");
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

void test_command(ClientState *state) { }

int check_command_move(ClientState *state, Move *move) {
    LegalMoves lms;
    int colStart = state->command_buffer[0] - 'a';
    int rowStart = state->command_buffer[1] - '1';
    int colEnd = state->command_buffer[2] - 'a';
    int rowEnd = state->command_buffer[3] - '1';

    move->start = rowStart * 8 + colStart;
    move->end = rowEnd * 8 + colEnd;

    if (move->start < 0 || move->start > 63 || move->end < 0 || move->end > 63) {
        return 0;
    }

    get_legal_moves(state->board, &lms);

    for (int i = 0;i < lms.count;i++) {
        if (lms.moves[i].start == move->start && lms.moves[i].end == move->end)
            return TRUE;
    }
    return FALSE;
}

void go_command(ClientState *state) {
    LegalMoves lms;
    Move *m;
    get_legal_moves(state->board, &lms);

    m = choose_move(state->board, &lms);

    make_move(state->board, m);
    printBoard(state->board);
}

int main(int argc, char *argv[]) {
    ClientState *state = malloc(sizeof(ClientState));
    ssize_t read_size;
    size_t buffer_size = 256;
    Move move;
    
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
        *xboard_command,
        *go_command
    };

    char *command_names[COMMAND_COUNT] = {
        "new",
        "print_commands",
        "printB",
        "printLMS",
        "print",
        "loadFen",
        "test",
        "xboard",
        "go"
    };

    state->command_funcs = funcs;
    state->command_names = command_names;

    magic_init();

    while(1) {
        printf("Enter command: ");
        read_size = getline(&state->command_buffer, &buffer_size, stdin);
        state->command_buffer[read_size - 1] = 0;

        if (!strcmp(state->command_buffer, "quit")) {
            break;
        } else if (handle_command(state->command_buffer, state)) {
            continue;
        } else if (check_command_move(state, &move)) {
            make_move(state->board, &move);
            printBoard(state->board);
            continue;
        } else {
            printDebug("Illegal move\n");
        }
    }

    return 0;
}
