#include "definitions.h"
#include "globals.h"
#include "init.h"
#include "macros.h"
#include "stdio.h"
#include "stdlib.h"

void ResetBoard(board_representation *pos) {
    int index = 0;

    for (index = 0; index < BRD_SQ_NUM; index++) {
        pos -> pieces[index] = OFFBOARD;
    }

    for (index = 0; index < 64; index++) {
        pos -> pieces[SQUARE64TOSQUARE120[index]] = EMPTY;
    }

    for (index = 0; index < 3; index++) {
        pos -> majorPieces[index] = 0;
        pos -> minorPieces[index] = 0;
        pos -> nonPawnPieces[index] = 0;
        pos -> pawns[index] = 0ULL;
    }    

    for (index = 0; index < 13; index++) {
        pos -> pieceNumber[index] = 0;
    }

    pos -> kingSquare[WHITE] = pos -> kingSquare[BLACK] = NO_SQ;

    pos -> side = BOTH;
    pos -> enPassant = NO_SQ;
    pos -> fiftyMoveRule = 0;

    pos -> ply = 0;
    pos -> histPly = 0;

    pos -> castlePermission = 0;

    pos -> posKey = 0;
}