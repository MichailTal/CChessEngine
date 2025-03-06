#include "definitions.h"
#include "globals.h"
#include "init.h"
#include "macros.h"
#include "stdio.h"
#include "stdlib.h"

int main() {
  AllInit();

  board_representation board[1];
  ParseFen("rnbqkb1r/5pPp/8/2p1pP2/1P1P3P/3P4/P1P1P3/RNBQKBNR w - - 0 1",
           board);
  PrintBoard(board);

  move_list list[1];

  GenerateAllMoves(board, list);

  PrintMoveList(list);

  return 0;
}
