#include "../include/definitions.h"
#include "../include/globals.h"
#include "../include/init.h"
#include "../include/macros.h"
#include "math.h"
#include "stdio.h"
#include "stdlib.h"

static void CheckUp() {
  // Check if time up or interrupt from GUI
}

static void PickNextMove(int moveNum, move_list *list) {

  move_representation temp;
  int index = 0;
  int bestScore = 0;
  int bestNum = moveNum;

  for (index = moveNum; index < list->count; ++index) {
    if (list->moves[index].score > bestScore) {
      bestScore = list->moves[index].score;
      bestNum = index;
    }
  }

  temp = list->moves[moveNum];
  list->moves[moveNum] = list->moves[bestNum];
  list->moves[bestNum] = temp;
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
      pos->searchHistory[index][index_2] = 0;
    }
  }

  for (index = 0; index < 2; ++index) {
    for (index_2 = 0; index_2 < MAXDEPTH; ++index_2) {
      pos->searchKillers[index][index_2] = 0;
    }
  }

  ClearPvTable(pos->PvTable);
  pos->ply = 0;

  info->starttime = GetTimeMs();
  info->stopped = 0;
  info->nodes = 0;
  info->failhigh = 0;
  info->failhighfirst = 0;
}

static int AlphaBeta(int alpha, int beta, int depth, board_representation *pos,
                     S_SEARCHINFO *info, int DoNull) {

  ASSERT(CheckBoard(pos));

  if (depth == 0) {
    info->nodes++;
    return EvalPosition(pos);
  }

  info->nodes++;

  if (IsRepetition(pos) || pos->fiftyMoveRule >= 100) {
    return 0;
  }

  if (pos->ply > MAXDEPTH - 1) {
    return EvalPosition(pos);
  }

  move_list list[1];
  GenerateAllMoves(pos, list);

  int MoveNum = 0;
  int Legal = 0;
  int OldAlpha = alpha;
  int bestMove = NOMOVE;
  int Score = -INFINITE;
  int PvMove = NOMOVE;

  if (PvMove != NOMOVE) {
    for (MoveNum = 0; MoveNum < list->count; ++MoveNum) {
      if (list->moves[MoveNum].move == PvMove) {
        list->moves[MoveNum].score = 2000000;
        printf("Pv move found \n");
        break;
      }
    }
  }

  for (MoveNum = 0; MoveNum < list->count; ++MoveNum) {

    PickNextMove(MoveNum, list);

    if (!MakeMove(pos, list->moves[MoveNum].move)) {
      continue;
    }

    Legal++;
    Score = -AlphaBeta(-beta, -alpha, depth - 1, pos, info, TRUE);
    TakeMove(pos);

    if (Score > alpha) {
      if (Score >= beta) {
        if (Legal == 1) {
          info->failhighfirst++;
        }
        info->failhigh++;

        if (!(list->moves[MoveNum].move & MOVEFLAGCAPTURE)) {
          pos->searchKillers[1][pos->ply] = pos->searchKillers[0][pos->ply];
          pos->searchKillers[0][pos->ply] = list->moves[MoveNum].move;
        }

        return beta;
      }
      alpha = Score;
      bestMove = list->moves[MoveNum].move;
      if (!(list->moves[MoveNum].move & MOVEFLAGCAPTURE)) {
        pos->searchHistory[pos->pieces[FROMSQ(bestMove)]][TOSQ(bestMove)] +=
            depth;
      }
    }
  }

  if (Legal == 0) {
    if (SqAttacked(pos->kingSquare[pos->side], pos->side ^ 1, pos)) {
      return -ISMATE + pos->ply; // Distance to mate from the root
    } else {
      return 0;
    }
  }

  if (alpha != OldAlpha) {
    StorePvMove(pos, bestMove);
  }

  return alpha;
}

static int Quiescence(int alpha, int beta, board_representation *pos,
                      S_SEARCHINFO *info) {
  return 0;
}

void SearchPosition(board_representation *pos, S_SEARCHINFO *info) {

  int bestMove = NOMOVE;
  int bestScore = -INFINITE;
  int currentDepth = 0;
  int pvMoves = 0;
  int pvNum = 0;
  ClearForSearch(info, pos);

  for (currentDepth = 1; currentDepth <= info->depth; ++currentDepth) {

    bestScore = AlphaBeta(-INFINITE, INFINITE, currentDepth, pos, info, TRUE);
    pvMoves = GetPvLine(currentDepth, pos);
    bestMove = pos->PvArray[0];

    printf("Depth: %d score: %d move: %s nodes %1ld", currentDepth, bestScore,
           PrintMove(bestMove), info->nodes);

    pvMoves = GetPvLine(currentDepth, pos);
    printf("pv");
    for (pvNum = 0; pvNum < pvMoves; ++pvNum) {
      printf(" %s", PrintMove(pos->PvArray[pvNum]));
    }
    printf("\n");
    printf("Ordering: %.2f\n", (info->failhighfirst / info->failhigh));
  }
}
