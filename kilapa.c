#include "kilapa.h"
#include "core.h"

Move * choose_move(Board *board, LegalMoves *lms) {
    return &lms->moves[getRandom() % lms->count];
}