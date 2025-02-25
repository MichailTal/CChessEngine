#include "definitions.h"
#include "globals.h"
#include "init.h"
#include "macros.h"
#include "stdio.h"

const int BitTable[64] = {63, 30, 3,  32, 25, 41, 22, 33, 15, 50, 42, 13, 11,
                          53, 19, 34, 61, 29, 2,  51, 21, 43, 45, 10, 18, 47,
                          1,  54, 9,  57, 0,  35, 62, 4,  49, 5,  52, 26, 60,
                          6,  23, 44, 46, 27, 56, 16, 7,  39, 48, 24, 59, 14,
                          12, 55, 38, 28, 58, 20, 37, 17, 36, 8};

int PopBit(U64 *bb) {
  U64 bit = *bb ^ (*bb - 1);
  unsigned int fold = (unsigned)((bit & 0xffffffff) ^ (bit >> 32));
  *bb &= (*bb - 1);
  return BitTable[(fold * 0x783a9b2c) >> 26];
}

int CountBits(U64 bit) {
  int r;
  for (r = 0; bit; r++) {
    bit &= bit - 1;
  }
  return r;
}

void ClearBit(U64 *bb, U64 square) {
  extern U64 SetMask[64];
  (*bb) &= ClearMask[(square)];
}

void SetBit(U64 *bb, U64 square) {
  extern U64 ClearMask[64];
  (*bb) |= SetMask[(square)];
}

void PrintBitBoard(U64 bb) {
  U64 shiftMe = 1ULL;

  int rank = 0;
  int file = 0;
  int square = 0;
  int square64 = 0;

  printf("\n");
  for (rank = RANK_8; rank >= RANK_1; rank--) {
    for (file = FILE_A; file <= FILE_H; file++) {
      square = FILERANK2SQUARE(file, rank);   // 120 based index
      square64 = SQUARE120TOSQUARE64[square]; // 64 based
      if ((shiftMe << square64) & bb) {
        printf("X");
      } else {
        printf("-");
      }
    }
    printf("\n");
  }
  printf("\n\n");
}
