#include "definitions.h"
#include "globals.h"
#include "init.h"
#include "macros.h"
#include "stdio.h"
#include "stdlib.h"

const int CastlePerm[120] = {
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
    15, 13, 15, 15, 15, 12, 15, 15, 14, 15,
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
    15,  7, 15, 15, 15,  3, 15, 15, 11, 15,
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15
};


static void ClearPiece(const int square, board_representation *pos) {

    ASSERT(SqOnBoard(square));

    int piece = pos -> pieces[square];

    ASSERT(PieceValid(piece));

    int colour = PieceColour[piece];
    int index = 0;
    int t_pieceNumber = -1;

    HASH_PCE(piece, square);

    pos -> pieces[square] = EMPTY;
    pos -> material[colour] -= PieceValue[piece];

    if (PieceBig[piece]) {
        pos -> nonPawnPieces[colour]--;

        if (PieceMajor[piece]) {
            pos -> minorPieces[colour]--;
        }
        if (PieceMinor[piece]) {
            pos -> minorPieces[colour]--;
        }
    } else {
        CLRBIT(pos -> pawns[colour], SQ64(square));
        CLRBIT(pos -> pawns[BOTH], SQ64(square));
    }

    for (index = 0; index < pos -> pieceNumber[piece]; ++index) {
        if (pos -> pieceList[piece][index] == square) {
            t_pieceNumber = index;
            break;
        }
    }

    ASSERT(t_pieceNumber != -1);

    pos -> pieceNumber[piece]--;
    pos -> pieceList[piece][t_pieceNumber]  = pos -> pieceList[piece] [pos -> pieceNumber[piece]];
}

static void AddPiece(const int square, board_representation *pos, const int piece) {

    ASSERT(PieceValid(piece));
    ASSERT(SqOnBoard(square));

    int colour = PieceColour[piece];

    HASH_PCE(piece, square);

    pos -> pieces[square] = piece;

    if (PieceBig[piece]) {
        pos -> nonPawnPieces[colour]++;

        if (PieceMajor[piece]) {
            pos -> majorPieces[colour]++;
        }
        if (PieceMajor[piece]) {
            pos -> minorPieces[colour]++;
        }
    } else {
        SETBIT(pos -> pawns[colour], SQ64(square));
        SETBIT(pos -> pawns[BOTH], SQ64(square));
    }

    pos -> material[colour] += PieceValue[piece];
    pos -> pieceList[piece] [pos -> pieceNumber[piece]++] = square;
}

static void MovePiece(const int from, const int to, board_representation *pos) {

    ASSERT(SqOnBoard(from));
    ASSERT(SqOnBoard(to));

    int index = 0;
    int piece = pos -> pieces[from];
    int colour = PieceColour[piece];

#ifdef DEBUG
    int t_PieceNum = FALSE;
#endif

    HASH_PCE(piece, from);
    pos -> pieces[from] = EMPTY;

    HASH_PCE(piece, to);
    pos -> pieces[to] = piece;

    // e.g. it is a pawn
    if(!PieceBig[piece]) {
        CLRBIT(pos -> pawns[colour], SQ64(from));
        CLRBIT(pos -> pawns[BOTH], SQ64(from));
        SETBIT(pos -> pawns[colour], SQ64(to));
        SETBIT(pos -> pawns[BOTH], SQ64(to));
    }

    for(index = 0; index < pos -> pieceNumber[piece]; ++index) {
        if(pos -> pieceList[piece][index] == from) {
            pos -> pieceList[piece][index] = to;
#ifdef DEBUG
            t_PieceNum = TRUE;
#endif
            break;
        }
    }
    ASSERT(t_PieceNum);
}