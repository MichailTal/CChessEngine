#include "init.h"
#include "definitions.h"
#include "globals.h"
#include "macros.h"
#include <stdio.h>

// Define the global variables here
int SQUARE120TOSQUARE64[BRD_SQ_NUM];
int SQUARE64TOSQUARE120[64];

void InitSquare120ToSquare64() {
  int index = 0;
  int file = FILE_A;
  int rank = RANK_1;
  int square = A1;
  int square64 = 0;

  for (index = 0; index < BRD_SQ_NUM; index++) {
    SQUARE120TOSQUARE64[index] = 65; // impossible value
  }

  for (index = 0; index < 64; index++) {
    SQUARE64TOSQUARE120[index] = 120; // impossible value
  }

  for (rank = RANK_1; rank <= 8; rank++) {
    for (file = FILE_A; file < FILE_H; file++) {
      square = FILERANK2SQUARE(file, rank);
      SQUARE64TOSQUARE120[square64] = square;
      SQUARE120TOSQUARE64[square] = square64;
      square64++;
    }
  }
}

void AllInit() { InitSquare120ToSquare64(); }
