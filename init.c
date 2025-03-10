#include "init.h"
#include "definitions.h"
#include "globals.h"
#include "macros.h"
#include "stdio.h"

// Define the global variables here
int SQUARE120TOSQUARE64[BRD_SQ_NUM];
int SQUARE64TOSQUARE120[64];
U64 SetMask[64];
U64 ClearMask[64];
U64 PieceKey[13][120];
U64 SideKey;
U64 CastleKeys[16];
// int PieceMajor[13];
// int PieceMinor[13];
// int PieceValue[13];
// int PieceColour[13];

int FilesBrd[BRD_SQ_NUM];
int RanksBrd[BRD_SQ_NUM];

void InitFilesRanksBrd() {

  int index = 0;
  int file = FILE_A;
  int rank = RANK_1;
  int sq = A1;

  for (index = 0; index < BRD_SQ_NUM; ++index) {
    FilesBrd[index] = OFFBOARD;
    RanksBrd[index] = OFFBOARD;
  }

  for (rank = RANK_1; rank <= RANK_8; ++rank) {
    for (file = FILE_A; file <= FILE_H; ++file) {
      sq = FILERANK2SQUARE(file, rank);
      FilesBrd[sq] = file;
      RanksBrd[sq] = rank;
    }
  }
}

void InitHashKeys() {

  int index_pieces = 0;
  int index_squares = 0;
  int index_castling = 0;

  for (index_pieces = 0; index_pieces < 13; index_pieces++) {
    for (index_squares = 0; index_squares < 120; index_squares++) {
      PieceKey[index_pieces][index_squares] = Rand_64();
    }
  }
  SideKey = Rand_64();
  for (index_castling = 0; index_castling < 16; index_castling++) {
    CastleKeys[index_castling] = Rand_64();
  }
}

void InitBitMask() {
  int index = 0;

  for (index = 0; index < 64; index++) {
    SetMask[index] = 0ULL;
    ClearMask[index] = 0ULL;
  }

  for (index = 0; index < 64; index++) {
    SetMask[index] |= (1ULL << index);
    ClearMask[index] = ~SetMask[index];
  }
}

void InitSq120To64() {

  int index = 0;
  int file = FILE_A;
  int rank = RANK_1;
  int sq = A1;
  int sq64 = 0;
  for (index = 0; index < BRD_SQ_NUM; ++index) {
    SQUARE120TOSQUARE64[index] = 65;
  }

  for (index = 0; index < 64; ++index) {
    SQUARE64TOSQUARE120[index] = 120;
  }

  for (rank = RANK_1; rank <= RANK_8; ++rank) {
    for (file = FILE_A; file <= FILE_H; ++file) {
      sq = FILERANK2SQUARE(file, rank);
      SQUARE64TOSQUARE120[sq64] = sq;
      SQUARE120TOSQUARE64[sq] = sq64;
      sq64++;
    }
  }
}

void AllInit() {
  InitSq120To64();
  InitBitMask();
  InitHashKeys();
  InitFilesRanksBrd();
}
