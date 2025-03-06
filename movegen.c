#include "definitions.h"
#include "globals.h"
#include "init.h"
#include "macros.h"
#include "stdio.h"
#include "stdlib.h"

#define MOVE(f, t, ca, pro, fl)                                                \
  ((f) | ((t) << 7) | ((ca) << 14) | ((pro) << 20) | (fl))
#define SQOFFBOARD(sq) (FilesBrd[(sq)] == OFFBOARD)

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

void AddWhitePawnCaptureMove(const board_representation *pos, const int from,
                             const int to, const int cap, move_list *list) {
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

  if (RanksBrd[from] == RANK_7) {
    AddQuietMove(pos, MOVE(from, to, EMPTY, wQ, 0), list);
    AddQuietMove(pos, MOVE(from, to, EMPTY, wR, 0), list);
    AddQuietMove(pos, MOVE(from, to, EMPTY, wB, 0), list);
    AddQuietMove(pos, MOVE(from, to, EMPTY, wN, 0), list);
  } else {
    AddQuietMove(pos, MOVE(from, to, EMPTY, EMPTY, 0), list);
  }
}

void GenerateAllMoves(const board_representation *pos, move_list *list) {

  // ASSERT(CheckBoard(pos));

  list->count = 0;

  int piece = EMPTY;
  int side = pos->side;
  int square = 0;
  int t_sq = 0;
  int pieceNumber = 0;
  int direction = 0;
  int index = 0;
  int pieceIndex = 0;

  if (side == WHITE) {

    for (pieceNumber = 0; pieceNumber < pos->pieceNumber[wP]; ++pieceNumber) {
      square = pos->pieceList[wP][pieceNumber];
      ASSERT(SqOnBoard(square));

      if (pos->pieces[square + 10] == EMPTY) {
        AddWhitePawnMove(pos, square, square + 10, list);
        if (RanksBrd[square] == RANK_2 && pos->pieces[square + 20] == EMPTY) {
          AddQuietMove(
              pos, MOVE(square, (square + 20), EMPTY, EMPTY, MOVEFLAGPAWNSTART),
              list);
        }
      }

      if (!SQOFFBOARD(square + 9) &&
          PieceColour[pos->pieces[square + 9]] == BLACK) {
        AddWhitePawnCaptureMove(pos, square, square + 9,
                                pos->pieces[square + 9], list);
      }
      if (!SQOFFBOARD(square + 11) &&
          PieceColour[pos->pieces[square + 11]] == BLACK) {
        AddWhitePawnCaptureMove(pos, square, square + 11,
                                pos->pieces[square + 11], list);
      }

      if (pos->enPassant != NO_SQ) {
        if (square + 9 == pos->enPassant) {
          AddEnPassantMove(
              pos, MOVE(square, square + 9, EMPTY, EMPTY, MOVEFLAGENPASSANT),
              list);
        }
        if (square + 11 == pos->enPassant) {
          AddEnPassantMove(
              pos, MOVE(square, square + 11, EMPTY, EMPTY, MOVEFLAGENPASSANT),
              list);
        }
      }
    }
  }
}
