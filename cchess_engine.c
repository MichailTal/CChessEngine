#include "definitions.h"
#include "globals.h"
#include "init.h"
#include "macros.h"
#include "stdio.h"

int main() {
  AllInit();
  U64 playBitBoard = 0ULL; // unsigned 64 bit

  printf("%llu\n", playBitBoard);
  SetBit(&playBitBoard, 31);
  printf("%llu\n", playBitBoard);
  PrintBitBoard(playBitBoard);

  return 0;
}
