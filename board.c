#include "definitions.h"
#include "globals.h"
#include "init.h"
#include "macros.h"
#include "stdio.h"
#include "stdlib.h"

int Parse_FEN(char *fen, board_representation *pos) {

  ASSERT(fen != NULL);
  ASSERT(pos != NULL);

  int rank = RANK_8; // see how FEN is structured
  int file = FILE_A;
  int piece = 0;
  int count = 0;
  int index = 0;
  int square64 = 0;
  int square120 = 0;

  ResetBoard(pos);

  while ((rank >= RANK_1) && *fen) {
    count = 1;
    switch (*fen) {
    case 'p':
      piece = bP;
      break;
    case 'r':
      piece = bR;
      break;
    case 'n':
      piece = bN;
      break;
    case 'b':
      piece = bB;
      break;
    case 'q':
      piece = bQ;
      break;
    case 'k':
      piece = bK;
      break;
    case 'P':
      piece = wP;
      break;
    case 'R':
      piece = wR;
      break;
    case 'N':
      piece = wN;
      break;
    case 'B':
      piece = wB;
      break;
    case 'Q':
      piece = wQ;
      break;
    case 'K':
      piece = wK;
      break;

    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
      piece = EMPTY;
      count = *fen - '0';
      break;

    case '/':
    case ' ':
      rank--;
      file = FILE_A;
      fen++;
      continue;

    default:
      printf("FEN Error ");
      return -1;
    }

    for (index = 0; index < count; index++) {
      square64 = rank * 8 + file;
      square120 = SQUARE64TOSQUARE120[square64];
      if (piece != EMPTY) {
        pos->pieces[square120] = piece;
      }
      file++;
    }
  }
  fen++;
}

void ResetBoard(board_representation *pos) {
  int index = 0;

  for (index = 0; index < BRD_SQ_NUM; index++) {
    pos->pieces[index] = OFFBOARD;
  }

  for (index = 0; index < 64; index++) {
    pos->pieces[SQUARE64TOSQUARE120[index]] = EMPTY;
  }

  for (index = 0; index < 3; index++) {
    pos->majorPieces[index] = 0;
    pos->minorPieces[index] = 0;
    pos->nonPawnPieces[index] = 0;
    pos->pawns[index] = 0ULL;
  }

  for (index = 0; index < 13; index++) {
    pos->pieceNumber[index] = 0;
  }

  pos->kingSquare[WHITE] = pos->kingSquare[BLACK] = NO_SQ;

  pos->side = BOTH;
  pos->enPassant = NO_SQ;
  pos->fiftyMoveRule = 0;

  pos->ply = 0;
  pos->histPly = 0;

  pos->castlePermission = 0;

  pos->posKey = 0;
}
