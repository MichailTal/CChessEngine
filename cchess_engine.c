#include "definitions.h"
#include "globals.h"
#include "init.h"
#include "macros.h"
#include "stdio.h"
#include "stdlib.h"

int main() {
  AllInit();

  board_representation board[1];

  ParseFen(START_FEN, board);
  PrintBoard(board);
  ASSERT(CheckBoard(board));

  int move = 0;
  int from = 6;
  int to = 12;
  int cap = wR;
  int promoted = bR;

  move = (( from ) | (to << 7 ) | (cap << 14) | (promoted << 20));

  printf("\ndec: %d hex:%X\n", move, move);

  printf("from: %d to: %d cap: %d prom: %d", FROMSQ(move), TOSQ(move), CAPTURED(move), PROMOTED(move));


  return 0;
}
