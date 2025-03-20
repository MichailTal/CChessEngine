#include "../include/definitions.h"
#include "../include/globals.h"
#include "../include/init.h"
#include "../include/macros.h"
#include "stdio.h"

board_representation *GenBoard() {
  board_representation *board =
      (board_representation *)malloc(sizeof(board_representation));
  board->PvTable->pTable = NULL;
  return board;
}

int CheckBoard(const board_representation *pos) {

  int t_pceNum[13] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  int t_bigPce[2] = {0, 0};
  int t_majPce[2] = {0, 0};
  int t_minPce[2] = {0, 0};
  int t_material[2] = {0, 0};

  int square64, t_piece, t_pce_num, square120, colour, pcount;

  U64 t_pawns[3] = {0ULL, 0ULL, 0ULL};

  t_pawns[WHITE] = pos->pawns[WHITE];
  t_pawns[BLACK] = pos->pawns[BLACK];
  t_pawns[BOTH] = pos->pawns[BOTH];

  // check piece list
  for (t_piece = wP; t_piece <= bK; t_piece++) {
    for (t_pce_num = 0; t_pce_num < pos->pieceNumber[t_piece]; t_pce_num++) {
      square120 = pos->pieceList[t_piece][t_pce_num];
      ASSERT(pos->pieces[square120] == t_piece);
    }
  }

  // check piece counter
  for (square64 = 0; square64 < 64; square64++) {
    square120 = SQ120(square64);
    t_piece = pos->pieces[square120];
    t_pceNum[t_piece]++;
    colour = PieceColour[t_piece];
    if (PieceBig[t_piece] == TRUE)
      t_bigPce[colour]++;
    if (PieceMajor[t_piece] == TRUE)
      t_majPce[colour]++;
    if (PieceMinor[t_piece] == TRUE)
      t_minPce[colour]++;

    t_material[colour] += PieceValue[t_piece];
  }

  for (t_piece = wP; t_piece <= bK; ++t_piece) {
    ASSERT(t_pceNum[t_piece] == pos->pieceNumber[t_piece]);
  }

  // check bitboard count
  pcount = CountBits(t_pawns[WHITE]);
  ASSERT(pcount == pos->pieceNumber[wP]);
  pcount = CountBits(t_pawns[BLACK]);
  ASSERT(pcount == pos->pieceNumber[bP]);
  pcount = CountBits(t_pawns[BOTH]);
  ASSERT(pcount == (pos->pieceNumber[wP] + pos->pieceNumber[bP]));

  // check bitboards squares
  while (t_pawns[WHITE]) {
    square64 = PopBit(&t_pawns[WHITE]);
    ASSERT(pos->pieces[SQ120(square64)] == wP);
  }

  while (t_pawns[BLACK]) {
    square64 = PopBit(&t_pawns[BLACK]);
    ASSERT(pos->pieces[SQ120(square64)] == bP);
  }

  while (t_pawns[BOTH]) {
    square64 = PopBit(&t_pawns[BOTH]);
    ASSERT((pos->pieces[SQ120(square64)] == bP) ||
           (pos->pieces[SQ120(square64)] == wP));
  }

  ASSERT(t_material[WHITE] == pos->material[WHITE] &&
         t_material[BLACK] == pos->material[BLACK]);
  ASSERT(t_minPce[WHITE] == pos->minPce[WHITE]);
  ASSERT(t_minPce[BLACK] == pos->minPce[BLACK]);
  ASSERT(t_majPce[WHITE] == pos->majPce[WHITE] &&
         t_majPce[BLACK] == pos->majPce[BLACK]);
  ASSERT(t_bigPce[WHITE] == pos->bigPce[WHITE] &&
         t_bigPce[BLACK] == pos->bigPce[BLACK]);

  ASSERT(pos->side == WHITE || pos->side == BLACK);
  ASSERT(GeneratePosKey(pos) == pos->posKey);

  ASSERT(pos->enPassant == NO_SQ ||
         (RanksBrd[pos->enPassant] == RANK_6 && pos->side == WHITE) ||
         (RanksBrd[pos->enPassant] == RANK_3 && pos->side == BLACK));

  ASSERT(pos->pieces[pos->kingSquare[WHITE]] == wK);
  ASSERT(pos->pieces[pos->kingSquare[BLACK]] == bK);

  ASSERT(pos->castlePermission >= 0 && pos->castlePermission <= 15);

  return TRUE;
}

void UpdateListMaterial(board_representation *pos) {
  int square, piece, index, colour;

  for (index = 0; index < BRD_SQ_NUM; index++) {
    square = index;
    piece = pos->pieces[index];
    if (piece != OFFBOARD && piece != EMPTY) {
      colour = PieceColour[piece];
      if (PieceBig[piece] == TRUE)
        pos->bigPce[colour]++;
      if (PieceMajor[piece] == TRUE)
        pos->majPce[colour]++;
      if (PieceMinor[piece] == TRUE)
        pos->minPce[colour]++;

      pos->material[colour] += PieceValue[piece];

      ASSERT(pos->pieceNumber[piece] < 10 && pos->pieceNumber[piece] >= 0);

      // Piece List
      // PieceList[wP][0] = a1
      pos->pieceList[piece][pos->pieceNumber[piece]] = square;
      pos->pieceNumber[piece]++;

      if (piece == wK)
        pos->kingSquare[WHITE] = square;
      if (piece == bK)
        pos->kingSquare[BLACK] = square;

      if (piece == wP) {
        SETBIT(pos->pawns[WHITE], SQ64(square));
        SETBIT(pos->pawns[BOTH], SQ64(square));
      } else if (piece == bP) {
        SETBIT(pos->pawns[BLACK], SQ64(square));
        SETBIT(pos->pawns[BOTH], SQ64(square));
      }
    }
  }
}

int ParseFen(char *fen, board_representation *pos) {

  ASSERT(fen != NULL);
  ASSERT(pos != NULL);

  int rank = RANK_8;
  int file = FILE_A;
  int piece = 0;
  int count = 0;
  int i = 0;
  int sq64 = 0;
  int sq120 = 0;

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
    case 'k':
      piece = bK;
      break;
    case 'q':
      piece = bQ;
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
    case 'K':
      piece = wK;
      break;
    case 'Q':
      piece = wQ;
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
      printf("FEN error \n");
      return -1;
    }

    for (i = 0; i < count; i++) {
      sq64 = rank * 8 + file;
      sq120 = SQUARE64TOSQUARE120[sq64];
      if (piece != EMPTY) {
        pos->pieces[sq120] = piece;
      }
      file++;
    }
    fen++;
  }

  ASSERT(*fen == 'w' || *fen == 'b');

  pos->side = (*fen == 'w') ? WHITE : BLACK;
  fen += 2;

  for (i = 0; i < 4; i++) {
    if (*fen == ' ') {
      break;
    }
    switch (*fen) {
    case 'K':
      pos->castlePermission |= whiteKingCastle;
      break;
    case 'Q':
      pos->castlePermission |= whiteQueenCastle;
      break;
    case 'k':
      pos->castlePermission |= blackKingCastle;
      break;
    case 'q':
      pos->castlePermission |= blackQueenCastle;
      break;
    default:
      break;
    }
    fen++;
  }
  fen++;

  ASSERT(pos->castlePermission >= 0 && pos->castlePermission <= 15);

  if (*fen != '-') {
    file = fen[0] - 'a';
    rank = fen[1] - '1';

    ASSERT(file >= FILE_A && file <= FILE_H);
    ASSERT(rank >= RANK_1 && rank <= RANK_8);

    pos->enPassant = FILERANK2SQUARE(file, rank);
  }

  pos->posKey = GeneratePosKey(pos);

  UpdateListMaterial(pos);

  return 0;
}

void ResetBoard(board_representation *pos) {

  int index = 0;

  for (index = 0; index < BRD_SQ_NUM; ++index) {
    pos->pieces[index] = OFFBOARD;
  }

  for (index = 0; index < 64; ++index) {
    pos->pieces[SQ120(index)] = EMPTY;
  }

  for (index = 0; index < 2; ++index) {
    pos->bigPce[index] = 0;
    pos->majPce[index] = 0;
    pos->minPce[index] = 0;
    pos->material[index] = 0;
  }

  for (index = 0; index < 3; ++index) {
    pos->pawns[index] = 0ULL;
  }

  for (index = 0; index < 13; ++index) {
    pos->pieceNumber[index] = 0;
  }

  pos->kingSquare[WHITE] = pos->kingSquare[BLACK] = NO_SQ;

  pos->side = BOTH;
  pos->enPassant = NO_SQ;
  pos->fiftyMoveRule = 0;

  pos->ply = 0;
  pos->histPly = 0;

  pos->castlePermission = 0;

  pos->posKey = 0ULL;

  pos->PvTable->pTable = NULL;

  InitPvTable(pos->PvTable);
}

void PrintBoard(const board_representation *pos) {

  int sq, file, rank, piece;

  printf("\nGame Board:\n\n");

  for (rank = RANK_8; rank >= RANK_1; rank--) {
    printf("%d  ", rank + 1);
    for (file = FILE_A; file <= FILE_H; file++) {
      sq = FILERANK2SQUARE(file, rank);
      piece = pos->pieces[sq];
      printf("%3c", PieceCharacter[piece]);
    }
    printf("\n");
  }

  printf("\n   ");
  for (file = FILE_A; file <= FILE_H; file++) {
    printf("%3c", 'a' + file);
  }
  printf("\n");
  printf("side:%c\n", SideCharacter[pos->side]);
  printf("enPas:%d\n", pos->enPassant);
  printf("castle:%c%c%c%c\n",
         pos->castlePermission & whiteKingCastle ? 'K' : '-',
         pos->castlePermission & whiteQueenCastle ? 'Q' : '-',
         pos->castlePermission & blackKingCastle ? 'k' : '-',
         pos->castlePermission & blackQueenCastle ? 'q' : '-');
  printf("PosKey:%llX\n", pos->posKey);
}
