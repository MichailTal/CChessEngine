#include "../include/definitions.h"
#include "../include/globals.h"
#include "../include/init.h"
#include "../include/macros.h"
#include "stdio.h"
#include "stdlib.h"

const int PawnIsolated = -10;
const int PawnPassed[8] = {0, 5, 10, 20, 35, 60, 100, 200};
const int RookOpenFile = 10;
const int RookSemiOpenFile = 5;
const int QueenOpenFile = 5;
const int QueenSemiOpenFile = 3;
const int BishopPair = 30;

#define ENDGAME_MAT                                                            \
  (1 * PieceValue[wR] + 2 * PieceValue[wN] + 2 * PieceValue[wP] +              \
   PieceValue[wK])

const int PawnTable[64] = {0,  0,  0,  0,  0,  0,  0,  0,  10, 10, 0,  -10, -10,
                           0,  10, 10, 5,  0,  0,  5,  5,  0,  0,  5,  0,   0,
                           10, 20, 20, 10, 0,  0,  5,  5,  5,  10, 10, 5,   5,
                           5,  10, 10, 10, 20, 20, 10, 10, 10, 20, 20, 20,  30,
                           30, 20, 20, 20, 0,  0,  0,  0,  0,  0,  0,  0};

const int KnightTable[64] = {
    0, -10, 0,  0,  0,  0,  -10, 0, 0, 0,  0,  5,  5,  0,  0,  0,
    0, 0,   10, 10, 10, 10, 0,   0, 0, 0,  10, 20, 20, 10, 5,  0,
    5, 10,  15, 20, 20, 15, 10,  5, 5, 10, 10, 20, 20, 10, 10, 5,
    0, 0,   5,  10, 10, 5,  0,   0, 0, 0,  0,  0,  0,  0,  0,  0};

const int BishopTable[64] = {
    0, 0,  -10, 0,  0,  -10, 0,  0, 0, 0,  0,  10, 10, 0,  0,  0,
    0, 0,  10,  15, 15, 10,  0,  0, 0, 10, 15, 20, 20, 15, 10, 0,
    0, 10, 15,  20, 20, 15,  10, 0, 0, 0,  10, 15, 15, 10, 0,  0,
    0, 0,  0,   10, 10, 0,   0,  0, 0, 0,  0,  0,  0,  0,  0,  0};

const int RookTable[64] = {0,  0,  5,  10, 10, 5,  0,  0,  0,  0,  5,  10, 10,
                           5,  0,  0,  0,  0,  5,  10, 10, 5,  0,  0,  0,  0,
                           5,  10, 10, 5,  0,  0,  0,  0,  5,  10, 10, 5,  0,
                           0,  0,  0,  5,  10, 10, 5,  0,  0,  25, 25, 25, 25,
                           25, 25, 25, 25, 0,  0,  5,  10, 10, 5,  0,  0};

const int KingE[64] = {
    -50, -10, 0,  0,  0,  0,  -10, -50, -10, 0,   10, 10, 10, 10, 0,   -10,
    0,   10,  20, 20, 20, 20, 10,  0,   0,   10,  20, 40, 40, 20, 10,  0,
    0,   10,  20, 40, 40, 20, 10,  0,   0,   10,  20, 20, 20, 20, 10,  0,
    -10, 0,   10, 10, 10, 10, 0,   -10, -50, -10, 0,  0,  0,  0,  -10, -50};

const int KingO[64] = {0,   5,   5,   -10, -10, 0,   10,  5,   -30, -30, -30,
                       -30, -30, -30, -30, -30, -50, -50, -50, -50, -50, -50,
                       -50, -50, -70, -70, -70, -70, -70, -70, -70, -70, -70,
                       -70, -70, -70, -70, -70, -70, -70, -70, -70, -70, -70,
                       -70, -70, -70, -70, -70, -70, -70, -70, -70, -70, -70,
                       -70, -70, -70, -70, -70, -70, -70, -70, -70};

int Mirror64[64] = {56, 57, 58, 59, 60, 61, 62, 63, 48, 49, 50, 51, 52,
                    53, 54, 55, 40, 41, 42, 43, 44, 45, 46, 47, 32, 33,
                    34, 35, 36, 37, 38, 39, 24, 25, 26, 27, 28, 29, 30,
                    31, 16, 17, 18, 19, 20, 21, 22, 23, 8,  9,  10, 11,
                    12, 13, 14, 15, 0,  1,  2,  3,  4,  5,  6,  7};

int MaterialDraw(const board_representation *pos) {

  ASSERT(CheckBoard(pos));

  if (!pos->pieceNumber[wR] && !pos->pieceNumber[bR] && !pos->pieceNumber[wQ] &&
      !pos->pieceNumber[bQ]) {
    if (!pos->pieceNumber[bB] && !pos->pieceNumber[wB]) {
      if (pos->pieceNumber[wN] < 3 && pos->pieceNumber[bN] < 3) {
        return TRUE;
      }
    } else if (!pos->pieceNumber[wN] && !pos->pieceNumber[bN]) {
      if (abs(pos->pieceNumber[wB] - pos->pieceNumber[bB]) < 2) {
        return TRUE;
      }
    } else if ((pos->pieceNumber[wN] < 3 && !pos->pieceNumber[wB]) ||
               (pos->pieceNumber[wB] == 1 && !pos->pieceNumber[wN])) {
      if ((pos->pieceNumber[bN] < 3 && !pos->pieceNumber[bB]) ||
          (pos->pieceNumber[bB] == 1 && !pos->pieceNumber[bN])) {
        return TRUE;
      }
    }
  } else if (!pos->pieceNumber[wQ] && !pos->pieceNumber[bQ]) {
    if (pos->pieceNumber[wR] == 1 && pos->pieceNumber[bR] == 1) {
      if ((pos->pieceNumber[wN] + pos->pieceNumber[wB]) < 2 &&
          (pos->pieceNumber[bN] + pos->pieceNumber[bB]) < 2) {
        return TRUE;
      }
    } else if (pos->pieceNumber[wR] == 1 && !pos->pieceNumber[bR]) {
      if ((pos->pieceNumber[wN] + pos->pieceNumber[wB] == 0) &&
          (((pos->pieceNumber[bN] + pos->pieceNumber[bB]) == 1) ||
           ((pos->pieceNumber[bN] + pos->pieceNumber[bB]) == 2))) {
        return TRUE;
      }
    } else if (pos->pieceNumber[bR] == 1 && !pos->pieceNumber[wR]) {
      if ((pos->pieceNumber[bN] + pos->pieceNumber[bB] == 0) &&
          (((pos->pieceNumber[wN] + pos->pieceNumber[wB]) == 1) ||
           ((pos->pieceNumber[wN] + pos->pieceNumber[wB]) == 2))) {
        return TRUE;
      }
    }
  }
  return FALSE;
}

int EvalPosition(const board_representation *pos) {

  ASSERT(CheckBoard(pos));

  int pce;
  int pieceNumber;
  int sq;
  int score = pos->material[WHITE] - pos->material[BLACK];

  if (!pos->pieceNumber[wP] && !pos->pieceNumber[bP] &&
      MaterialDraw(pos) == TRUE) {
    return 0;
  }

  pce = wP;
  for (pieceNumber = 0; pieceNumber < pos->pieceNumber[pce]; ++pieceNumber) {
    sq = pos->pieceList[pce][pieceNumber];
    ASSERT(SqOnBoard(sq));
    ASSERT(SQ64(sq) >= 0 && SQ64(sq) <= 63);
    score += PawnTable[SQ64(sq)];

    if ((IsolatedMask[SQ64(sq)] & pos->pawns[WHITE]) == 0) {
      score += PawnIsolated;
    }

    if ((WhitePassedMask[SQ64(sq)] & pos->pawns[BLACK]) == 0) {
      score += PawnPassed[RanksBrd[sq]];
    }
  }

  pce = bP;
  for (pieceNumber = 0; pieceNumber < pos->pieceNumber[pce]; ++pieceNumber) {
    sq = pos->pieceList[pce][pieceNumber];
    ASSERT(SqOnBoard(sq));
    ASSERT(MIRROR64(SQ64(sq)) >= 0 && MIRROR64(SQ64(sq)) <= 63);
    score -= PawnTable[MIRROR64(SQ64(sq))];

    if ((IsolatedMask[SQ64(sq)] & pos->pawns[BLACK]) == 0) {
      score -= PawnIsolated;
    }

    if ((BlackPassedMask[SQ64(sq)] & pos->pawns[WHITE]) == 0) {
      score -= PawnPassed[7 - RanksBrd[sq]];
    }
  }

  pce = wN;
  for (pieceNumber = 0; pieceNumber < pos->pieceNumber[pce]; ++pieceNumber) {
    sq = pos->pieceList[pce][pieceNumber];
    ASSERT(SqOnBoard(sq));
    ASSERT(SQ64(sq) >= 0 && SQ64(sq) <= 63);
    score += KnightTable[SQ64(sq)];
  }

  pce = bN;
  for (pieceNumber = 0; pieceNumber < pos->pieceNumber[pce]; ++pieceNumber) {
    sq = pos->pieceList[pce][pieceNumber];
    ASSERT(SqOnBoard(sq));
    ASSERT(MIRROR64(SQ64(sq)) >= 0 && MIRROR64(SQ64(sq)) <= 63);
    score -= KnightTable[MIRROR64(SQ64(sq))];
  }

  pce = wB;
  for (pieceNumber = 0; pieceNumber < pos->pieceNumber[pce]; ++pieceNumber) {
    sq = pos->pieceList[pce][pieceNumber];
    ASSERT(SqOnBoard(sq));
    ASSERT(SQ64(sq) >= 0 && SQ64(sq) <= 63);
    score += BishopTable[SQ64(sq)];
  }

  pce = bB;
  for (pieceNumber = 0; pieceNumber < pos->pieceNumber[pce]; ++pieceNumber) {
    sq = pos->pieceList[pce][pieceNumber];
    ASSERT(SqOnBoard(sq));
    ASSERT(MIRROR64(SQ64(sq)) >= 0 && MIRROR64(SQ64(sq)) <= 63);
    score -= BishopTable[MIRROR64(SQ64(sq))];
  }

  pce = wR;
  for (pieceNumber = 0; pieceNumber < pos->pieceNumber[pce]; ++pieceNumber) {
    sq = pos->pieceList[pce][pieceNumber];
    ASSERT(SqOnBoard(sq));
    ASSERT(SQ64(sq) >= 0 && SQ64(sq) <= 63);
    score += RookTable[SQ64(sq)];

    ASSERT(FileRankValid(FilesBrd[sq]));

    if (!(pos->pawns[BOTH] & FileBBMask[FilesBrd[sq]])) {
      score += RookOpenFile;
    } else if (!(pos->pawns[WHITE] & FileBBMask[FilesBrd[sq]])) {
      score += RookSemiOpenFile;
    }
  }

  pce = bR;
  for (pieceNumber = 0; pieceNumber < pos->pieceNumber[pce]; ++pieceNumber) {
    sq = pos->pieceList[pce][pieceNumber];
    ASSERT(SqOnBoard(sq));
    ASSERT(MIRROR64(SQ64(sq)) >= 0 && MIRROR64(SQ64(sq)) <= 63);
    score -= RookTable[MIRROR64(SQ64(sq))];
    ASSERT(FileRankValid(FilesBrd[sq]));
    if (!(pos->pawns[BOTH] & FileBBMask[FilesBrd[sq]])) {
      score -= RookOpenFile;
    } else if (!(pos->pawns[BLACK] & FileBBMask[FilesBrd[sq]])) {
      score -= RookSemiOpenFile;
    }
  }

  pce = wQ;
  for (pieceNumber = 0; pieceNumber < pos->pieceNumber[pce]; ++pieceNumber) {
    sq = pos->pieceList[pce][pieceNumber];
    ASSERT(SqOnBoard(sq));
    ASSERT(SQ64(sq) >= 0 && SQ64(sq) <= 63);
    ASSERT(FileRankValid(FilesBrd[sq]));
    if (!(pos->pawns[BOTH] & FileBBMask[FilesBrd[sq]])) {
      score += QueenOpenFile;
    } else if (!(pos->pawns[WHITE] & FileBBMask[FilesBrd[sq]])) {
      score += QueenSemiOpenFile;
    }
  }

  pce = bQ;
  for (pieceNumber = 0; pieceNumber < pos->pieceNumber[pce]; ++pieceNumber) {
    sq = pos->pieceList[pce][pieceNumber];
    ASSERT(SqOnBoard(sq));
    ASSERT(SQ64(sq) >= 0 && SQ64(sq) <= 63);
    ASSERT(FileRankValid(FilesBrd[sq]));
    if (!(pos->pawns[BOTH] & FileBBMask[FilesBrd[sq]])) {
      score -= QueenOpenFile;
    } else if (!(pos->pawns[BLACK] & FileBBMask[FilesBrd[sq]])) {
      score -= QueenSemiOpenFile;
    }
  }

  pce = wK;
  sq = pos->pieceList[pce][0];
  ASSERT(SqOnBoard(sq));
  ASSERT(SQ64(sq) >= 0 && SQ64(sq) <= 63);

  if ((pos->material[BLACK] <= ENDGAME_MAT)) {
    score += KingE[SQ64(sq)];
  } else {
    score += KingO[SQ64(sq)];
  }

  pce = bK;
  sq = pos->pieceList[pce][0];
  ASSERT(SqOnBoard(sq));
  ASSERT(MIRROR64(SQ64(sq)) >= 0 && MIRROR64(SQ64(sq)) <= 63);

  if ((pos->material[WHITE] <= ENDGAME_MAT)) {
    score -= KingE[MIRROR64(SQ64(sq))];
  } else {
    score -= KingO[MIRROR64(SQ64(sq))];
  }

  if (pos->pieceNumber[wB] >= 2)
    score += BishopPair;
  if (pos->pieceNumber[bB] >= 2)
    score -= BishopPair;

  if (pos->side == WHITE) {
    return score;
  } else {
    return -score;
  }
}
