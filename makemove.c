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

int MakeMove(board_representation *pos, const int move) {

    ASSERT(CheckBoard(pos));

    int from = FROMSQ(move);
    int to = TOSQ(move);
    int side = pos -> side;

    ASSERT(SqOnBoard(from));
    ASSERT(SqOnBoard(from));
    ASSERT(SideValid(side));
    ASSERT(PieceValid(pos -> pieces[from]));

    pos -> moveHistory[pos -> histPly].posKey = pos -> posKey;

    if (move & MOVEFLAGENPASSANT) {
        if (side == WHITE) {
            ClearPiece(to-10, pos);
        } else {
            ClearPiece(to+10, pos);
        }
    } else if (move & MOVEFLAGECASTLE) {
        switch(to) {
            case C1:
                MovePiece(A1, D1, pos);
            break;
            case C8:
                MovePiece(A8, D8, pos);
            break;
            case G1:
                MovePiece(H1, F1, pos);
            break;
            case G8:
                MovePiece(H8, F8, pos);
            break;
            default: ASSERT(FALSE); break;
        }
    }

    if (pos -> enPassant != NO_SQ) HASH_EP;
    HASH_CA;

    pos -> moveHistory[pos -> histPly].move = move;
    pos -> moveHistory[pos -> histPly].fiftyMoveRule = pos -> fiftyMoveRule;
    pos -> moveHistory[pos -> histPly].enPassant = pos -> enPassant;
    pos -> moveHistory[pos -> histPly].castlePermission = pos ->castlePermission;

    pos -> castlePermission &= CastlePerm[from];
    pos -> castlePermission &= CastlePerm[to];
    pos -> enPassant = NO_SQ;

    HASH_CA;

    int captured = CAPTURED(move);

    pos -> fiftyMoveRule++;

    if (captured != EMPTY) {
        ASSERT(PieceValid(captured));
        ClearPiece(to, pos);
        pos -> fiftyMoveRule = 0;
    }

    pos -> histPly++;
    pos -> ply++;

    if (PiecePawn[pos -> pieces[from]]) {
        pos -> fiftyMoveRule = 0;

        if (move & MOVEFLAGPAWNSTART) {
            if (side == WHITE) {
                pos -> enPassant = from + 10;
                ASSERT(RanksBrd[pos -> enPassant] == RANK_3);
            } else {
                pos -> enPassant = from - 10;
                ASSERT(RanksBrd[pos -> enPassant] == RANK_6); 
            }
            HASH_EP;
        }
    }

    // TODO: Hier gehts weiter :)

}