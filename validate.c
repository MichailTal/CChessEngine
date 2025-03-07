#include "definitions.h"
#include "globals.h"
#include "init.h"
#include "macros.h"
#include "stdio.h"
#include "stdlib.h"

int SqOnBoard(const int square) { return FilesBrd[square] == OFFBOARD ? 0 : 1; }

int SideValid(const int side) {
  return (side == WHITE || side == BLACK) ? 1 : 0;
}

int FileRankValid(const int filerank) {
  return (filerank >= 0 && filerank <= 7) ? 1 : 0;
}

int PieceValidEmpty(const int piece) {
  return (piece >= EMPTY && piece <= bK) ? 1 : 0;
}

int PieceValid(const int piece) { return (piece >= wP && piece <= bK) ? 1 : 0; }
