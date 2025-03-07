#include "definitions.h"
#include "globals.h"
#include "init.h"
#include "macros.h"
#include "stdio.h"
#include "stdlib.h"

#define MOVE(f, t, ca, pro, fl)                                                \
  ((f) | ((t) << 7) | ((ca) << 14) | ((pro) << 20) | (fl))
#define SQOFFBOARD(sq) (FilesBrd[(sq)] == OFFBOARD)

const int LoopSlidePiece[8] = {wB, wR, wQ, 0, bB, bR, bQ, 0};

const int LoopNonSlidePiece[6] = {wN, wK, 0, bN, bK, 0};

const int LoopSlideIndex[2] = {0, 4};
const int LoopNonSlideIndex[2] = {0, 3};

const int PceDir[13][8] = {{0, 0, 0, 0, 0, 0, 0},
                           {0, 0, 0, 0, 0, 0, 0},
                           {-8, -19, -21, -12, 8, 19, 21, 12},
                           {-9, -11, 11, 9, 0, 0, 0, 0},
                           {-1, -10, 1, 10, 0, 0, 0, 0},
                           {-1, -10, 1, 10, -9, -11, 11, 9},
                           {-1, -10, 1, 10, -9, -11, 11, 9},
                           {0, 0, 0, 0, 0, 0, 0},
                           {-8, -19, -21, -12, 8, 19, 21, 12},
                           {-9, -11, 11, 9, 0, 0, 0, 0},
                           {-1, -10, 1, 10, 0, 0, 0, 0},
                           {-1, -10, 1, 10, -9, -11, 11, 9},
                           {-1, -10, 1, 10, -9, -11, 11, 9}};

const int NumDir[13] = {0, 0, 8, 4, 4, 8, 8, 0, 8, 4, 4, 8, 8};

void AddQuietMove(const board_representation *pos, int move, move_list *list) {
  list->moves[list->count].move = move;
  list->moves[list->count].score = 0;
  list->count++;
}

void AddCaptureMove(const board_representation *pos, int move,
                    move_list *list) {

  list->moves[list->count].move = move;
  list->moves[list->count].score = 0;
  list->count++;
}

void AddEnPassantMove(const board_representation *pos, int move,
                      move_list *list) {

  list->moves[list->count].move = move;
  list->moves[list->count].score = 0;
  list->count++;
}

void AddWhitePawnCapMove(const board_representation *pos, const int from,
                         const int to, const int cap, move_list *list) {
  ASSERT(PieceValidEmpty(cap));
  ASSERT(SqOnBoard(from));
  ASSERT(SqOnBoard(to));

  if (RanksBrd[from] == RANK_7) {
    AddCaptureMove(pos, MOVE(from, to, cap, wQ, 0), list);
    AddCaptureMove(pos, MOVE(from, to, cap, wR, 0), list);
    AddCaptureMove(pos, MOVE(from, to, cap, wB, 0), list);
    AddCaptureMove(pos, MOVE(from, to, cap, wN, 0), list);
  } else {
    AddCaptureMove(pos, MOVE(from, to, cap, EMPTY, 0), list);
  }
}

void AddWhitePawnMove(const board_representation *pos, const int from,
                      const int to, move_list *list) {
  ASSERT(SqOnBoard(from));
  ASSERT(SqOnBoard(to));

  if (RanksBrd[from] == RANK_7) {
    AddQuietMove(pos, MOVE(from, to, EMPTY, wQ, 0), list);
    AddQuietMove(pos, MOVE(from, to, EMPTY, wR, 0), list);
    AddQuietMove(pos, MOVE(from, to, EMPTY, wB, 0), list);
    AddQuietMove(pos, MOVE(from, to, EMPTY, wN, 0), list);
  } else {
    AddQuietMove(pos, MOVE(from, to, EMPTY, EMPTY, 0), list);
  }
}

void AddBlackPawnCapMove(const board_representation *pos, const int from,
                         const int to, const int cap, move_list *list) {
  ASSERT(PieceValidEmpty(cap));
  ASSERT(SqOnBoard(from));
  ASSERT(SqOnBoard(to));

  if (RanksBrd[from] == RANK_2) {
    AddCaptureMove(pos, MOVE(from, to, cap, bQ, 0), list);
    AddCaptureMove(pos, MOVE(from, to, cap, bR, 0), list);
    AddCaptureMove(pos, MOVE(from, to, cap, bB, 0), list);
    AddCaptureMove(pos, MOVE(from, to, cap, bN, 0), list);
  } else {
    AddCaptureMove(pos, MOVE(from, to, cap, EMPTY, 0), list);
  }
}

void AddBlackPawnMove(const board_representation *pos, const int from,
                      const int to, move_list *list) {

  ASSERT(SqOnBoard(from));
  ASSERT(SqOnBoard(to));
  if (RanksBrd[from] == RANK_2) {
    AddQuietMove(pos, MOVE(from, to, EMPTY, bQ, 0), list);
    AddQuietMove(pos, MOVE(from, to, EMPTY, bR, 0), list);
    AddQuietMove(pos, MOVE(from, to, EMPTY, bB, 0), list);
    AddQuietMove(pos, MOVE(from, to, EMPTY, bN, 0), list);
  } else {
    AddQuietMove(pos, MOVE(from, to, EMPTY, EMPTY, 0), list);
  }
}

void GenerateAllMoves(const board_representation *pos, move_list *list) {

  ASSERT(CheckBoard(pos));

  list->count = 0;

  int pce = EMPTY;
  int side = pos->side;
  int sq = 0;
  int t_sq = 0;
  int pceNum = 0;
  int dir = 0;
  int index = 0;
  int pceIndex = 0;

  if (side == WHITE) {

    for (pceNum = 0; pceNum < pos->pieceNumber[wP]; ++pceNum) {
      sq = pos->pieceList[wP][pceNum];
      ASSERT(SqOnBoard(sq));

      if (pos->pieces[sq + 10] == EMPTY) {
        AddWhitePawnMove(pos, sq, sq + 10, list);
        if (RanksBrd[sq] == RANK_2 && pos->pieces[sq + 20] == EMPTY) {
          AddQuietMove(
              pos, MOVE(sq, (sq + 20), EMPTY, EMPTY, MOVEFLAGPAWNSTART), list);
        }
      }

      if (!SQOFFBOARD(sq + 9) && PieceColour[pos->pieces[sq + 9]] == BLACK) {
        AddWhitePawnCapMove(pos, sq, sq + 9, pos->pieces[sq + 9], list);
      }
      if (!SQOFFBOARD(sq + 11) && PieceColour[pos->pieces[sq + 11]] == BLACK) {
        AddWhitePawnCapMove(pos, sq, sq + 11, pos->pieces[sq + 11], list);
      }

      if (pos->enPassant != NO_SQ) {
        if (sq + 9 == pos->enPassant) {
          AddEnPassantMove(
              pos, MOVE(sq, sq + 9, EMPTY, EMPTY, MOVEFLAGENPASSANT), list);
        }
        if (sq + 11 == pos->enPassant) {
          AddEnPassantMove(
              pos, MOVE(sq, sq + 11, EMPTY, EMPTY, MOVEFLAGENPASSANT), list);
        }
      }
    }

  } else {

    for (pceNum = 0; pceNum < pos->pieceNumber[bP]; ++pceNum) {
      sq = pos->pieceList[bP][pceNum];
      ASSERT(SqOnBoard(sq));

      if (pos->pieces[sq - 10] == EMPTY) {
        AddBlackPawnMove(pos, sq, sq - 10, list);
        if (RanksBrd[sq] == RANK_7 && pos->pieces[sq - 20] == EMPTY) {
          AddQuietMove(
              pos, MOVE(sq, (sq - 20), EMPTY, EMPTY, MOVEFLAGPAWNSTART), list);
        }
      }

      if (!SQOFFBOARD(sq - 9) && PieceColour[pos->pieces[sq - 9]] == WHITE) {
        AddBlackPawnCapMove(pos, sq, sq - 9, pos->pieces[sq - 9], list);
      }

      if (!SQOFFBOARD(sq - 11) && PieceColour[pos->pieces[sq - 11]] == WHITE) {
        AddBlackPawnCapMove(pos, sq, sq - 11, pos->pieces[sq - 11], list);
      }
      if (pos->enPassant != NO_SQ) {
        if (sq - 9 == pos->enPassant) {
          AddEnPassantMove(
              pos, MOVE(sq, sq - 9, EMPTY, EMPTY, MOVEFLAGENPASSANT), list);
        }
        if (sq - 11 == pos->enPassant) {
          AddEnPassantMove(
              pos, MOVE(sq, sq - 11, EMPTY, EMPTY, MOVEFLAGENPASSANT), list);
        }
      }
    }

    /* Loop for slide pieces */
    pceIndex = LoopSlideIndex[side];
    pce = LoopSlidePiece[pceIndex++];
    while (pce != 0) {
      ASSERT(PieceValid(pce));

      for (pceNum = 0; pceNum < pos->pieceNumber[pce]; ++pceNum) {
        sq = pos->pieceList[pce][pceNum];
        ASSERT(SqOnBoard(sq));

        for (index = 0; index < NumDir[pce]; ++index) {
          dir = PceDir[pce][index];
          t_sq = sq + dir;

          while (!SQOFFBOARD(t_sq)) {
            // BLACK ^ 1 == WHITE       WHITE ^ 1 == BLACK
            if (pos->pieces[t_sq] != EMPTY) {
              if (PieceColour[pos->pieces[t_sq]] == (side ^ 1)) {
                AddCaptureMove(pos, MOVE(sq, t_sq, pos->pieces[t_sq], EMPTY, 0),
                               list);
              }
              break;
            }
            AddQuietMove(pos, MOVE(sq, t_sq, EMPTY, EMPTY, 0), list);
            t_sq += dir;
          }
        }
      }

      pce = LoopSlidePiece[pceIndex++];
    }

    /* Loop for non slide */
    pceIndex = LoopNonSlideIndex[side];
    pce = LoopNonSlidePiece[pceIndex++];

    while (pce != 0) {
      ASSERT(PieceValid(pce));

      for (pceNum = 0; pceNum < pos->pieceNumber[pce]; ++pceNum) {
        sq = pos->pieceList[pce][pceNum];
        ASSERT(SqOnBoard(sq));

        for (index = 0; index < NumDir[pce]; ++index) {
          dir = PceDir[pce][index];
          t_sq = sq + dir;

          if (SQOFFBOARD(t_sq)) {
            continue;
          }

          // BLACK ^ 1 == WHITE       WHITE ^ 1 == BLACK
          if (pos->pieces[t_sq] != EMPTY) {
            if (PieceColour[pos->pieces[t_sq]] == (side ^ 1)) {
              AddCaptureMove(pos, MOVE(sq, t_sq, pos->pieces[t_sq], EMPTY, 0),
                             list);
            }
            continue;
          }
          AddQuietMove(pos, MOVE(sq, t_sq, EMPTY, EMPTY, 0), list);
        }
      }

      pce = LoopNonSlidePiece[pceIndex++];
    }
  }
}
