#include "../include/definitions.h"
#include "../include/globals.h"
#include "../include/init.h"
#include "../include/macros.h"
#include "stdio.h"
#include "stdlib.h"

int MoveListOk(const move_list *list, const board_representation *pos) {

  if (list->count < 0 || list->count >= MAXPOSITIONMOVES) {
    return FALSE;
  }

  int MoveNum;
  int from = 0;
  int to = 0;

  for (MoveNum = 0; MoveNum < list->count; ++MoveNum) {
    to = TOSQ(list->moves[MoveNum].move);
    from = FROMSQ(list->moves[MoveNum].move);

    if (!SqOnBoard(to) || !SqOnBoard(from)) {
      return FALSE;
    }
    if (!PieceValid(pos->pieces[from])) {
      PrintBoard(pos);
      return FALSE;
    }
  }
  return TRUE;
}

int SqIs120(const int square) { return (square >= 0 && square < 120); }

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
