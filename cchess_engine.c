#include "definitions.h"
#include "globals.h"
#include "init.h"
#include "macros.h"
#include "stdio.h"
#include "stdlib.h"

int main() {
  AllInit();

  board_representation board[1];

  ParseFen("8/8/8/8/3Q4/8/8/8 w - - 0 1", board);
  PrintBoard(board);

  printf("\n\nWhite Attacking: \n");

  // ASSERT(CheckBoard(board));

  return 0;
}
