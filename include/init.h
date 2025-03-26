#ifndef INIT_H
#define INIT_H

#include "definitions.h"

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
int CheckBoard(const board_representation *pos);
int SqAttacked(const int square, const int side,
               const board_representation *pos);
char *PrintSquare(const int square);
char *PrintMove(const int move);
int SqOnBoard(const int square);
int SideValid(const int side);
int FileRankValid(const int filerank);
int PieceValidEmpty(const int piece);
int PieceValid(const int piece);
void PrintMoveList(const move_list *list);
void GenerateAllMoves(const board_representation *pos, move_list *list);
void TakeMove(board_representation *pos);
int MakeMove(board_representation *pos, const int move);
void PerftTest(int depth, board_representation *pos);
int ParseMove(char *ptrChar, board_representation *pos);
int GetTimeMs();
void InitPvTable(S_PVTABLE *table);
int ProbePvTable(const board_representation *pos);
void StorePvMove(const board_representation *pos, const int move);
int MoveExists(board_representation *pos, const int move);
int GetPvLine(const int depth, board_representation *pos);
int EvalPosition(const board_representation *pos);
void SearchPosition(board_representation *pos, S_SEARCHINFO *info);
void ClearPvTable(S_PVTABLE *table);
void InitMvvLa();
board_representation *GenBoard();
void GenerateAllCaps(const board_representation *pos, move_list *list);
void UCI_Loop();
#endif // INIT_H
