#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "board.h"
#include "core.h"
#include "kilapa.h"

#define COMMAND_COUNT 12
#define _CRT_SECURE_NO_WARNINGS

typedef struct _ClientState {
    Board *board;
    void (**command_funcs) (struct _ClientState*); 
    char **command_names;
    char *command_buffer;
    char engine_turn;
    char *moves_played[256];
    int current_move;
} ClientState;

void print_command(ClientState * state) {
    if (state->board) {
        printBoard(state->board);
    }
}

void new_command (ClientState *state) {
    state->board = newBoard();

    state->engine_turn = BLACK;
}

void print_commands_command(ClientState *state) {
    for (int i = 0;i < COMMAND_COUNT;i++) {
        printDebug("%s\n", state->command_names[i]);
    }
}

void printB_command(ClientState *state) {
    if (state->board) {
        printBBoards(state->board);
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

    printDebug("Count: %d\n", lms.count);
    for(int i = 0;i < lms.count;i++){
        char startCol = (lms.moves[i].start % (short)8) + 'a';
        char startRow = (lms.moves[i].start / (short)8) + '1';
        char endCol = (lms.moves[i].end % (short)8) + 'a';
        char endRow = (lms.moves[i].end / (short)8) + '1';
        char promo;
        switch (lms.moves[i].promo * state->board->turn) {
            case KING: promo = 'K'; break;
            case QUEEN: promo = 'Q'; break;
            case ROOK: promo = 'R'; break;
            case KNIGHT: promo = 'N'; break;
            case BISHOP: promo = 'B'; break;
            default: promo = ' '; break;
        }
        printDebug("\t%c%c%c%c%c\n", startCol, startRow, endCol, endRow, promo);
    }
    printDebug("\n");
}

void xboard_command(ClientState *state) {
    setXboardMode(TRUE);
}

void set_fen(ClientState *state) {
    int error;
    char *fen = &state->command_buffer[8];
    load_fen(state->board, fen, &error);

    if (error) {
        printDebug("Invalid fen\n");
    }
}

void test_command(ClientState *state) { }

int check_command_move(ClientState *state, Move *move) {
    LegalMoves lms;
    int colStart = state->command_buffer[0] - 'a';
    int rowStart = state->command_buffer[1] - '1';
    int colEnd = state->command_buffer[2] - 'a';
    int rowEnd = state->command_buffer[3] - '1';
    int promo = state->command_buffer[4];

    move->start = rowStart * 8 + colStart;
    move->end = rowEnd * 8 + colEnd;
    
    if (move->start < 0 || move->start > 63 || move->end < 0 || move->end > 63) {
        return 0;
    }

    if (promo > 0 && promo < 6) move->promo = promo;

    get_legal_moves(state->board, &lms);

    for (int i = 0;i < lms.count;i++) {
        if (lms.moves[i].start == move->start && lms.moves[i].end == move->end)
            return TRUE;
    }
    return FALSE;
}

void force_command(ClientState *state) {
    state->engine_turn = EMPTY;
}

void undo_command(ClientState *state) {
    
}

void sigInt(int val) {
    printDebug("Got SIGINT: %d\n", val);
}


void sigTerm(int val) {
    printDebug("Got SIGTERM: %d\n", val);
}

void engine_makemove(ClientState *state) {
    Move m;
    char buffer[8];

    m = choose_move(state->board);
    make_move(state->board, &m);
    write(STDOUT_FILENO, "move ", 5);
    write(STDOUT_FILENO, buffer, snprintMove(buffer, 8, &m) + 1);
    write(STDERR_FILENO, "\n", 1);
}

void go_command(ClientState *state) {
    state->engine_turn = state->board->turn;
    engine_makemove(state);
}

void quit_command(ClientState *state) {
    exit(0);
}

void init_client_state(ClientState *state) {
    void (*funcs[COMMAND_COUNT])(ClientState *) = {
        *new_command,
        *print_commands_command,
        *printB_command,
        *printLMs_command,
        *print_command,
        *set_fen,
        *test_command,
        *xboard_command,
        *go_command,
        *force_command,
        *undo_command,
        *quit_command
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
        "go",
        "force",
        "undo",
        "quit"
    };

    state->command_buffer = malloc(256);
    state->board = NULL;
    state->command_funcs = funcs;
    state->command_names = command_names;
    state->engine_turn = 0;
    state->current_move = 0;
}

int main(int argc, char *argv[]) {
    ClientState *state = malloc(sizeof(ClientState));
    Move move;
    
    signal(SIGTERM, sigTerm);
    signal(SIGINT, sigInt);    

    init_client_state(state);

    magic_init();

    write(STDOUT_FILENO, "feature debug=1\n", 16);

    while(1) {
        if (state->board != NULL && state->engine_turn == state->board->turn) {
            engine_makemove(state);
            continue;
        }
        printDebug("Enter command: \n");
        fgets(state->command_buffer, 256, stdin);
        state->command_buffer[strlen(state->command_buffer) - 1] = '\0';
        if (handle_command(state->command_buffer, state)) {
            continue;
        } else if (state->board && check_command_move(state, &move)) {
            make_move(state->board, &move);
            if (!getXboardMode()) printBoard(state->board);
            continue;
        } else {
            write(STDOUT_FILENO, "# ignoring: ", 12);
            write(STDOUT_FILENO, state->command_buffer, strlen(state->command_buffer));
            write(STDOUT_FILENO, "\n", 1);    
        }
    }

    return 0;
}
