#include "definitions.h"
#include "globals.h"
#include "init.h"
#include "macros.h"
#include "stdio.h"
#include "stdlib.h"

#define HARDTEST "r2k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3RK2R w KQkq - 0 1"

int main() {
  AllInit();

  board_representation board[1];
  move_list list[1];

  ParseFen(HARDTEST, board);
  PerftTest(2, board);
  // GenerateAllMoves(board, list);
  // PrintMoveList(list);

  return 0;
}
