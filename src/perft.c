#include "../include/definitions.h"
#include "../include/globals.h"
#include "../include/init.h"
#include "../include/macros.h"
#include "stdio.h"
#include "stdlib.h"

long leafNodes;

void Perft(int depth, board_representation *pos) {

  ASSERT(CheckBoard(pos));

  if (depth == 0) {
    leafNodes++;
    return;
  }

  move_list list[1];
  GenerateAllMoves(pos, list);

  int MoveNum = 0;
  for (MoveNum = 0; MoveNum < list->count; ++MoveNum) {

    if (!MakeMove(pos, list->moves[MoveNum].move)) {
      continue;
    }
    Perft(depth - 1, pos);
    TakeMove(pos);
  }

  return;
}

void PerftTest(int depth, board_representation *pos) {

  ASSERT(CheckBoard(pos));

  PrintBoard(pos);
  printf("\nStarting Test To Depth:%d\n", depth);
  leafNodes = 0;
  int start = GetTimeMs();
  move_list list[1];
  GenerateAllMoves(pos, list);

  int move;
  int MoveNum = 0;
  for (MoveNum = 0; MoveNum < list->count; ++MoveNum) {
    move = list->moves[MoveNum].move;
    if (!MakeMove(pos, move)) {
      continue;
    }
    long cumnodes = leafNodes;
    Perft(depth - 1, pos);
    TakeMove(pos);
    long oldnodes = leafNodes - cumnodes;
    printf("move %d : %s : %ld\n", MoveNum + 1, PrintMove(move), oldnodes);
  }

  printf("\nTest Complete : %ld nodes visited in %dms\n", leafNodes,
         GetTimeMs() - start);

  return;
}
