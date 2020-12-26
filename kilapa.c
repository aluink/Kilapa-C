#include <stdlib.h>

#include "kilapa.h"
#include "core.h"

#define INF 300000

int eval(Board *board, LegalMoves *lms) {
    int cur = 0, other = 0;
    BBPieceIdx idx, i;
    int value;

    // Do current player
    idx = board->turn == BLACK ? 0 : 6;
    for(i = 0;i < 6;i++, idx++) {
        BitBoard bb = board->bitboards[idx];
        while(bb) {
            cur++;
            bb &= bb - 1;
        }
    }

    // Do other player
    idx %= 12;
    for(i = 0;i < 6;i++, idx++) {
        BitBoard bb = board->bitboards[idx];
        while(bb) {
            other++;
            bb &= bb - 1;
        }
    }

    value = other - cur;

    // If the game is over, return an INF value
    if (!lms->count) {
        return value > 0 ? INF : -INF;
    }

    return value + lms->count;
}

Move choose_move(Board *board) {
    int i;
    LegalMoves lms;

    get_legal_moves(board, &lms);


    for(i = 0;i < lms.count;i++) {

    }

    return lms.moves[rand() % lms.count];
}