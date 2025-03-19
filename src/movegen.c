#include "../include/definitions.h"
#include "../include/globals.h"
#include "../include/init.h"
#include "../include/macros.h"
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

const int VictimScore[13] = {0, 100, 200, 300, 400, 500, 600, 100, 200, 300, 400, 500, 600};
static int MvvLvaScores[13][13];

void InitMvvLa() {

  int Attacker;
  int Victim;
  
  for (Attacker = wP; Attacker <= bK; ++Attacker) {
    for (Victim = wP; Victim <= bK; ++Victim) {
      MvvLvaScores[Victim][Attacker] = VictimScore[Victim] + 6 - (VictimScore[Attacker] / 100);
    }
  }

  for (Victim = wP; Victim <= bK; ++Victim) {
    for (Attacker = wP; Attacker <= bK; ++Attacker) {
      printf("%c x %c = %d\n", PieceCharacter[Attacker], PieceCharacter[Victim], MvvLvaScores[Victim][Attacker]);
    }
  }
}

int MoveExists(board_representation *pos, const int move) {

  move_list list[1];
  GenerateAllMoves(pos, list);

  int MoveNum = 0;
  for (MoveNum = 0; MoveNum < list->count; ++MoveNum) {

    if (!MakeMove(pos, list->moves[MoveNum].move)) {
      continue;
    }

    TakeMove(pos);
    if (list->moves[MoveNum].move == move) {
      return TRUE;
    }
  }

  return FALSE;
}

static void AddQuietMove(const board_representation *pos, int move,
                         move_list *list) {
  list->moves[list->count].move = move;
  list->moves[list->count].score = 0;
  list->count++;
}

static void AddCaptureMove(const board_representation *pos, int move,
                           move_list *list) {

  list->moves[list->count].move = move;
  list->moves[list->count].score = MvvLvaScores[CAPTURED(move)][pos -> pieces[FROMSQ(move)]];
  list->count++;
}

static void AddEnPassantMove(const board_representation *pos, int move,
                             move_list *list) {

  list->moves[list->count].move = move;
  list->moves[list->count].score = 105;
  list->count++;
}

static void AddWhitePawnCapMove(const board_representation *pos, const int from,
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

static void AddWhitePawnMove(const board_representation *pos, const int from,
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

static void AddBlackPawnCapMove(const board_representation *pos, const int from,
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

static void AddBlackPawnMove(const board_representation *pos, const int from,
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

  int piece = EMPTY;
  int side = pos->side;
  int square = 0;
  int target_square = 0;
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
        AddWhitePawnCapMove(pos, square, square + 9, pos->pieces[square + 9],
                            list);
      }
      if (!SQOFFBOARD(square + 11) &&
          PieceColour[pos->pieces[square + 11]] == BLACK) {
        AddWhitePawnCapMove(pos, square, square + 11, pos->pieces[square + 11],
                            list);
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

    if (pos->castlePermission & whiteKingCastle) {
      if (pos->pieces[F1] == EMPTY && pos->pieces[G1] == EMPTY) {
        if (!SqAttacked(E1, BLACK, pos) && !SqAttacked(F1, BLACK, pos)) {
          AddQuietMove(pos, MOVE(E1, G1, EMPTY, EMPTY, MOVEFLAGECASTLE), list);
        }
      }
    }

    if (pos->castlePermission & whiteQueenCastle) {
      if (pos->pieces[D1] == EMPTY && pos->pieces[C1] == EMPTY &&
          pos->pieces[B1] == EMPTY) {
        if (!SqAttacked(E1, BLACK, pos) && !SqAttacked(D1, BLACK, pos)) {
          AddQuietMove(pos, MOVE(E1, C1, EMPTY, EMPTY, MOVEFLAGECASTLE), list);
        }
      }
    }

  } else {

    for (pieceNumber = 0; pieceNumber < pos->pieceNumber[bP]; ++pieceNumber) {
      square = pos->pieceList[bP][pieceNumber];
      ASSERT(SqOnBoard(square));

      if (pos->pieces[square - 10] == EMPTY) {
        AddBlackPawnMove(pos, square, square - 10, list);
        if (RanksBrd[square] == RANK_7 && pos->pieces[square - 20] == EMPTY) {
          AddQuietMove(
              pos, MOVE(square, (square - 20), EMPTY, EMPTY, MOVEFLAGPAWNSTART),
              list);
        }
      }

      if (!SQOFFBOARD(square - 9) &&
          PieceColour[pos->pieces[square - 9]] == WHITE) {
        AddBlackPawnCapMove(pos, square, square - 9, pos->pieces[square - 9],
                            list);
      }

      if (!SQOFFBOARD(square - 11) &&
          PieceColour[pos->pieces[square - 11]] == WHITE) {
        AddBlackPawnCapMove(pos, square, square - 11, pos->pieces[square - 11],
                            list);
      }
      if (pos->enPassant != NO_SQ) {
        if (square - 9 == pos->enPassant) {
          AddEnPassantMove(
              pos, MOVE(square, square - 9, EMPTY, EMPTY, MOVEFLAGENPASSANT),
              list);
        }
        if (square - 11 == pos->enPassant) {
          AddEnPassantMove(
              pos, MOVE(square, square - 11, EMPTY, EMPTY, MOVEFLAGENPASSANT),
              list);
        }
      }
    }
    if (pos->castlePermission & blackKingCastle) {
      if (pos->pieces[F8] == EMPTY && pos->pieces[G8] == EMPTY) {
        if (!SqAttacked(E8, WHITE, pos) && !SqAttacked(F8, WHITE, pos)) {
          AddQuietMove(pos, MOVE(E8, G8, EMPTY, EMPTY, MOVEFLAGECASTLE), list);
          ;
        }
      }
    }

    if (pos->castlePermission & blackQueenCastle) {
      if (pos->pieces[D8] == EMPTY && pos->pieces[C8] == EMPTY &&
          pos->pieces[B8] == EMPTY) {
        if (!SqAttacked(E8, WHITE, pos) && !SqAttacked(D8, WHITE, pos)) {
          AddQuietMove(pos, MOVE(E8, C8, EMPTY, EMPTY, MOVEFLAGECASTLE), list);
          ;
        }
      }
    }
  }

  /* Loop for slide pieces */
  pieceIndex = LoopSlideIndex[side];
  piece = LoopSlidePiece[pieceIndex++];
  while (piece != 0) {
    ASSERT(PieceValid(piece));

    for (pieceNumber = 0; pieceNumber < pos->pieceNumber[piece];
         ++pieceNumber) {
      square = pos->pieceList[piece][pieceNumber];
      ASSERT(SqOnBoard(square));

      for (index = 0; index < NumDir[piece]; ++index) {
        direction = PceDir[piece][index];
        target_square = square + direction;

        while (!SQOFFBOARD(target_square)) {
          // BLACK ^ 1 == WHITE       WHITE ^ 1 == BLACK
          if (pos->pieces[target_square] != EMPTY) {
            if (PieceColour[pos->pieces[target_square]] == (side ^ 1)) {
              AddCaptureMove(pos,
                             MOVE(square, target_square,
                                  pos->pieces[target_square], EMPTY, 0),
                             list);
            }
            break;
          }
          AddQuietMove(pos, MOVE(square, target_square, EMPTY, EMPTY, 0), list);
          target_square += direction;
        }
      }
    }
    piece = LoopSlidePiece[pieceIndex++];
  }
  /* Loop for non slide */
  pieceIndex = LoopNonSlideIndex[side];
  piece = LoopNonSlidePiece[pieceIndex++];

  while (piece != 0) {
    ASSERT(PieceValid(piece));

    for (pieceNumber = 0; pieceNumber < pos->pieceNumber[piece];
         ++pieceNumber) {
      square = pos->pieceList[piece][pieceNumber];
      ASSERT(SqOnBoard(square));

      for (index = 0; index < NumDir[piece]; ++index) {
        direction = PceDir[piece][index];
        target_square = square + direction;

        if (SQOFFBOARD(target_square)) {
          continue;
        }

        // BLACK ^ 1 == WHITE       WHITE ^ 1 == BLACK
        if (pos->pieces[target_square] != EMPTY) {
          if (PieceColour[pos->pieces[target_square]] == (side ^ 1)) {
            AddCaptureMove(pos,
                           MOVE(square, target_square,
                                pos->pieces[target_square], EMPTY, 0),
                           list);
          }
          continue;
        }
        AddQuietMove(pos, MOVE(square, target_square, EMPTY, EMPTY, 0), list);
      }
    }

    piece = LoopNonSlidePiece[pieceIndex++];
  }
}
