#include "../include/definitions.h"
#include "../include/globals.h"
#include "../include/init.h"
#include "../include/macros.h"
#include "stdio.h"
#include "string.h"

int ThreeFoldRedp(const board_representation *pos) {
    int i = 0;
    int r = 0;

    for (i = 0; i < pos -> histPly; ++i) {
        if (pos -> moveHistory[i].posKey == pos -> posKey) {
            r++;
        }
    }
    return r;
}

int DrawMaterial (const board_representation *pos) {
    if (pos -> pieceNumber[wP] || pos -> pieceNumber[bP]) return FALSE;  // no draw if pawns are still there
    if (pos -> pieceNumber[wQ] || pos -> pieceNumber[bQ] || pos -> pieceNumber[wR] || pos -> pieceNumber[bR]) return FALSE; // no draw if one or both sides has Rock and Queen
    if (pos -> pieceNumber[wB] > 1 || pos -> pieceNumber[bB] > 1) {return FALSE;}
    if (pos -> pieceNumber[wN] > 1 || pos -> pieceNumber[bN] > 1) {return FALSE;}
    if (pos -> pieceNumber[wN] && pos ->pieceNumber[wB]) {return FALSE;}
    if (pos -> pieceNumber[bN] && pos ->pieceNumber[bB]) {return FALSE;}

    return TRUE;
}

int Checkresult (board_representation *pos) {

    if (pos -> fiftyMoveRule > 100) {
        printf("1/2-1/2 {fity move rule claimed by engine}\n"); return TRUE;
    }

    if (ThreeFoldRedp(pos) >= 2) {
        printf("1/2-1/2 {threefold claimed by engine}\n");
    }

    if (DrawMaterial(pos) == TRUE) {
        printf("1/2-1/2 {insufficient material claimed by engine}\n");
    }

    move_list list[1];
    GenerateAllMoves(pos, list);

    int MoveNum = 0;
    int found = 0;

    for (MoveNum = 0; MoveNum < list -> count; ++MoveNum) {

        if(!MakeMove(pos, list -> moves[MoveNum].move)) {
            continue;
        }
        found++;
        TakeMove(pos);
        break;
    }

    if(found != 0) return FALSE;

    int InCheck = SqAttacked(pos -> kingSquare[pos->side], pos->side^1, pos);

    if (InCheck == TRUE) {
        if(pos -> side == WHITE) {
            printf("0-1 {black mates according to engine}\n"); return TRUE;
        } else {
            printf("1-0 {white mates according to engine}\n"); return TRUE;
        }
    } else {
        printf("1/2-1/2 {stalemate claimed by engine}\n"); return TRUE;
    }

    return FALSE;
}
