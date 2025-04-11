#include "../include/init.h"
#include "../include/definitions.h"
#include "../include/globals.h"
#include "../include/macros.h"
#include "stdio.h"

// Define the global variables here
int SQUARE120TOSQUARE64[BRD_SQ_NUM];
int SQUARE64TOSQUARE120[64];
U64 SetMask[64];
U64 ClearMask[64];
U64 PieceKey[13][120];
U64 SideKey;
U64 CastleKeys[16];

int FilesBrd[BRD_SQ_NUM];
int RanksBrd[BRD_SQ_NUM];

U64 FileBBMask[8];
U64 RankBBMask[8];

U64 BlackPassedMask[64];
U64 WhitePassedMask[64];
U64 IsolatedMask[64];

void InitEvalMasks(void) {

  int square, tsq, rank, file;

  for (square = 0; square < 8; ++square) {
    FileBBMask[square] = 0ULL;
    RankBBMask[square] = 0ULL;
  }

  for (rank = RANK_8; rank >= RANK_1; rank--) {
    for (file = FILE_A; file <= FILE_H; file++) {
      square = rank * 8 + file;
      FileBBMask[file] |= (1ULL << square);
      RankBBMask[rank] |= (1ULL << square);
    }
  }

  for (square = 0; square < 64; ++square) {
    IsolatedMask[square] = 0ULL;
    WhitePassedMask[square] = 0ULL;
    BlackPassedMask[square] = 0ULL;
  }

  for (square = 0; square < 64; square++) {
    tsq = square + 8;

    while (tsq < 64) {
      WhitePassedMask[square] |= (1ULL << tsq);
      tsq += 8;
    }

    tsq = square - 8;

    while (tsq >= 0) {
      BlackPassedMask[square] |= (1ULL << tsq);
      tsq -= 8;
    }

    if (FilesBrd[SQ120(square)] > FILE_A) {
      IsolatedMask[square] |= FileBBMask[FilesBrd[SQ120(square)] - 1];

      tsq = square + 7;
      while (tsq < 64) {
        WhitePassedMask[square] |= (1ULL << tsq);
        tsq += 8;
      }

      tsq = square - 8;

      while (tsq >= 0) {
        BlackPassedMask[square] |= (1ULL << tsq);
        tsq -= 8;
      }
    }

    if (FilesBrd[SQ120(square)] < FILE_H) {
      IsolatedMask[square] |= FileBBMask[FilesBrd[SQ120(square)] + 1];

      tsq = square + 9;
      while (tsq < 64) {
        WhitePassedMask[square] |= (1ULL << tsq);
        tsq += 8;
      }

      tsq = square - 7;
      while (tsq >= 0) {
        BlackPassedMask[square] |= (1ULL << tsq);
        tsq -= 8;
      }
    }
  }
}

void InitFilesRanksBrd(void) {

  int index = 0;
  int file = FILE_A;
  int rank = RANK_1;
  int square = A1;

  for (index = 0; index < BRD_SQ_NUM; ++index) {
    FilesBrd[index] = OFFBOARD;
    RanksBrd[index] = OFFBOARD;
  }

  for (rank = RANK_1; rank <= RANK_8; ++rank) {
    for (file = FILE_A; file <= FILE_H; ++file) {
      square = FILERANK2SQUARE(file, rank);
      FilesBrd[square] = file;
      RanksBrd[square] = rank;
    }
  }
}

void InitHashKeys(void) {

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

void InitBitMask(void) {
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

void InitSq120To64(void) {

  int index = 0;
  int file = FILE_A;
  int rank = RANK_1;
  int square = A1;
  int sq64 = 0;
  for (index = 0; index < BRD_SQ_NUM; ++index) {
    SQUARE120TOSQUARE64[index] = 65;
  }

  for (index = 0; index < 64; ++index) {
    SQUARE64TOSQUARE120[index] = 120;
  }

  for (rank = RANK_1; rank <= RANK_8; ++rank) {
    for (file = FILE_A; file <= FILE_H; ++file) {
      square = FILERANK2SQUARE(file, rank);
      SQUARE64TOSQUARE120[sq64] = square;
      SQUARE120TOSQUARE64[square] = sq64;
      sq64++;
    }
  }
}

void AllInit(void) {
  InitSq120To64();
  InitBitMask();
  InitHashKeys();
  InitFilesRanksBrd();
  InitEvalMasks();
  InitMvvLa();
}
