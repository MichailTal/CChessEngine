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
  int from = A2;
  int to = H7;
  int cap = wR;
  int promoted = bQ;

  move = (( from ) | (to << 7 ) | (cap << 14) | (promoted << 20));

  printf("from: %d, to: %d, cap: %d, prom: %d\n", FROMSQ(move), TOSQ(move), CAPTURED(move), PROMOTED(move));

  printf("Algebraic Notation from: %s\n", PrintSquare(from));
  printf("Algebraic Notation to: %s\n", PrintSquare(to));
  printf("Algebraic Notation move: %s\n", PrintMove(move));

  return 0;
}
