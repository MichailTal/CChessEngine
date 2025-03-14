#include "definitions.h"
#include "globals.h"
#include "init.h"
#include "macros.h"
#include "stdio.h"
#include "stdlib.h"
#include "stddef.h"
#include "string.h"

#define HARDTEST                                                               \
  "r2k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3RK2R w KQkq - 0 1"

int main() {
  AllInit();

  board_representation board[1];
  move_list list[1];

  ParseFen(HARDTEST, board);

  char input[6];
  int Move = NOMOVE;

  while (TRUE) {
      PrintBoard(board);
      printf("Please enter a move: > ");
      fgets(input, 6, stdin);

      if (input[0] == 'q') {
        break;
      } else if (input[0] == 't') {
        TakeMove(board);
      } else {
        Move = ParseMove(input, board);
        if (Move != NOMOVE) {
          MakeMove(board, Move);
        }
      }
      fflush(stdin);
  }    
}