#ifndef GLOB_H
#define GLOB_H

#include "definitions.h"

extern int SQUARE120TOSQUARE64[BRD_SQ_NUM];
extern int SQUARE64TOSQUARE120[64];
extern U64 SetMask[64];
extern U64 ClearMask[64];

extern U64 PieceKey[13][120]; //[Pieces][Square]
extern U64 SideKey;
extern U64 CastleKeys[16]; // 0 0 0 0

extern char PieceCharacter[];
extern char SideCharacter[];
extern char RankCharacter[];
extern char FileCharacter[];

extern int PieceBig[13];
extern int PieceMajor[13];
extern int PieceMinor[13];
extern int PieceValue[13];
extern int PieceColour[13];
extern int PieceSlides[13];

extern int FilesBrd[BRD_SQ_NUM];
extern int RanksBrd[BRD_SQ_NUM];

extern int PieceKnight[13];
extern int PieceKing[13];
extern int PieceRookQueen[13];
extern int PieceBishopQueen[13];

#endif
