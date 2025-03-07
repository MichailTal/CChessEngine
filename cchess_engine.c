#include "definitions.h"
#include "globals.h"
#include "init.h"
#include "macros.h"
#include "stdio.h"
#include "stdlib.h"

#define TEST "5k2/1n6/4n3/6N1/8/3N4/8/5K2 w - - 0 1"

int main() {
  AllInit();

  board_representation board[1];
  ParseFen("2k5/8/8/8/8/8/8/4K3 b - - 0 1", board);
  PrintBoard(board);

  move_list list[1];

  GenerateAllMoves(board, list);

  PrintMoveList(list);

  return 0;
}
