#include "definitions.h"
#include "globals.h"
#include "init.h"
#include "macros.h"
#include "stddef.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#define HARDTEST                                                               \
  "r2k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3RK2R w KQkq - 0 1"

int main() {
  AllInit();

  board_representation board[1];
  move_list list[1];

  ParseFen(START_FEN, board);

  char input[6];
  int Move = NOMOVE;
  int PvNum = 0;
  int Max = 0;

  while (TRUE) {
    PrintBoard(board);
    printf("Please enter a move: > ");
    fgets(input, 6, stdin);

    if (input[0] == 'q') {
      break;
    } else if (input[0] == 't') {
      TakeMove(board);
    } else if (input[0] == 'p') {
      PerftTest(4, board);
    } else if (input[0] == 'r') {
      Max = GetPvLine(4, board);
      printf("Pv Line of %d Moves: ", Max);
      for (PvNum = 0; PvNum < Max; ++PvNum) {
        Move = board->PvArray[PvNum];
        printf(" %s", PrintMove(Move));
      }
      printf("\n");
    } else {
      Move = ParseMove(input, board);
      if (Move != NOMOVE) {
        StorePvMove(board, Move);
        MakeMove(board, Move);
        if (IsRepetition(board)) {
          printf("REP SEEN\n");
        }
      } else {
        printf("Move not parsed: %s \n");
      }
    }
    fflush(stdin);
  }
}
