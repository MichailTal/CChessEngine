#include "definitions.h"
#include "globals.h"
#include "init.h"
#include "macros.h"
#include "stdio.h"
#include "stdlib.h"

int main() {
  AllInit();

  board_representation board[1];
  ParseFen(
      "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1",
      board);
  PrintBoard(board);

  move_list list[1];

  GenerateAllMoves(board, list);

  PrintMoveList(list);

  return 0;
}
