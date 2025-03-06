#include "definitions.h"
#include "globals.h"
#include "init.h"
#include "macros.h"
#include "stdio.h"
#include "stdlib.h"

char PieceCharacter[] = ".PNBRQKpnbrqk";
char SideCharacter[] = "wb-";
char RankCharacter[] = "12345678";
char FileChararacter[] = "abcdefgh";

int PieceBig[13] = {FALSE, FALSE, TRUE, TRUE, TRUE, TRUE, TRUE,
                    FALSE, TRUE,  TRUE, TRUE, TRUE, TRUE};
int PieceMajor[13] = {FALSE, FALSE, FALSE, FALSE, TRUE, TRUE, TRUE,
                      FALSE, FALSE, FALSE, TRUE,  TRUE, TRUE};
int PieceMinor[13] = {FALSE, FALSE, TRUE, TRUE,  FALSE, FALSE, FALSE,
                      FALSE, TRUE,  TRUE, FALSE, FALSE, FALSE};
int PieceValue[13] = {0,   100, 325, 325, 550,  1000, 50000,
                      100, 325, 325, 550, 1000, 50000};
int PieceColour[13] = {BOTH,  WHITE, WHITE, WHITE, WHITE, WHITE, WHITE,
                       BLACK, BLACK, BLACK, BLACK, BLACK, BLACK};

int PiecePawn[13] = {FALSE, TRUE,  FALSE, FALSE, FALSE, FALSE, FALSE,
                     TRUE,  FALSE, FALSE, FALSE, FALSE, FALSE};
int PieceKnight[13] = {FALSE, FALSE, TRUE,  FALSE, FALSE, FALSE, FALSE,
                       FALSE, TRUE,  FALSE, FALSE, FALSE, FALSE};
int PieceKing[13] = {FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, TRUE,
                     FALSE, FALSE, FALSE, FALSE, FALSE, TRUE};
int PieceRookQueen[13] = {FALSE, FALSE, FALSE, FALSE, TRUE, TRUE, FALSE,
                          FALSE, FALSE, FALSE, TRUE,  TRUE, FALSE};
int PieceBishopQueen[13] = {FALSE, FALSE, FALSE, TRUE,  FALSE, TRUE, FALSE,
                            FALSE, FALSE, TRUE,  FALSE, TRUE,  FALSE};
int PieceSlides[13] = {FALSE, FALSE, FALSE, TRUE, TRUE, TRUE, FALSE,
                       FALSE, FALSE, TRUE,  TRUE, TRUE, FALSE};
