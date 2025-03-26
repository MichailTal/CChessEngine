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

#define WAC1                                                                   \
  "r1b1k2r/ppppnppp/2n2q2/2b5/3NP3/2P1B3/PP3PPP/RN1QKB1R w KQkq - 0 1"

int main() {
  AllInit();

  board_representation *board = GenBoard();
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
      info->depth = 6;
      info -> starttime = GetTimeMs();
      info -> stoptime = GetTimeMs() + 200000;
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
  return 0;
}
