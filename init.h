#ifndef INIT_H
#define INIT_H

#include "definitions.h"

void InitSquare120ToSquare64();
void AllInit();
void PrintBitBoard(U64 bb);
int PopBit(U64 *bb);
int CountBits(U64 bit);
void ClearBit(U64 *bb, U64 square);
void SetBit(U64 *bb, U64 square);

#endif // INIT_H
