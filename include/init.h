#ifndef INIT_H
#define INIT_H

#include "definitions.h"

void AllInit(const char *filePath);
void PrintBitBoard(U64 bb);
int PopBit(U64 *bb);
int CountBits(U64 bit);
U64 Rand_64(void);
U64 GeneratePosKey(const board_representation *pos);
void ResetBoard(board_representation *pos, S_HASHTABLE *table);
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
int GetTimeMs(void);
void InitHashTable(S_HASHTABLE *table, const int MB);
int ProbeHashEntry(board_representation *pos, S_HASHTABLE *table, int *move,
                   int *score, int alpha, int beta, int depth);
void StoreHashEntry(board_representation *pos, S_HASHTABLE *table,
                    const int move, int score, const int flags,
                    const int depth);
int MoveExists(board_representation *pos, const int move);
int GetPvLine(const int depth, board_representation *pos, S_HASHTABLE *table);
int EvalPosition(const board_representation *pos);
void SearchPosition(board_representation *pos, S_SEARCHINFO *info,
                    S_HASHTABLE *table);
void ClearHashTable(S_HASHTABLE *table);
void InitMvvLa(void);
board_representation *GenBoard(void);
void GenerateAllCaps(const board_representation *pos, move_list *list);
void UCI_Loop(board_representation *pos, S_SEARCHINFO *info);
void ReadInput(S_SEARCHINFO *info);
void XBoard_Loop(board_representation *pos, S_SEARCHINFO *info);
void MirrorBoard(board_representation *pos, S_HASHTABLE *table);
void TakeNullMove(board_representation *pos);
void MakeNullMove(board_representation *pos);
int ProbePvMove(const board_representation *pos, S_HASHTABLE *table);
int MoveListOk(const move_list *list, const board_representation *pos);
int SqIs120(const int square);
int GetBookMoves(board_representation *board);
void CleanPolyBook(void);
void InitPolyBook(const char *filePath);
#endif // INIT_H
