#include "../include/definitions.h"
#include "../include/globals.h"
#include "../include/init.h"
#include "../include/macros.h"
#include "stddef.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#define HARDTEST                                                               \
  "r2k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3RK2R w KQkq - 0 1"

#define WAC1 "2rr3k/pp3pp1/1nnqbN1p/3pN3/2pP4/2P3Q1/PPB4P/R4RK1 w - -"

int main() {
  AllInit();

  board_representation board[1];
  move_list list[1];
  S_SEARCHINFO info[1];

  ParseFen(WAC1, board);

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
    } else if (input[0] == 's') {
      info -> depth = 4;
      SearchPosition(board, info);
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
      } else {
        printf("Move not parsed: %s \n", PrintMove(Move));
      }
    }
    fflush(stdin);
  }
}
