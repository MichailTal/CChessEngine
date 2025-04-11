#include "../include/definitions.h"
#include "../include/globals.h"
#include "../include/init.h"
#include "../include/macros.h"
#include "../include/polykeys.h"
#include "stdio.h"

const int PolyKindOfPiece[13] = {
    -1, 1, 3, 5, 7, 9, 11, 0, 2, 4, 6, 8, 10
};

int HasPawnForCapture(board_representation *pos ) {
    int squareWithPawn = 0;
    int targetPiece = (pos -> side == WHITE) ? wP : bP;

    if (pos -> enPassant != NO_SQ) {
        if (pos -> side == WHITE) {
            squareWithPawn = pos -> enPassant -10;
        } else {
            squareWithPawn = pos -> enPassant +10;
        }

        if (pos -> pieces[squareWithPawn + 1] == targetPiece) {
            return TRUE;
        } else if (pos -> pieces[squareWithPawn - 1] == targetPiece) {
            return TRUE;
        }
    }

    return FALSE;
}

U64 PolyKeyFromBoard(board_representation *pos) {

  int square = 0;
  int rank = 0;
  int file = 0;
  U64 finalKey = 0;
  int piece = EMPTY;
  int polyPiece = 0;
  int offset = 0;
  
  for (square = 0; square < BRD_SQ_NUM; ++square) {
    piece = pos -> pieces[square];

    if (piece != NO_SQ && piece != EMPTY && piece != OFFBOARD) {
        ASSERT(piece >= wP && piece <= bK);
        polyPiece = PolyKindOfPiece[piece];
        rank = RanksBrd[square];
        file = FilesBrd[square];
        finalKey ^= Random64Poly[(64 * polyPiece) + (8 * rank) + file];
    }
  }

  // Castling

  offset = 768;

  if (pos -> castlePermission & whiteKingCastle) finalKey ^= Random64Poly[offset + 0];
  if (pos -> castlePermission & whiteQueenCastle) finalKey ^= Random64Poly[offset + 1];
  if (pos -> castlePermission & blackKingCastle) finalKey ^= Random64Poly[offset + 2];
  if (pos -> castlePermission & blackQueenCastle) finalKey ^= Random64Poly[offset + 3];

  // En Passant 

  offset = 772;

  if (HasPawnForCapture(pos) == TRUE) {
    file = FilesBrd[pos -> enPassant];
    finalKey ^= Random64Poly[offset + file];
  }

  // Side to Move

  if (pos -> side == WHITE) {
    finalKey ^= Random64Poly[780];
  }

  return finalKey;
}