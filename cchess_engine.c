#include "definitions.h"
#include "globals.h"
#include "init.h"
#include "macros.h"
#include "stdio.h"
#include "stdlib.h"

int main() {
  AllInit();

  board_representation board[1];
  ParseFen("8/8/3n1k2/4P3/8/8/8/8 w - - 0 1", board);
  PrintBoard(board);

  move_list list[1];

  GenerateAllMoves(board, list);

  PrintMoveList(list);

  return 0;
}
