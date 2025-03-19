#include "../include/definitions.h"
#include "../include/globals.h"
#include "../include/init.h"
#include "../include/macros.h"
#include "stdio.h"
#include "stdlib.h"

static void CheckUp() {
  // Check if time up or interrupt from GUI
}

static int IsRepetition(const board_representation *pos) {

  int index = 0;

  for (index = pos->histPly - pos->fiftyMoveRule; index < pos->histPly - 1;
       ++index) {

    ASSERT(index >= 0 && index < MAXGAMEHALFMOVES);

    if (pos->posKey == pos->moveHistory[index].posKey) {
      return TRUE;
    }
  }
  return FALSE;
}

static void ClearForSearch(S_SEARCHINFO *info, board_representation *pos) {

  int index = 0;
  int index_2 = 0;

  for (index = 0; index < 13; ++index) {
    for (index_2 = 0; index_2 < BRD_SQ_NUM; ++index_2) {
      pos -> searchHistory[index][index_2] = 0;
    }
  }

  for (index = 0; index < 2; ++index) {
    for (index_2 = 0; index_2 < MAXDEPTH; ++index_2) {
      pos -> searchKillers[index][index_2] = 0;
    }
  }

  ClearPvTable(pos -> PvTable);
  pos -> ply = 0;

  info -> starttime = GetTimeMs();
  info -> stopped = 0;
  info -> nodes = 0;

  
}

static int AlphaBeta(int alpha, int beta, int depth, board_representation *pos,
                     S_SEARCHINFO *info, int DoNull) {
  return 0;
}

static int Quiescence(int alpha, int beta, board_representation *pos,
                      S_SEARCHINFO *info) {
  return 0;
}

void SearchPosition(board_representation *pos, S_SEARCHINFO *info) {}
