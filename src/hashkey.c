#include "../include/definitions.h"
#include "../include/globals.h"
#include "../include/init.h"
#include "../include/macros.h"
#include "stdio.h"
#include "stdlib.h"

U64 Rand_64(void) {
  return ((U64)rand() | ((U64)rand() << 15) | ((U64)rand() << 30) |
          ((U64)rand() << 45) | (((U64)rand() & 0xf) << 60));
}

U64 GeneratePosKey(const board_representation *pos) {

  int square = 0;
  U64 finalKey = 0;
  int piece = EMPTY;

  // pieces
  for (square = 0; square < BRD_SQ_NUM; square++) {
    piece = pos->pieces[square];
    if (piece != NO_SQ && piece != EMPTY && piece != OFFBOARD) {
      ASSERT(piece >= wP && piece <= bK);
      finalKey ^= PieceKey[piece][square];
    }
  }

  // Side to Move
  if (pos->side == WHITE) {
    finalKey ^= SideKey;
  }

  // En Passant eligable squares
  if (pos->enPassant != NO_SQ) {
    ASSERT(pos->enPassant >= 0 && pos->enPassant < BRD_SQ_NUM);
    finalKey ^= PieceKey[EMPTY][pos->enPassant];
  }

  ASSERT(pos->castlePermission >= 0 && pos->castlePermission <= 15);

  finalKey ^= CastleKeys[pos->castlePermission];

  return finalKey;
}
