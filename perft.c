#include "definitions.h"
#include "globals.h"
#include "init.h"
#include "macros.h"
#include "stdio.h"
#include "stdlib.h"

long leafNodes;

void Perft(int depth, board_representation *pos) {

  int check = CheckBoard(pos);

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

  printf("\nTest Complete : %ld nodes visited\n", leafNodes);

  return;
}
