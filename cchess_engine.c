#include "definitions.h"
#include "globals.h"
#include "init.h"
#include "macros.h"
#include "stdio.h"
#include "stdlib.h"

int main() {
  AllInit();

  board_representation board[1];
  move_list list[1];

  ParseFen(
      START_FEN,
      board);

  GenerateAllMoves(board, list);
  PrintMoveList(list);

  int MoveNum = 0;
  int move = 0;

  PrintBoard(board);
  getchar();

  for (MoveNum = 0; MoveNum < list -> count; ++MoveNum) {
    move = list -> moves[MoveNum].move;

    if (!MakeMove(board, move)) {
      continue;
    }

    printf("\nMade: %s\n", PrintMove(move));
    PrintBoard(board);

    TakeMove(board);
    printf("\nTAKEN:%s\n", PrintMove(move));
    PrintBoard(board);

    getchar();
  }

  return 0;
}
