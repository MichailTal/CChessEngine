#include "definitions.h"
#include "globals.h"
#include "init.h"
#include "macros.h"
#include "stdio.h"
#include "stdlib.h"


U64 Rand_64() {
    return ((U64)rand() + 
            ((U64)rand() << 15) + 
            ((U64)rand() << 30) + 
            ((U64)rand() << 45) + 
            (((U64)rand() & 0xf) << 60));
}


U64 GeneratePosKey(const board_representation *pos) {

    int square = 0;
    U64 finalKey = 0;
    int piece = EMPTY;

    // pieces
    for(square = 0; square < BRD_SQ_NUM; square++) {
        piece = pos->pieces[square];
        if(piece != NO_SQ && piece != EMPTY) {
            ASSERT(piece >= wP && piece <= bN);
            finalKey ^= PieceKey[piece][square];
        }
    }

    // Side to Move
    if(pos->side == WHITE) {
        finalKey ^= SideKey;
    }

    // En Passant eligable squares
    if(pos->enPassant != NO_SQ) {
        ASSERT(pos->enPassant >= 0 && pos->enPassant < BRD_SQ_NUM);
        finalKey ^= PieceKey[EMPTY][pos->enPassant];
    }

    ASSERT(pos->castlePermission >= 0 && pos-> castlePermission <= 15);

    finalKey ^= CastleKeys[pos->castlePermission];

    return finalKey;
}