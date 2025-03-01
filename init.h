#ifndef INIT_H
#define INIT_H

#include "definitions.h"

void InitSquare120ToSquare64();
void AllInit();
void PrintBitBoard(U64 bb);
int PopBit(U64 *bb);
int CountBits(U64 bit);
U64 Rand_64();
U64 GeneratePosKey(const board_representation *pos);
void ResetBoard(board_representation *pos);
int ParseFen(char *fen, board_representation *pos);
void PrintBoard(const board_representation *pos);
void UpdateListMaterial(board_representation *pos);

#endif // INIT_H
