#include "../include/definitions.h"
#include "../include/globals.h"
#include "../include/init.h"
#include "../include/macros.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

static void CheckUp(S_SEARCHINFO *info) {
  // Check if time up or interrupt from GUI
  if (info->timeset == TRUE && GetTimeMs() > info->stoptime) {
    info->stopped = TRUE;
  }
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

static void ClearForSearch(S_SEARCHINFO *info, board_representation *pos,
                           S_HASHTABLE *table) {

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

  table->overWrite = 0;
  table->hit = 0;
  table->cut = 0;
  pos->ply = 0;
  table->currentage++;

  info->starttime = GetTimeMs();
  info->stopped = 0;
  info->nodes = 0;
  info->failhigh = 0;
  info->failhighfirst = 0;
}

static int Quiescence(int alpha, int beta, board_representation *pos,
                      S_SEARCHINFO *info) {

  ASSERT(CheckBoard(pos));
  ASSERT(beta > alpha);
  if ((info->nodes & 2047) == 0) {
    CheckUp(info);
  }

  info->nodes++;

  if (IsRepetition(pos) || pos->fiftyMoveRule >= 100) {
    return 0;
  }

  if (pos->ply > MAXDEPTH - 1) {
    return EvalPosition(pos);
  }

  int Score = EvalPosition(pos);

  ASSERT(Score > -INF_BOUND && Score < INF_BOUND);

  if (Score >= beta) {
    return beta;
  }

  if (Score > alpha) {
    alpha = Score;
  }

  move_list list[1];
  GenerateAllCaps(pos, list);

  int MoveNum = 0;
  int Legal = 0;
  Score = -INF_BOUND;

  for (MoveNum = 0; MoveNum < list->count; ++MoveNum) {

    PickNextMove(MoveNum, list);

    if (!MakeMove(pos, list->moves[MoveNum].move)) {
      continue;
    }

    Legal++;
    Score = -Quiescence(-beta, -alpha, pos, info);
    TakeMove(pos);

    if (info->stopped == TRUE) {
      return 0;
    }

    if (Score > alpha) {
      if (Score >= beta) {
        if (Legal == 1) {
          info->failhighfirst++;
        }
        info->failhigh++;
        return beta;
      }
      alpha = Score;
    }
  }

  return alpha;
}

static int AlphaBeta(int alpha, int beta, int depth, board_representation *pos,
                     S_SEARCHINFO *info, S_HASHTABLE *table, int DoNull) {

  ASSERT(CheckBoard(pos));

  if (depth == 0) {
    return Quiescence(alpha, beta, pos, info);
  }

  if ((info->nodes & 2047) == 0) {
    CheckUp(info);
  }

  info->nodes++;

  if (IsRepetition(pos) || pos->fiftyMoveRule >= 100) {
    return 0;
  }

  if (pos->ply > MAXDEPTH - 1) {
    return EvalPosition(pos);
  }

  int InCheck = SqAttacked(pos->kingSquare[pos->side], pos->side ^ 1, pos);

  if (InCheck == TRUE) {
    depth++;
  }

  int Score = -INF_BOUND;
  int PvMove = NOMOVE;

  if (ProbeHashEntry(pos, table, &PvMove, &Score, alpha, beta, depth) == TRUE) {
    table->cut++;
    return Score;
  }

  if (DoNull && !InCheck && pos->ply && (pos->bigPce[pos->side] > 1) &&
      depth >= 4) { // No Check and no Zugzwang for the Null Move Pruning
    MakeNullMove(pos);
    Score = -AlphaBeta(-beta, -beta + 1, depth - 4, pos, info, table,
                       FALSE); // No recursive null moving
    TakeNullMove(pos);
    if (info->stopped == TRUE) {
      return 0;
    }
    if (Score >= beta && abs(Score) < ISMATE) {
      info->nullCut++;
      return beta;
    }
  }

  move_list list[1];
  GenerateAllMoves(pos, list);

  int MoveNum = 0;
  int Legal = 0;
  int OldAlpha = alpha;
  int bestMove = NOMOVE;
  int BestScore = -INF_BOUND;

  if (PvMove != NOMOVE) {
    for (MoveNum = 0; MoveNum < list->count; ++MoveNum) {
      if (list->moves[MoveNum].move == PvMove) {
        list->moves[MoveNum].score = 2000000;
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
    Score = -AlphaBeta(-beta, -alpha, depth - 1, pos, info, table, TRUE);
    TakeMove(pos);

    if (info->stopped == TRUE) {
      return 0;
    }

    if (Score > BestScore) {
      BestScore = Score;
      bestMove = list->moves[MoveNum].move;

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

          StoreHashEntry(pos, table, bestMove, beta, HFBETA, depth);

          return beta;
        }
        alpha = Score;

        if (!(list->moves[MoveNum].move & MOVEFLAGCAPTURE)) {
          pos->searchHistory[pos->pieces[FROMSQ(bestMove)]][TOSQ(bestMove)] +=
              depth;
        }
      }
    }
  }

  if (Legal == 0) {
    if (InCheck) {
      return -ISMATE + pos->ply; // Distance to mate from the root
    } else {
      return 0;
    }
  }

  if (alpha != OldAlpha) {
    StoreHashEntry(pos, table, bestMove, BestScore, HFEXACT, depth);
  } else {
    StoreHashEntry(pos, table, bestMove, alpha, HFALPHA, depth);
  }

  return alpha;
}

int SearchPositionThread(void *data) {
  S_SEARCH_THREAD_DATA *searchData = (S_SEARCH_THREAD_DATA *)data;
  board_representation *pos = malloc(sizeof(board_representation));
  memcpy(pos, searchData->pos, sizeof(board_representation));
  SearchPosition(pos, searchData->info, searchData->ttable);
  free(pos);
  printf("Freed Thread\n");
  return 0;
}

void SearchPosition(board_representation *pos, S_SEARCHINFO *info,
                    S_HASHTABLE *table) {

  int bestMove = NOMOVE;
  int bestScore = -INF_BOUND;
  int currentDepth = 0;
  int pvMoves = 0;
  int pvNum = 0;
  ClearForSearch(info, pos, table);

  if (EngineOptions->UseBook == TRUE) {
    bestMove = GetBookMoves(pos);
  }

  if (bestMove == NOMOVE) {
    for (currentDepth = 1; currentDepth <= info->depth; ++currentDepth) {

      bestScore = AlphaBeta(-INF_BOUND, INF_BOUND, currentDepth, pos, info,
                            table, TRUE);

      if (info->stopped == TRUE) {
        break;
      }

      pvMoves = GetPvLine(currentDepth, pos, table);
      bestMove = pos->PvArray[0];

      if (info->GAME_MODE == UCIMODE) {
        printf("info score cp %d depth %d nodes %ld time %d ", bestScore,
               currentDepth, info->nodes, GetTimeMs() - info->starttime);
      } else if (info->GAME_MODE == XBOARDMODE && info->POST_THINKING == TRUE) {
        printf("%d %d %d %ld ", currentDepth, bestScore,
               (GetTimeMs() - info->starttime) / 10, info->nodes);
      } else if (info->POST_THINKING == TRUE) {
        printf("score:%d depth:%d nodes:%ld time:%d(ms) ", bestScore,
               currentDepth, info->nodes, GetTimeMs() - info->starttime);
      }
      if (info->GAME_MODE == UCIMODE || info->POST_THINKING == TRUE) {
        if (!(info->GAME_MODE == XBOARDMODE)) {
          printf("pv");
        }
        for (pvNum = 0; pvNum < pvMoves; ++pvNum) {
          printf(" %s", PrintMove(pos->PvArray[pvNum]));
        }
        printf("\n");
      }
    }
  }

  if (info->GAME_MODE == UCIMODE) {
    printf("bestmove %s\n", PrintMove(bestMove));
  } else if (info->GAME_MODE == XBOARDMODE) {
    printf("move %s\n", PrintMove(bestMove));
    MakeMove(pos, bestMove);
  } else {
    printf("\n\n***!! Engine move %s !!***\n\n", PrintMove(bestMove));
    MakeMove(pos, bestMove);
    PrintBoard(pos);
  }
}
