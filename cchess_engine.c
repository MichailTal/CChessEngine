#include "definitions.h"
#include "globals.h"
#include "init.h"
#include "macros.h"
#include "stdio.h"
#include "stdlib.h"

int main() {
  AllInit();

  board_representation board[1];

  ParseFen("rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1",
           board);
  PrintBoard(board);

  PrintBitBoard(board->pawns[WHITE]);

  return 0;
}
